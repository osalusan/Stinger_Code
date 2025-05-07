#include "meshfiled.h"
#include "manager/textureManager.h"
#include "component/shaderComponent.h"
#include <fstream>
#include <iostream>
#include <sstream>

constexpr float MESH_SIZE = 12.0f;
constexpr int MESH_DISTANCE = FILED_MAX / 2;

void MeshFiled::FiledDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int loadRowCnt = 0;
	int loadCellCnt = 0;
	std::string stateName = {};

	std::vector<std::string> baseStatas = {};
	std::vector<std::string> stateOptionNameData = {};

	// 1�s���ǂݍ���
	while (std::getline(ifs, line))
	{
		loadCellCnt = 0;

		// �J���}�ŋ�؂�
		std::stringstream ss(line);
		std::string column;
		std::vector<float> skillData = {};

		while (std::getline(ss, column, ','))
		{
			if (column.empty()) continue;

			m_FiledHeight[loadRowCnt][loadCellCnt] = std::stof(column);

			loadCellCnt++;
		}
		loadRowCnt++;
	}
}

MeshFiled::MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture, const wchar_t* textureName, std::string filePath)
{
	m_Texture = texture;
	m_Normal = TEXTURE::MAX;
	TextureManager::ReservDDSTexture(m_Texture, textureName);
	if (filePath != "")
	{
		FiledDataLoadCSV(filePath);
	}
	m_Position = pos;
}

MeshFiled::MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture, const wchar_t* textureName, const TEXTURE& normal, const wchar_t* normalName, std::string filePath)
	:MeshFiled(pos,texture,textureName, filePath)
{
	m_Normal = normal;
	TextureManager::ReservDDSTexture(m_Normal, normalName);
}

void MeshFiled::Init()
{
	GameObject::Init();
	// Vertex�o�b�t�@����
	{
		for (int x = 0; x < FILED_MAX; x++)
		{
			for (int z = 0; z < FILED_MAX; z++)
			{
				m_Vertex[x][z].Position = XMFLOAT3(
					(x - MESH_DISTANCE) * MESH_SIZE,
					m_FiledHeight[x][z],
					(z - MESH_DISTANCE) * -MESH_SIZE);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x * 0.5f, z * 0.5f);
			}
		}
		// �@���x�N�g���̎Z�o
		for (int x = 1; x <= FILED_MAX - 2; x++)
		{
			for (int z = 1; z <= FILED_MAX - 2; z++)
			{
				XMFLOAT3 vx, vz, vn;
				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

				// �O��
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				// ���K��
				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= len;
				vn.y /= len;
				vn.z /= len;

				m_Vertex[x][z].Normal = vn;
			}
		}

		//���_�o�b�t�@�̐���
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * FILED_MAX * FILED_MAX;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	// Index�o�b�t�@����
	{
		// �x���h�~�p
		const size_t indexCount = ((FILED_MAX + 1) * 2) * (FILED_MAX - 1) - 1;
		std::vector<unsigned int> index(indexCount);

		size_t i = 0;
		for (int x = 0; x < FILED_MAX - 1; x++)
		{
			for (int z = 0; z < FILED_MAX; z++)
			{
				index[i++] = x * FILED_MAX + z;
				index[i++] = (x + 1) * FILED_MAX + z;
			}
			if (x == FILED_MAX - 2)
				break;

			// �k�ރ|���S���p�̃C���f�b�N�X
			index[i++] = (x + 1) * FILED_MAX + (FILED_MAX - 1);
			index[i++] = (x + 1) * FILED_MAX;
		}

		// �C���f�b�N�X�o�b�t�@�����p�̐ݒ�
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * indexCount);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = index.data();

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	AddComponent<ShaderComponent>(this);
}

void MeshFiled::Uninit()
{
	GameObject::Uninit();
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Release();
	}
	if (m_IndexBuffer != nullptr)
	{
		m_IndexBuffer->Release();
	}
}

void MeshFiled::Draw()
{
	GameObject::Draw();

	Renderer::SetLight(m_Light);

	//���_�o�b�t�@�̐ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(m_Texture);
	if (texture == nullptr) return;
	ID3D11ShaderResourceView* normal = TextureManager::GetTexture(m_Normal);
	if (normal == nullptr) return;

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &normal);

	//�v���~�e�B�u�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`�� / ���_�쐬���� - ������������
	Renderer::GetDeviceContext()->DrawIndexed((FILED_MAX * 2) * FILED_MAX - 4, 0, 0);
}

float MeshFiled::GetHeight(const XMFLOAT3& position)
{
	int x, z = 0;
	// �u���b�N�ԍ��Z�o
	x = static_cast<int>(position.x / MESH_SIZE + MESH_DISTANCE);
	z = static_cast<int>(position.z / -MESH_SIZE + MESH_DISTANCE);

	XMFLOAT3 pos0, pos1, pos2, pos3 = {};
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p = {};
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x = position.x - pos1.x;
	v1p.y = position.y - pos1.y;
	v1p.z = position.z - pos1.z;

	// �O��
	const float& cy = v12.z * v1p.x - v12.x * v1p.z;

	float py = 0.0f;
	XMFLOAT3 n = {};
	if (cy > 0.0f)
	{
		// ����|���S��
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		// �O��
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		// �E���|���S��
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		// �O��
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	// �����擾
	py = -((position.x - pos1.x) * n.x + (position.z - pos1.z) * n.z) / n.y + pos1.y;
	return py;
}
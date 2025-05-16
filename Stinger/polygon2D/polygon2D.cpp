#include "main/main.h"
#include "polygon2D/polygon2D.h"
#include "manager/textureManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/gameScene.h"
#include "camera/camera.h"
#include "component/shaderComponent.h"

Polygon2D::Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName)
{
	// ������
	m_Texture = TEXTURE::MAX;

	m_Texture = texture;

	m_PivotPoint = pivot;

	if (m_PivotPoint < PIVOT::CENTER && m_PivotPoint > PIVOT::MAX)
	{
		return;
	}

	m_Position.x = position.x;
	m_Position.y = position.y;
	m_Scale.x = size.x;
	m_Scale.y = size.y;

	TextureManager::ReservTexture(m_Texture, fileName);

	SetPolygon(position,size);

	AddComponent<ShaderComponent>(this);
}

Polygon2D::Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName, const bool& ui)
	:Polygon2D(position, size, pivot, texture, fileName)
{
	m_UseUI = ui;
}

Polygon2D::Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName)
	:Polygon2D(position,size,pivot,texture,fileName)
{
	m_UseStencil = useStencil;
	if (m_UseStencil)
	{
		m_Color = { 0.0f,0.0f,0.0f,0.5f };
	}
}

Polygon2D::Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName, const bool& enable)
	:Polygon2D(position, size, pivot, texture, useStencil ,fileName)
{
	m_Enable = enable;
}

void Polygon2D::Init()
{
	GameObject::Init();


	//���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertex;

	if (m_UseUI)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	if (m_CameraCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_CameraCache = objManager->GetCamera();
	}
}

void Polygon2D::Uninit()
{
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Release();
	}
}

void Polygon2D::Update(const float& deltaTime)
{
	// �������z�֐��̈׋����
}

void Polygon2D::Draw()
{
	GameObject::Draw();

	//�}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���_�o�b�t�@�̐ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	if (m_VertexBuffer != nullptr)
	{
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	}

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = !m_UseStencil;
	Renderer::SetMaterial(material);

	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(m_Texture);
	if (texture == nullptr) return;

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	//�v���~�e�B�u�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (m_UseStencil)
	{
		// �X�e���V���ǂݍ��ݗL��
		Renderer::SetStencilEnable(false);
	}

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	if (m_UseStencil)
	{
		// �X�e���V������
		Renderer::SetDepthEnable(true);
		if (m_CameraCache == nullptr) return;
		m_CameraCache->Draw();
	}

}

void Polygon2D::SetPolygon(const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT2& uvValue)
{
	switch (m_PivotPoint)
	{
	case PIVOT::CENTER:		// ����
	{
		const XMFLOAT2& halfSize = { size.x * 0.5f ,size.y * 0.5f };

		m_Vertex[0].Position = XMFLOAT3(position.x - halfSize.x, position.y - halfSize.y, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(position.x + halfSize.x, position.y - halfSize.y, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(uvValue.x, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(position.x - halfSize.x, position.y + halfSize.y, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, uvValue.y);

		m_Vertex[3].Position = XMFLOAT3(position.x + halfSize.x, position.y + halfSize.y, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(uvValue.x, uvValue.y);

		break;
	}
	case PIVOT::LEFT_TOP:	// ����
	{
		const float& right = position.x + size.x;
		const float& bottom = position.y + size.y;

		m_Vertex[0].Position = XMFLOAT3(position.x, position.y, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(right, position.y, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(uvValue.x, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(position.x, bottom, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, uvValue.y);

		m_Vertex[3].Position = XMFLOAT3(right, bottom, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(uvValue.x, uvValue.y);

		break;
	}
	case PIVOT::RIGHT_TOP:	// �E��
	{
		const float& left = position.x - size.x;
		const float& bottom = position.y + size.y;

		m_Vertex[0].Position = XMFLOAT3(left, position.y, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(position.x, position.y, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(uvValue.x, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(left, bottom, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, uvValue.y);

		m_Vertex[3].Position = XMFLOAT3(position.x, bottom, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(uvValue.x, uvValue.y);

		break;
	}
	case PIVOT::LEFT_BOTTOM:	//����
	{
		const float& right = position.x + size.x;
		const float& top = position.y - size.y;

		m_Vertex[0].Position = XMFLOAT3(position.x, top, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(right, top, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(uvValue.x, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(position.x, position.y, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, uvValue.y);

		m_Vertex[3].Position = XMFLOAT3(right, position.y, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(uvValue.x, uvValue.y);

		break;
	}
	case PIVOT::RIGHT_BOTTOM:
	{
		const float& left = position.x - size.x;
		const float& top = position.y - size.y;

		m_Vertex[0].Position = XMFLOAT3(left, top, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(position.x, top, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(uvValue.x, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(left, position.y, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, uvValue.y);

		m_Vertex[3].Position = XMFLOAT3(position.x, position.y, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(uvValue.x, uvValue.y);

		break;
	}
	default:
		break;
	}
}

void Polygon2D::ChangeUVScaling(const XMFLOAT2& uvValue)
{
	if (!m_UseUI) return;

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	const XMFLOAT2& uvScaleSize = { m_Scale.x * uvValue.x, m_Scale.y * uvValue.y };

	SetPolygon(XMFLOAT2(m_Position.x,m_Position.y), uvScaleSize, uvValue);

	memcpy(msr.pData,m_Vertex , sizeof(VERTEX_3D) * 4);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}


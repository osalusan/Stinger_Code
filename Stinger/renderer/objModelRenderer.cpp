#include "main/main.h"
#include "objModelRenderer.h"

ObjModelRenderer::ObjModelRenderer(MODEL* model)
{
	m_Model = model;
}

void ObjModelRenderer::Draw()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Model->VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_Model->SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		if (m_Model->SubsetArray[i].Material.Texture)
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Model->SubsetArray[i].Material.Texture);
		}

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
	}
}

void ObjModelRenderer::DrawCollision()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Model->VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_Model->SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		if (m_Model->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Model->SubsetArray[i].Material.Texture);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
	}
}

void ObjModelRenderer::Uninit()
{
	if (m_Model->VertexBuffer == nullptr || m_Model->IndexBuffer == nullptr)return;

	m_Model->VertexBuffer->Release();
	m_Model->IndexBuffer->Release();

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		if (m_Model->SubsetArray[i].Material.Texture)
		{
			m_Model->SubsetArray[i].Material.Texture->Release();
		}	
	}

	delete[] m_Model->SubsetArray;
	delete[] m_Model;
}

void ObjModelRenderer::SetModel(MODEL* model)
{
	if (model == nullptr) return;

	m_Model = model;
}


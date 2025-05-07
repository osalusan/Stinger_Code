#include "shaderComponent.h"
#include "object/gameObject.h"

ShaderComponent::ShaderComponent(GameObject* gameObject, const std::string& vsFileName, const std::string& psFileName)
	:Component(gameObject)
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, vsFileName.c_str());
	Renderer::CreatePixelShader(&m_PixelShader, psFileName.c_str());
}

ShaderComponent::~ShaderComponent()
{
	// �V�F�[�_�[�̍폜
	if (m_VertexLayout != nullptr)
	{
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
	if (m_VertexShader != nullptr)
	{
		m_VertexShader->Release();
		m_VertexLayout = nullptr;
	}
	if (m_PixelShader != nullptr)
	{
		m_PixelShader->Release();
		m_VertexLayout = nullptr;
	}
}

void ShaderComponent::Init()
{
	// �R���X�g���N�^��shader���w�肵�Ȃ������ꍇ
	if (m_VertexShader == nullptr && m_VertexLayout == nullptr)
	{
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "cso\\unlitTextureVS.cso");
	}
	if (m_PixelShader == nullptr)
	{
		Renderer::CreatePixelShader(&m_PixelShader, "cso\\unlitTexturePS.cso");
	}
}

void ShaderComponent::Uninit()
{
	// �������z�֐��̈׉�����
}

void ShaderComponent::Update()
{
	// �������z�֐��̈׉�����
}

void ShaderComponent::Draw()
{
	if (m_VertexLayout != nullptr && m_VertexShader != nullptr && m_PixelShader != nullptr)
	{
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
	}

	if (m_GameObject == nullptr) return;

	const XMFLOAT3& objScale = m_GameObject->GetScale();
	const XMFLOAT3& objRot = m_GameObject->GetRot();
	const XMFLOAT3& objPos = m_GameObject->GetPos();

	XMMATRIX world, scl, rot, trans;

	scl = XMMatrixScaling(objScale.x, objScale.y, objScale.z);
	if (m_IsUseRotMatrix)
	{
		rot = m_RotationMatrix;
		m_IsUseRotMatrix = false;
	}
	else
	{
		rot = XMMatrixRotationRollPitchYaw(objRot.x, objRot.y, objRot.z);
	}

	trans = XMMatrixTranslation(objPos.x, objPos.y, objPos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);
}

void ShaderComponent::SetRotMatrix(const XMMATRIX& rotMatrix)
{
	m_RotationMatrix = rotMatrix;
	m_IsUseRotMatrix = true;
}

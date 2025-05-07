#pragma once
#include "object/gameObject.h"

class Camera :public GameObject
{
protected:
	XMFLOAT3 m_Target = {};
	float m_Length = 20.0f;
	XMFLOAT4X4 m_ViewMatrix = {};
	bool m_UseRendition = false;
	float m_RendLength = 0.0f;
	float m_RendPosY = {};
	float m_RendTargetPosY = {};
public:
	virtual ~Camera() {};
	virtual void Draw()override;
	
	void SetTarget(const XMFLOAT3& position)
	{
		m_Target = position;
	}
	void SetPosition(const XMFLOAT3& position)
	{
		m_Position = position;
	}
	void SetRendition(const bool& use)
	{
		m_UseRendition = use;
	}
	void SetRenditionLength(const float& length)
	{
		m_RendLength = length;
	}
	void SetRenditionPosY(const float& posY)
	{
		m_RendPosY = posY;
	}
	void SetRenditionTargetPosY(const float& posY)
	{
		m_RendTargetPosY = posY;
	}

	const bool& GetRendition()const
	{
		return m_UseRendition;
	}

	XMMATRIX GetViewMatrix() 
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

	//�O���x�N�g���̎擾
	XMFLOAT3 GetForward()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	//�E�����x�N�g���̎擾
	XMFLOAT3 GetRight()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	//������x�N�g���̎擾
	XMFLOAT3 GetUp()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 up;
		XMStoreFloat3(&up, rotationMatrix.r[1]);
		return up;
	}
	const float& GetLength()const
	{
		return m_Length;
	}
};
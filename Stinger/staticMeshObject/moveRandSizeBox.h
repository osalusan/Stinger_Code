#pragma once
#include "staticMeshObject.h"
// �s�K���ɓ��������`�̃{�b�N�X / �^�C�g���p
class MoveRandSizeBox final: public StaticMeshObject
{
private:
	float m_Speed = 0.0f;				// �ړ����x
	float m_ResetPosZ = 0.0f;			// ���ȏ��Z���W�Ń��Z�b�g 
	float m_UpDownRange = 0.0f;			// �㉺�̕�
	float m_DefaultPosZ = 0.0f;			// ���ł���ꏊ
	float m_RandCenterPosY = 0.0f;		// ��{�ƂȂ鍂��

	XMFLOAT3 m_RotValue = {};

	bool m_Up = false;

	virtual void MoveControl(const float& deltaTime)override;
public:
	MoveRandSizeBox();
	MoveRandSizeBox(const float& resetPosZ, const float& posZ);
};
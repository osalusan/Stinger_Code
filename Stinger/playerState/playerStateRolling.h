#pragma once
#include "playerState.h"
// ����X�e�[�g
class PlayerStateRolling final :public PlayerState
{
private:
	XMFLOAT3 m_MoveVector = {};				// �����͎��̈ړ�������ۑ�
	XMFLOAT3 m_NormalizeVelocity = {};
	float m_RollingSpeed = 0.0f;
	float m_SpeedAttenuateValue = 0.0f;		// ���x�̌����l
	float m_RotSpeed = 0.0f;
	bool m_RollingAccept = false;			// ��𐬌�
	float m_MinRollingAcceptTime = 0.0f;	// �A�j���[�V�������ł����̃X�e�[�g�֍s����悤�ɂȂ鎞��
	float m_MaxRollingAcceptTime = 0.0f;	// ����̍ő厞��
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;

	bool CheckRollingAccept();
};
#pragma once
#include "playerState.h"
// TODO : �������O�ɍ폜�\�� / �f�o�b�O�p 
class PlayerStateJump :public PlayerState
{
private:
	float m_Accel = 0.0f;

public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};
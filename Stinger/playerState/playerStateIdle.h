#pragma once
#include "playerState.h"
// �ҋ@��Ԃ̃X�e�[�g
class PlayerStateIdle final:public PlayerState
{
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};
#include "playerStateIdle.h"
#include "playerStateMachine.h"

void PlayerStateIdle::Init()
{
	LoadAnimation("asset\\model\\player\\Idle_PaladinJNordstrom.fbx", "idle");
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
		m_PlayerMachine->SetAnimeBlendTimeValue(m_BlendTime);
	}
}

void PlayerStateIdle::Unit()
{

}

void PlayerStateIdle::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateIdle::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;
	
	// 優先順位順

	// 攻撃を受けたら
	if (m_PlayerMachine->GetIsHitAttack())
	{
		ChangePlayerState(PLAYER_STATE::HITDAMAGE);
	}
	// パリィ攻撃が押されたら
	else if (m_PlayerMachine->GetIsParryAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
	}
	// 通常攻撃が押されたら
	else if (m_PlayerMachine->GetIsNormalAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
	}
	// 回避が押されたら
	else if (m_PlayerMachine->GetIsRollingButton())
	{
		ChangePlayerState(PLAYER_STATE::ROLLING);
	}
	// 移動していると
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}

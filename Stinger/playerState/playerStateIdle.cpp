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
	
	// �D�揇�ʏ�

	// �U�����󂯂���
	if (m_PlayerMachine->GetIsHitAttack())
	{
		ChangePlayerState(PLAYER_STATE::HITDAMAGE);
	}
	// �p���B�U���������ꂽ��
	else if (m_PlayerMachine->GetIsParryAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
	}
	// �ʏ�U���������ꂽ��
	else if (m_PlayerMachine->GetIsNormalAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
	}
	// ����������ꂽ��
	else if (m_PlayerMachine->GetIsRollingButton())
	{
		ChangePlayerState(PLAYER_STATE::ROLLING);
	}
	// �ړ����Ă����
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}

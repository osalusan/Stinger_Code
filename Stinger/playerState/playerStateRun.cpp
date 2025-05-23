#include "playerStateRun.h"
#include "character/player.h"

void PlayerStateRun::Init()
{
	LoadAnimation("asset\\model\\player\\run_PaladinJNordstrom.fbx", "run");
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->SetAnimeBlendTimeValue(m_BlendTime);
	}
}

void PlayerStateRun::Unit()
{
	// 純粋仮想関数の為空実装
}

void PlayerStateRun::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	if (m_PlayerMachine == nullptr) return;
	const XMFLOAT3& forwardVector = m_PlayerMachine->GetCameraForward();
	const XMFLOAT3& rightVector = m_PlayerMachine->GetCameraRight();

	XMFLOAT3 moveVector = {};

	if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::RIGHT)
	{
		moveVector.x += rightVector.x;
		moveVector.z += rightVector.z;
	}
	else if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::LEFT)
	{
		moveVector.x += (-rightVector.x);
		moveVector.z += (-rightVector.z);
	}

	if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::FORWARD)
	{
		moveVector.x += forwardVector.x;
		moveVector.z += forwardVector.z;
	}
	else if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::BACKWARD)
	{
		moveVector.x += (-forwardVector.x);
		moveVector.z += (-forwardVector.z);
	}

	//斜めでも速度が変わらないように
	XMVECTOR velocityVec = XMLoadFloat3(&moveVector);
	XMVECTOR normalizdVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizeVelocity = {};
	XMStoreFloat3(&normalizeVelocity, normalizdVelocityVec);

	// Getのみ / 編集不可
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	const float& moveSpeed = playerCache->GetMoveSpeed();
	const float& rotSpeed = playerCache->GetRotSpeed();

	m_PlayerMachine->SetVelocityX(normalizeVelocity.x * moveSpeed);
	m_PlayerMachine->SetVelocityZ(normalizeVelocity.z * moveSpeed);

	float currentAngle = m_PlayerMachine->GetRotation().y;
	const float& targetAngle = atan2f(normalizeVelocity.x, normalizeVelocity.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// 少しずつ差を埋める
	currentAngle += angleDiff * rotSpeed * deltaTime;

	if (normalizeVelocity.x != 0.0f || normalizeVelocity.z != 0.0f)
	{
		m_PlayerMachine->SetRotationY(currentAngle);
	}
}

void PlayerStateRun::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	if (m_PlayerMachine->GetIsHitAttack())
	{
		ChangePlayerState(PLAYER_STATE::HITDAMAGE);
	}
	else if (m_PlayerMachine->GetIsParryAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
	}
	else if (m_PlayerMachine->GetIsNormalAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
	}
	else if (m_PlayerMachine->GetIsRollingButton())
	{
		ChangePlayerState(PLAYER_STATE::ROLLING);
	}
	// 移動していないと
	else if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::NONE && m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

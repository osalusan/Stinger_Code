#include "playerStateRolling.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"
#include "character/player.h"

constexpr float BLEND_VALUE_ROLLING = 30.0f;
constexpr float RORING_SPEED_VALUE = 1.24f;

void PlayerStateRolling::Init()
{
	LoadAnimation("asset\\model\\player\\rolling_PaladinJNordstrom.fbx", "rolling");
	m_CurrentTime = 0.0f;
	m_RollingAccept = false;
	m_AnimSpeedValue = RORING_SPEED_VALUE;

	if (m_PlayerMachine != nullptr)
	{
		// �u�����h���Ԑݒ�
		m_PlayerMachine->SetAnimeBlendTimeValue(BLEND_VALUE_ROLLING);
		// Get�̂� / �ҏW�s�� / �ۑ����ꂽ���l���擾
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (m_RollingSpeed == 0.0f && m_PlayerCache != nullptr)
	{
		const std::unordered_map<std::string, float>& rolling = m_PlayerCache->GetStateData("���");

		m_RollingSpeed = FindStateData(rolling, "������̍ō����x_�{��") * m_PlayerCache->GetMoveSpeed();
		m_MinRollingAcceptTime = FindStateData(rolling, "��𐬌�����_�ŏ�");
		m_MaxRollingAcceptTime = FindStateData(rolling, "��𐬌�����_�ő�");
		m_SpeedAttenuateValue = FindStateData(rolling, "������̑��x����_�{��");

		m_RotSpeed = m_PlayerCache->GetRotSpeed();
	}

	if (m_PlayerMachine != nullptr && m_PlayerCache != nullptr)
	{
		// ���x�ݒ�

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

		if (moveVector.x == 0.0f && moveVector.z == 0.0f)
		{
			m_MoveVector.x = m_PlayerCache->GetForward().x;
			m_MoveVector.z = m_PlayerCache->GetForward().z;
		}
		else
		{
			m_MoveVector = moveVector;
		}

		// �΂߂ł����x���ς��Ȃ��悤��
		XMVECTOR velocityVec = XMLoadFloat3(&m_MoveVector);
		XMVECTOR normalizdVelocityVec = XMVector3Normalize(velocityVec);
		XMStoreFloat3(&m_NormalizeVelocity, normalizdVelocityVec);
	}
}

void PlayerStateRolling::Unit()
{
	m_CurrentTime = 0.0f;
	m_RollingAccept = false;
	m_AnimSpeedValue = 0.0f;
}

void PlayerStateRolling::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime * m_AnimSpeedValue;

	// ����̂�
	if (m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimName);
		}
	}

	// �A�j���[�V�������x�ύX
	m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);

	// ��]�Ή�
	float currentAngle = m_PlayerMachine->GetRotation().y;
	const float& targetAngle = atan2f(m_NormalizeVelocity.x, m_NormalizeVelocity.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// ���������𖄂߂�
	currentAngle += angleDiff * m_RotSpeed * deltaTime;

	if (m_NormalizeVelocity.x != 0.0f || m_NormalizeVelocity.z != 0.0f)
	{
		m_PlayerMachine->SetRotationY(currentAngle);
	}

	m_NormalizeVelocity.x *= m_SpeedAttenuateValue;
	m_NormalizeVelocity.z *= m_SpeedAttenuateValue;
	m_PlayerMachine->SetVelocityX(m_NormalizeVelocity.x * m_RollingSpeed);
	m_PlayerMachine->SetVelocityZ(m_NormalizeVelocity.z * m_RollingSpeed);
}

void PlayerStateRolling::ChangeStateControl()
{
	if (m_CurrentTime < m_MaxAnimTime) return;


	// �ړ����Ă����
	if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
	// �������Ă��Ȃ���
	else
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

bool PlayerStateRolling::CheckRollingAccept()
{
	if (!m_RollingAccept)
	{
		if (m_CurrentTime >= m_MinRollingAcceptTime && m_CurrentTime <= m_MaxRollingAcceptTime)
		{
			m_RollingAccept = true;
		}
	}
	return m_RollingAccept;
}

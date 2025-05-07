#include "jumpAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// ���n�ɍ��킹��p
constexpr float LANDING_MIN_VALUE = 0.2f;
constexpr float LANDING_MAX_VALUE = 0.51f;
constexpr float JUMP_MAX_VALUE = 0.36f;
constexpr float INIT_ACCEL = 60.0f;														// �����x
constexpr float JUMP_RESIST = INIT_ACCEL / (JUMP_MAX_VALUE - LANDING_MIN_VALUE);

// ��R��
void JumpAttackTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumpAttack_IsPlace_MawJ.fbx", "jumpAttack");
	m_TaskName = "�W�����v�U��";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
	m_UseDerivation = true;
	m_AttackTask = true;
}

void JumpAttackTask::InitTask(const float& deltaTime)
{
	TaskNode::InitTask(deltaTime);

	m_Accel = INIT_ACCEL;
}

void JumpAttackTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return;
	}
	// �ړ� / �W�����v
	if (m_CurrentTime >= m_MaxAnimTime * LANDING_MIN_VALUE && m_CurrentTime < m_MaxAnimTime * LANDING_MAX_VALUE)
	{
		m_Accel -= (JUMP_RESIST / m_MaxAnimTime) * deltaTime;
		m_BossCache->AddVelocity({ 0.0f ,m_Accel ,0.0f });
		m_BossCache->AddVelocity(m_MoveVector);
		m_BossCache->InitGravity();
	}
	// �U���ꏊ�w��
	else if (m_CurrentTime < m_MaxAnimTime * LANDING_MIN_VALUE)
	{
		const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
		const XMFLOAT3& bossPos = m_BossCache->GetPos();
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		// �ړ������ݒ�
		m_MoveVector.x = (playerPos.x - bossPos.x) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
		m_MoveVector.z = (playerPos.z - bossPos.z) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
	}
	// �_���[�W����
	UseAttack(ATTACK_PARTS::ALL);
}
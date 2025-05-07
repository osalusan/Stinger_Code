#include "backJumpTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// �A�j���[�V�����ɍ��킹��
constexpr float MOVE_MIN_TIME_VALUE = 0.4f;
constexpr float MOVE_MAX_TIME_VALUE = 0.75f;

void BackJumpTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumping_MawJ.fbx", "jump");
	m_TaskName = "�o�b�N�W�����v";
	m_AttackTask = true;
}

void BackJumpTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);

	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return;
	}

	if (m_CurrentTime > m_MaxAnimTime * MOVE_MIN_TIME_VALUE && m_CurrentTime < m_MaxAnimTime * MOVE_MAX_TIME_VALUE)
	{
		const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
		const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

		// ����
		const float& moveSpeed = -m_BossCache->GetMoveSpeed();

		m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });
	}
	else
	{
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
	}
}
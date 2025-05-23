#include "dashAtThePlayerTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void DashAtThePlayerTask::Init()
{
	// 何も設定されていない時
	if (m_Range == 0.0f)
	{
		m_Range = 10.0f;
	}
	ReserveAnimation("asset\\model\\mawJ\\unarmedWalkForward_MawJ.fbx", "walking");
	m_TaskName = "プレイヤーに向かって走る";
}

NODE_STATE DashAtThePlayerTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	// 移動処理
	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

	m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
	const float& moveSpeed = m_BossCache->GetMoveSpeed();

	m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });

	m_BossCache->ChangeAnimation(m_AnimName);

	return NODE_STATE::RUNNING;
}

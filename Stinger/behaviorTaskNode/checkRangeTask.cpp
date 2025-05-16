#include "checkRangeTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void CheckRangeTask::Init()
{
	m_TaskName = "�͈͊m�F";
}

NODE_STATE CheckRangeTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}
	// �U�����s���ׁ̈A��Ԃ�ݒ肵�Ȃ�
	if (m_BossCache->GetRunningNode() != nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& bossPos = m_BossCache->GetPos();

	const float& vectorX = abs(playerPos.x - bossPos.x);
	const float& vectorZ = abs(playerPos.z - bossPos.z);
	const float& range = vectorX + vectorZ;

	// �͈͓��ɓ����Ă�����
	if (range <= m_BossCache->GetShortRange())
	{
		m_BossCache->SetCurrentRange(RANGE::SHORT);
	}
	else if (range <= m_BossCache->GetMiddelRange())
	{
		m_BossCache->SetCurrentRange(RANGE::MIDDLE);
	}
	else if (range <= m_BossCache->GetLongRange())
	{
		m_BossCache->SetCurrentRange(RANGE::LONG);
	}
	else
	{
		m_BossCache->SetCurrentRange(RANGE::NONE);
	}

	return NODE_STATE::FAILURE;
}

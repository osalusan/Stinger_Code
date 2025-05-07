#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

constexpr float STAMINA_HEAL_SPEED_VALUE = 0.7f;

void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle");
	m_TaskName = "�ҋ@";
}

NODE_STATE WaitTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_Wait = true;
		return NODE_STATE::FAILURE;
	}

	if (!m_Wait)
	{
		return NODE_STATE::FAILURE;
	}

	// �����ݒ�
	if (node == nullptr)
	{
		// �X�^�~�i��������Α����قǁA���A���Ԃ������Ȃ�m�����オ��
		const int& randValue = rand() % static_cast<int>(m_BossCache->GetStamina());
		m_WaitValue = randValue * 0.008f;
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime() * m_WaitValue)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// �X�^�~�i��
		m_BossCache->UseStamina(-deltaTime * STAMINA_HEAL_SPEED_VALUE);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ��Ԃ��폜
			m_CurrentTime = 0.0f;
			m_Wait = false;
			m_WaitValue = 1.0f;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}
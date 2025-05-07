#include "checkAttackParry.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void CheckAttackParryTask::Init()
{
	m_TaskName = "�p���B���ꂽ���m�F";
}

NODE_STATE CheckAttackParryTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr )
	{
		return NODE_STATE::SUCCESS;
	}

	if (m_BossCache->GetParryRecoil())
	{
		if (!m_RunningChild)
		{
			// �A�j���[�V�����𒆒f
			if (TaskNode* taskNode = dynamic_cast<TaskNode*>(m_BossCache->GetRunningNode()))
			{
				if (taskNode == nullptr) return NODE_STATE::FAILURE;
				taskNode->CancelRunningTask();

				m_BossCache->SetAnimeTime(taskNode->GetMaxAnimTime());
			}
			
			// ��Ԃ�ۑ�
			m_BossCache->SetRunningNode(nullptr);
		}


		const NODE_STATE& childResult = UpdateChildren(deltaTime);
		if (childResult == NODE_STATE::RUNNING)
		{
			m_RunningChild = true;
		}
		if (childResult == NODE_STATE::SUCCESS)
		{
			m_RunningChild = false;
			m_BossCache->SetParryRecoil(false);
		}

		return childResult;
	}

	return NODE_STATE::FAILURE;
}
#include "parryRecoilTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void ParryRecoilTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standingReact_MawJ.fbx", "parryRecoil");
	m_TaskName = "�p���B�ł̂�낯";
	InitSkillData(m_TaskName);
}

NODE_STATE ParryRecoilTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATE::FAILURE;
	}
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
		m_BossCache->SetAnimationSpeedValue(m_AnimSpeedValue);
		m_BossCache->UseStamina(m_BossCache->GetMaxStamina() * m_UseStaminaValue);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime * m_AnimSpeedValue;
		m_BossCache->ChangeAnimation(m_AnimName);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ��Ԃ��폜
			m_BossCache->SetRunningNode(nullptr);
			m_BossCache->SetAnimationSpeedValue(1.0f);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

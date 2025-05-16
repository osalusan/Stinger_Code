#include "deadTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"

void DeadTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\dying_MawJ.fbx", "dead");
	m_TaskName = "���S";
	InitSkillData(m_TaskName);
}

NODE_STATE DeadTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	if (m_BossCache->GetHealth() > 0.0f)
	{
		return NODE_STATE::FAILURE;
	}

	// ���񂾂Ƃ��ɋ����I�ɓ��鏉����
	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != this)
	{
		m_CurrentTime = 0.0f;
		m_CurrentAnimSpeedTime = 0.0f;
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return NODE_STATE::FAILURE;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return NODE_STATE::FAILURE;
		objManager->SetSlowTime(m_AnimChangeSpeedTime);
		objManager->SetSlowValue(m_AnimSpeedValue);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;

		m_BossCache->ChangeAnimation(m_AnimName);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);

		return NODE_STATE::RUNNING;
	}

	// �A�j���[�V�����d�؂����瓮�����~�߂�
	m_BossCache->SetAnimeTime(m_MaxAnimTime - deltaTime);
	m_BossCache->IsDead();
	return NODE_STATE::SUCCESS;
}
#include "battleStartRenditionTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/textureManager.h"
#include "scene/scene.h"
#include "polygon2D/polygon2D.h"

constexpr XMFLOAT2 DEFAULT_POS_BS_TEXT = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f };
constexpr XMFLOAT2 DEFAULT_SCL_BS_TEXT = { SCREEN_WIDTH * 0.9f,SCREEN_HEIGHT * 0.5f };

void BattleStartRenditionTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\mutantRoaring_MawJ.fbx", "roaring");
	m_TaskName = "�Њd";

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	// �o�g���X�^�[�g��UI
	if (m_BattleStartTextCache == nullptr)
	{
		m_BattleStartTextCache = objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_BS_TEXT, DEFAULT_SCL_BS_TEXT, PIVOT::CENTER, TEXTURE::BATTLESTART_TEXT, L"asset\\texture\\battleStart.png", true);
	}
	if (m_BattleStartTextCache != nullptr)
	{
		m_BattleStartTextCache->ChangeUVScaling(m_BSTextUV);
	}
}

NODE_STATE BattleStartRenditionTask::Update(const float& deltaTime)
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

	// �͈͓��ɓ����Ă�����
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		if(node == nullptr)
		{
			m_CurrentTime = 0.0f;
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BSTextUV.x += deltaTime * 0.7f;

		if (m_BSTextUV.x > 1.0f)
		{
			m_BSTextUV.x = 1.0f;
		}
		if (m_BattleStartTextCache != nullptr)
		{
			m_BattleStartTextCache->ChangeUVScaling(m_BSTextUV);
		}

		m_BossCache->ChangeAnimation(m_AnimName);
		// �U����Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// �U����Ԃ��폜
			m_BossCache->SetRunningNode(nullptr);
			if (m_BattleStartTextCache != nullptr)
			{
				m_BattleStartTextCache->SetEnable(false);
			}
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

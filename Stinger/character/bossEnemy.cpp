#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "manager/objectManager.h"
#include "polygon2D/polygon2D.h"
#include "scene/scene.h"
#include "scene/loadScene.h"
#include <fstream>
#include <iostream>
#include <sstream>

#if _DEBUG
#include "manager/inputManager.h"
#endif // _DEBUG



constexpr XMFLOAT2 DEFAULT_SCALE_HPBAR = { SCREEN_WIDTH * 0.8f,SCREEN_HEIGHT * 0.035f };
constexpr XMFLOAT2 DEFAULT_POS_BS_TEXT = { (SCREEN_WIDTH - DEFAULT_SCALE_HPBAR.x) * 0.5f ,(SCREEN_HEIGHT - DEFAULT_SCALE_HPBAR.y) * 0.01f };
constexpr XMFLOAT2 DEFAULT_SCALE_HPFRAME = { DEFAULT_SCALE_HPBAR.x + (DEFAULT_SCALE_HPBAR.x * 0.008f) ,DEFAULT_SCALE_HPBAR.y + (DEFAULT_SCALE_HPBAR.y * 0.2f) };
constexpr XMFLOAT2 DEFAULT_POS_HPFRAME = { (SCREEN_WIDTH - DEFAULT_SCALE_HPFRAME.x) * 0.5f, DEFAULT_POS_BS_TEXT.y - ((DEFAULT_SCALE_HPFRAME.y - DEFAULT_SCALE_HPBAR.y) * 0.5f)};

// ----------------------- protected -----------------------
void BossEnemy::MoveControl(const float& deltaTime)
{
	// �_���[�W�v�Z�p�Ƀ��Z�b�g
	m_CurrentAttackParts = ATTACK_PARTS::NONE;
	m_AttackDamage = 0.0f;
	m_ParryPossibleAtk = false;

	if (m_Tree != nullptr)
	{
		m_Tree->Update(deltaTime);
	}
#if _DEBUG
	if (InputManager::GetKeyPress('0'))
	{
		TakeDamage(2000);
	}
	if (InputManager::GetKeyPress('9'))
	{
		TakeDamage(1);
	}
#endif // _DEBUG

	CheckWorldWallPos();
}

void BossEnemy::ParameterControl(const float& deltaTime)
{
	Character::ParameterControl(deltaTime);
	m_CurStaminaValue += deltaTime;

	if (m_CurStaminaValue > m_MaxStamina)
	{
		m_CurStaminaValue = m_MaxStamina;
	}
}

// BossEnemy�̃R���X�g���N�^���Ă΂�؂����ザ��Ȃ��ƁATask��dynamic_cast���������Ȃ�
void BossEnemy::CreateTree()
{
	if (m_Tree != nullptr)
	{
		m_Tree->CreateTree(this);
	}
}

// �G�̊�{�f�[�^�̓ǂݍ��� / �r�w�C�r�A�c���[�̍쐬����ɌĂяo��
void BossEnemy::EnemyDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int loadLine = 0;
	int loadCount = 0;
	int loadCellCnt = 0;
	std::string skillName = {};

	std::vector<std::string> baseStatas = {};
	std::vector<std::string> skillOptionNameData = {};

	// 1�s���ǂݍ���
	while (std::getline(ifs, line))
	{
		loadLine++;

		// 1�s�ڂ͓ǂݔ�΂�
		if (loadLine == 1) continue;

		loadCount++;
		loadCellCnt = 0;

		// ��s�̓X�L�b�v
		if (line.empty()) continue;

		// �J���}�ŋ�؂�
		std::stringstream ss(line);
		std::string column;
		std::vector<float> skillData = {};

		while (std::getline(ss, column, ','))
		{
			if (column.empty()) continue;

			loadCellCnt++;

			if (loadCount == 2)
			{
				if (loadCellCnt == 1)
				{
					skillName = column;
				}
				else if(loadCellCnt >= 2)
				{
					skillOptionNameData.emplace_back(column);
				}
				continue;
			}

			// ��b�X�e�[�^�X
			if (loadLine == 2)
			{
				baseStatas.emplace_back(column);
			}
			else // �X�L�����i�[
			{
				skillData.emplace_back(std::stof(column));
			}
		}

		if (loadLine >= 3)
		{
			if (skillData.size() != 0)
			{
				int loop = 0;
				for (const std::string& skillOptionName : skillOptionNameData)
				{
					m_EnemySkillData[skillName].emplace(skillOptionName, skillData[loop]);
					loop++;
				}
			}
		}
		if (loadCount == 1)
		{
			skillOptionNameData.clear();
		}	
		else if (loadCount >= 2)
		{
			loadCount = 0;
		}
	}

	// �X�e�[�^�X�̗v�f��ǉ������������Aif���̐��l���ύX
	if (baseStatas.size() == 11)
	{
		m_MaxHealth = std::stof(baseStatas[0]);
		m_MaxStamina = std::stof(baseStatas[1]);
		m_MoveSpeed = std::stof(baseStatas[2]);
		m_GravityValue = std::stof(baseStatas[3]);
		m_ShortRange = std::stof(baseStatas[4]);
		m_MiddleRange = std::stof(baseStatas[5]);
		m_LongRange = std::stof(baseStatas[6]);
		m_Attack = std::stof(baseStatas[7]);
		m_RotSpeed = std::stof(baseStatas[8]);
		m_MinWaitTime = std::stof(baseStatas[9]);
		m_MaxWaitTime = std::stof(baseStatas[10]);

		m_Health = m_MaxHealth;
		m_CurStaminaValue = m_MaxStamina;
	}
}

// ----------------------- public -----------------------

BossEnemy::BossEnemy(BehaviorTree* tree,const XMFLOAT3& pos)
{
	if (m_Tree == nullptr)
	{
		m_Tree = tree;
	}

	m_Position = pos;

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	LoadScene* loadScene = SceneManager::GetLoadScene();
	if (loadScene == nullptr) return;
	ObjectManager* loadObjManager = loadScene->GetObjectManager();

	// ���[�h�̎���HP�o�[��\�����Ȃ�
	if (loadObjManager->GetBossEnemy() != this)
	{
		// �t���[��
		objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_HPFRAME, DEFAULT_SCALE_HPFRAME, PIVOT::LEFT_TOP, TEXTURE::HPBAR_FRAME_BOSS, L"asset\\texture\\white.png");
		// HP�o�[�̃o�b�N�O���E���h
		objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_BS_TEXT, DEFAULT_SCALE_HPBAR, PIVOT::LEFT_TOP, TEXTURE::HPBAR_BG_BOSS, L"asset\\texture\\black.png");
	}

	// �{�X��UI
	if (m_EnemyHpCache == nullptr)
	{
		// �̗�
		m_EnemyHpCache = objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_BS_TEXT, DEFAULT_SCALE_HPBAR, PIVOT::LEFT_TOP, TEXTURE::HPBAR_BOSS, L"asset\\texture\\boss_HpBar.png", true);
	}

}

BossEnemy::BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos, const XMFLOAT3& scale)
	:BossEnemy(tree,pos)
{
	m_Scale = scale;
}

BossEnemy::BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot)
	:BossEnemy(tree,pos,scale)
{
	m_Rotation = rot;
}

BossEnemy::~BossEnemy()
{
	m_EnemySkillData.clear();

	delete m_Tree;
	m_Tree = nullptr;

	m_RunningNodeCache = nullptr;
}

void BossEnemy::TakeDamage(const float& atk)
{
	if (atk <= 0 || m_Health <= 0) return;

	m_Health -= atk;

	if (m_EnemyHpCache != nullptr)
	{
		const XMFLOAT2& size = { DEFAULT_SCALE_HPBAR.x * (m_Health / m_MaxHealth),DEFAULT_SCALE_HPBAR.y };
		m_EnemyHpCache->ChangeUVScaling({ m_Health / m_MaxHealth ,1.0f });
	}

	if (m_Health <= 0)
	{
		m_Health = 0;
	}
}

void BossEnemy::RotToTarget(GameObject* obj,const float& deltaTime)
{
	if (obj == nullptr) return;
	const XMFLOAT3& enemyPos = m_Position;
	const XMFLOAT3& targetPos = obj->GetPos();

	float currentAngle = m_Rotation.y;
	const float& targetAngle = atan2f(targetPos.x - enemyPos.x, targetPos.z - enemyPos.z);

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

	m_Rotation.y = currentAngle;
}

void BossEnemy::RotToTarget(GameObject* obj)
{
	if (obj == nullptr) return;
	const XMFLOAT3& enemyPos = m_Position;
	const XMFLOAT3& targetPos = obj->GetPos();

	float currentAngle = m_Rotation.y;
	const float& targetAngle = atan2f(targetPos.x - enemyPos.x, targetPos.z - enemyPos.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	m_Rotation.y = currentAngle += angleDiff;
}

bool BossEnemy::UseStamina(const float& use)
{
	if (m_CurStaminaValue - use < 0.0f)
	{
		return false;
	}

	m_CurStaminaValue -= use;

	return true;
}

void BossEnemy::SetRunningNode(BehaviorNode* node)
{
	m_RunningNodeCache = node;
	if (node == nullptr)
	{
		m_AttackAccept = false;
	}
}


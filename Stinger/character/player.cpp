#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "manager/textureManager.h"
#include "renderer/fbxModelRenderer.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"
#include "scene/scene.h"
#include "staticMeshObject/equipmentObject.h"
#include "polygon2D/polygon2D.h"
#include <fstream>
#include <iostream>
#include <sstream>

// �v���C���[�̏��
constexpr XMFLOAT3 DEFAULT_SCALE_PLAYER = { 0.053f,0.053f,0.053f };
constexpr XMFLOAT3 DEFAULT_SCALE_SWORD = { DEFAULT_SCALE_PLAYER.x * 48.5f ,DEFAULT_SCALE_PLAYER.y * 48.5f ,DEFAULT_SCALE_PLAYER.z * 48.5f };
constexpr XMFLOAT3 DEFAULT_SCALE_SHILED = { DEFAULT_SCALE_PLAYER.x * 36.0f ,DEFAULT_SCALE_PLAYER.y * 36.0f ,DEFAULT_SCALE_PLAYER.z * 36.0f };
constexpr XMFLOAT3 OFFSET_POS_SWORD = { DEFAULT_SCALE_PLAYER.x * -10.0f, DEFAULT_SCALE_PLAYER.y * -3.0f, DEFAULT_SCALE_PLAYER.z * -1.0f };
constexpr XMFLOAT3 OFFSET_POS_SHILED = { 0.0f, DEFAULT_SCALE_PLAYER.y * -10.0f, DEFAULT_SCALE_PLAYER.z * 10.0f };
constexpr float OFFSET_SCL_SWORD_Y = DEFAULT_SCALE_SWORD.y * 0.25f;
// �v���C���[��UI���
constexpr XMFLOAT2 DEFAULT_SCALE_HPBAR = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.032f};
constexpr XMFLOAT2 DEFAULT_POS_BS_TEXT = { (SCREEN_WIDTH - DEFAULT_SCALE_HPBAR.x) * 0.5f ,(SCREEN_HEIGHT - DEFAULT_SCALE_HPBAR.y) * 0.98f};
constexpr XMFLOAT2 DEFAULT_SCALE_HPFRAME = { DEFAULT_SCALE_HPBAR.x + (DEFAULT_SCALE_HPBAR.x * 0.005f) ,DEFAULT_SCALE_HPBAR.y + (DEFAULT_SCALE_HPBAR.y * 0.15f) };
constexpr XMFLOAT2 DEFAULT_POS_HPFRAME = { (SCREEN_WIDTH - DEFAULT_SCALE_HPFRAME.x) * 0.5f, DEFAULT_POS_BS_TEXT.y - ((DEFAULT_SCALE_HPFRAME.y - DEFAULT_SCALE_HPBAR.y) * 0.5f) };

constexpr const char* RIGHTHAND_NAME_PLAYER = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME_PLAYER = "mixamorig:LeftHand";

Player::Player(const XMFLOAT3& pos)
{
	ReservModel(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\paladinJNordstrom.fbx");
	m_Position = pos;
}

Player::~Player()
{
	m_PlayerHpCache = nullptr;
	m_ShiledChache = nullptr;
	m_SwordChache = nullptr;
	delete m_PlayerStateMachine;
	m_PlayerStateMachine = nullptr;
}

void Player::Init()
{
	Character::Init();
	// ��ԍŏ��ɌĂ�
	PlayerDataLoadCSV("asset\\csv\\playerData.csv");

	m_Scale = DEFAULT_SCALE_PLAYER;
	m_EnableGravity = true;

	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::PLAYER);
	// �����蔻��̌�ɒǉ�
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");

	if (m_PlayerStateMachine == nullptr)
	{
		m_PlayerStateMachine = new PlayerStateMachine(this);
	}
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Init();
	}

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	// �v���C���[�̑���
	if (m_SwordChache == nullptr)
	{
		m_SwordChache = objManager->AddGameObjectArg<EquipmentObject>(OBJECT::STATICMESH, this, STATICMESH_MODEL::SOWRD, "asset\\model\\sword\\andlangr_sword.obj", m_Model, RIGHTHAND_NAME_PLAYER, DEFAULT_SCALE_SWORD, XMFLOAT3(0.0f, 0.0f, 5.3f), OFFSET_POS_SWORD);
	}
	if (m_ShiledChache == nullptr)
	{
		m_ShiledChache = objManager->AddGameObjectArg<EquipmentObject>(OBJECT::STATICMESH, this, STATICMESH_MODEL::SHIELD, "asset\\model\\shield\\shield.obj", m_Model, LEFTHAND_NAME_PLAYER, DEFAULT_SCALE_SHILED, XMFLOAT3(-0.5f, 0.0f, 0.0f), OFFSET_POS_SHILED);
	}

	if (m_SwordChache != nullptr)
	{
		m_SwordChache->SetOffsetCollScl({ 0.0f,OFFSET_SCL_SWORD_Y,0.0f });
	}

	// �t���[��
	objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_HPFRAME, DEFAULT_SCALE_HPFRAME, PIVOT::LEFT_TOP, TEXTURE::WHITE, L"asset\\texture\\white.png");
	// HP�o�[�̃o�b�N�O���E���h
	objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_BS_TEXT, DEFAULT_SCALE_HPBAR, PIVOT::LEFT_TOP, TEXTURE::BLACK, L"asset\\texture\\black.png");
	
	// �v���C���[��UI
	if (m_PlayerHpCache == nullptr)
	{
		m_PlayerHpCache = objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_BS_TEXT, DEFAULT_SCALE_HPBAR, PIVOT::LEFT_TOP, TEXTURE::HPBAR_PLAYER, L"asset\\texture\\player_HpBar.png",true);
	}
}

void Player::Uninit()
{
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Uninit();
	}
	Character::Uninit();
}

void Player::TakeDamage(const float& atk)
{
	if (atk <= 0 || m_Health <= 0) return;
	if (m_PlayerStateMachine->CheckRolling()) return;

	m_PlayerStateMachine->SetIsHitAttack(true);

	m_Health -= atk;

	m_CurTakeNoDmgTime = 0.0f;

	if (m_PlayerHpCache != nullptr)
	{
		const XMFLOAT2& size = { DEFAULT_SCALE_HPBAR.x * (m_Health / m_MaxHealth),DEFAULT_SCALE_HPBAR.y};
		m_PlayerHpCache->ChangeUVScaling({ m_Health / m_MaxHealth ,1.0f});
	}

	if (m_Health <= 0)
	{
		m_Health = 0;
	}
}

void Player::TakeDamageParryPossible(const float& atk)
{
	if (m_PlayerStateMachine == nullptr) return;

	if (m_PlayerStateMachine->CheckParry()) return;

	TakeDamage(atk);
}

// ���Ƀq�b�g���̂�
void Player::HitShiled(BoxCollisionComponent* boxColl)
{
	if (m_PlayerStateMachine == nullptr || boxColl == nullptr) return;

	std::vector<EquipmentObject*> hitObjcts = {};
	boxColl->GetHitGameObjects(hitObjcts);

	for (EquipmentObject* hitObj : hitObjcts)
	{
		if (hitObj == m_ShiledChache)
		{
			m_PlayerStateMachine->CheckParry();
			break;
		}
	}
}

// ------------------------------- private -------------------------------
void Player::MoveControl(const float& deltaTime)
{
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Update(deltaTime);
	}
	// ���R��
	m_CurTakeNoDmgTime += deltaTime;

	if (m_CurTakeNoDmgTime >= m_HealStartTime && m_Health > 0.0f)
	{
		m_Health += deltaTime * m_HealSpeed;

		if (m_Health >= m_MaxHealth)
		{
			m_Health = m_MaxHealth;
		}

		if (m_PlayerHpCache != nullptr)
		{
			const XMFLOAT2& size = { DEFAULT_SCALE_HPBAR.x * (m_Health / m_MaxHealth),DEFAULT_SCALE_HPBAR.y };
			m_PlayerHpCache->ChangeUVScaling({ m_Health / m_MaxHealth ,1.0f });
		}
	}
}

void Player::CustomCollisionInfo()
{
	const XMFLOAT3& customScale = { m_Scale.x * 0.4f ,m_Scale.y ,m_Scale.z * 1.0f };
	if (m_BoxCollCache == nullptr) return;

	if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, customScale, model->GetCenter(), model->GetScale(), GetRotationMatrix());
	}
}

void Player::CollisionControl()
{
	if (m_PlayerStateMachine == nullptr) return;

	if (m_BoxCollCache == nullptr) return;

	SwordCollisionControl();

	const int& maxCount = 5;
	int count = 0;
	XMFLOAT3 recPos = {};
	while(m_BoxCollCache->CheckHitObject(COLLISION_TAG::OBJECT))
	{
		count++;
		if (m_BoxCollCache->GetHitGameObject<GameObject>() == nullptr) break;

		XMVECTOR mtv = m_BoxCollCache->GetMtv();

		// �ʒu��MTV�������ړ�
		XMVECTOR playerVectorPos = XMLoadFloat3(&m_Position);
		playerVectorPos = XMVectorAdd(playerVectorPos, mtv);
		XMStoreFloat3(&m_Position, playerVectorPos);

		// �p�̖ڂ荞�݃o�O�h�~
		std::vector<GameObject*> objects = {};
		m_BoxCollCache->GetHitGameObjects(objects);
		if (objects.size() >= 2)
		{
			if (m_Position.x == recPos.x && m_Position.z == recPos.z)
			{
				m_Position.x = m_RecordPosition.x;
				m_Position.z = m_RecordPosition.z;
				count = maxCount;
			}
			else
			{
				recPos = m_Position;
			}
		}

		m_BoxCollCache->SetPos(m_Position);

		// ���ɉ����o���Ă��邩��n�ʂ̍����ŕ␳���Ȃ��Ă���
		if (mtv.m128_f32[1] > 0.0f)
		{
			m_Velocity.y = 0.0f;
			m_PlayerStateMachine->HitGround();
		}
		if (count >= maxCount) break;	// �������[�v���Ȃ��悤��
	}

	count = 0;
	while (m_BoxCollCache->CheckHitObject(COLLISION_TAG::ENEMY_BOSS))
	{
		count++;
		if (m_BoxCollCache->GetHitGameObject<GameObject>() == nullptr) break;

		XMVECTOR mtv = m_BoxCollCache->GetMtv();

		// �ʒu��MTV�������ړ�
		XMVECTOR playerVectorPos = XMLoadFloat3(&m_Position);
		playerVectorPos = XMVectorAdd(playerVectorPos, mtv);
		XMStoreFloat3(&m_Position, playerVectorPos);

		if (count >= maxCount) break;	// �������[�v���Ȃ��悤��
	}

	CheckWorldWallPos();

	float groundHeight = 0.0f;

	if (m_MeshFiledCache != nullptr)
	{
		groundHeight = m_MeshFiledCache->GetHeight(m_Position);
	}

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
		ResetGravity();
		// �n�ʂɐG��Ă��邩�ǂ�����`����
		m_PlayerStateMachine->HitGround();
	}
}

void Player::AnimationControl()
{
	if (m_PlayerStateMachine != nullptr)
	{
		ChangeAnimation(m_PlayerStateMachine->GetAnimation());
	}
}

void Player::SwordCollisionControl()
{
	if (m_SwordBoxCollCache == nullptr && m_SwordChache != nullptr)
	{
		std::vector<BoxCollisionComponent*> boxColls = {};
		m_SwordChache->GetComponents<BoxCollisionComponent>(boxColls);

		for (BoxCollisionComponent* boxColl : boxColls)
		{
			m_SwordBoxCollCache = boxColl;
		}
	}

	if (m_SwordBoxCollCache == nullptr || m_PlayerStateMachine == nullptr) return;

	if (m_SwordBoxCollCache->CheckHitObject(OBJECT::BOSS))
	{
		// �U������������_���[�W��^����
		m_PlayerStateMachine->CheckAttack();
	}
}

// �G�̊�{�f�[�^�̓ǂݍ��� / �r�w�C�r�A�c���[�̍쐬����ɌĂяo��
void Player::PlayerDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int loadLine = 0;
	int loadCount = 0;
	int loadCellCnt = 0;
	std::string stateName = {};

	std::vector<std::string> baseStatas = {};
	std::vector<std::string> stateOptionNameData = {};

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
					stateName = column;
				}
				else if (loadCellCnt >= 2)
				{
					stateOptionNameData.emplace_back(column);
				}
				continue;
			}

			// ��b�X�e�[�^�X
			if (loadLine == 2)
			{
				baseStatas.emplace_back(column);
			}
			else // �X�e�[�g�����i�[
			{
				skillData.emplace_back(std::stof(column));
			}
		}

		if (loadLine >= 3)
		{
			if (skillData.size() != 0)
			{
				int loop = 0;
				for (const std::string& stateOptionName : stateOptionNameData)
				{
					m_PlayerStateData[stateName].emplace(stateOptionName, skillData[loop]);
					loop++;
				}
			}
		}
		if (loadCount == 1)
		{
			stateOptionNameData.clear();
		}
		else if (loadCount >= 2)
		{
			loadCount = 0;
		}
	}

	// �X�e�[�^�X�̗v�f��ǉ������������Aif���̐��l���ύX
	if (baseStatas.size() == 7)
	{
		m_MaxHealth = std::stof(baseStatas[0]);
		m_Attack = std::stof(baseStatas[1]);
		m_MoveSpeed = std::stof(baseStatas[2]);
		m_GravityValue = std::stof(baseStatas[3]);
		m_RotSpeed = std::stof(baseStatas[4]);
		m_HealStartTime = std::stof(baseStatas[5]);
		m_HealSpeed = std::stof(baseStatas[6]);

		m_Health = m_MaxHealth;
	}
}

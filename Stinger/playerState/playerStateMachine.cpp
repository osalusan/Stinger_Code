#include "playerStateMachine.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/textureManager.h"
#include "camera/playerCamera.h"
#include "scene/gameScene.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "polygon2D/polygon2D.h"
#include "playerState.h"
#include "playerStateIdle.h"
#include "playerStateRun.h"
#include "playerStateJump.h"
#include "playerStateParryAttack.h"
#include "playerStateNormalAttack.h"
#include "playerState/playerStateRolling.h"
#include "playerState/playerStateExtrAttack.h"
#include "playerState/playerStateHitAttack.h"

constexpr int EXTRATTACK_ACCEPT_PARRY_MAX = 3;
constexpr XMFLOAT2 DEFAULT_SCALE_PARRYGAGE = { SCREEN_WIDTH * 0.4f,SCREEN_HEIGHT * 0.028f };
constexpr XMFLOAT2 DEFAULT_POS_PARRYGAGE = { (SCREEN_WIDTH - DEFAULT_SCALE_PARRYGAGE.x) * 0.5f ,(SCREEN_HEIGHT - DEFAULT_SCALE_PARRYGAGE.y) * 0.925f };
constexpr XMFLOAT2 DEFAULT_SCALE_PARRYFRAME = { DEFAULT_SCALE_PARRYGAGE.x + (DEFAULT_SCALE_PARRYGAGE.x * 0.007f) ,DEFAULT_SCALE_PARRYGAGE.y + (DEFAULT_SCALE_PARRYGAGE.y * 0.19f) };
constexpr XMFLOAT2 DEFAULT_POS_PARRYFRAME = { (SCREEN_WIDTH - DEFAULT_SCALE_PARRYFRAME.x) * 0.5f, DEFAULT_POS_PARRYGAGE.y - ((DEFAULT_SCALE_PARRYFRAME.y - DEFAULT_SCALE_PARRYGAGE.y) * 0.5f) };

// -------------------------------------- private --------------------------------------

void PlayerStateMachine::InputReset()
{
	m_RandL = MOVE_DIRECTION::NONE;
	m_FandB = MOVE_DIRECTION::NONE;
	m_IsJamp = false;
	m_IsParryAttackButton = false;
	m_IsNormalAttackButton = false;
	m_IsRollingButton = false;
	m_IsExtrAttack = false;
	m_IsParry = false;
}
// -------------------------------------- public --------------------------------------

PlayerStateMachine::PlayerStateMachine(Player* player)
{
	if (player == nullptr || m_PlayerCache != nullptr) return;
	m_PlayerCache = player;
}

PlayerStateMachine::~PlayerStateMachine()
{
	for (auto& PlayerStatePool : m_PlayerStatePool)
	{
		delete PlayerStatePool.second;
		PlayerStatePool.second = nullptr;
	}
	m_PlayerStatePool.clear();
	m_PlayerCache = nullptr;
}

void PlayerStateMachine::Init()
{
	if (m_PlayerStatePool.empty())
	{
		m_ParryCache = new PlayerStateParryAttack(this);
		m_RollingCache = new PlayerStateRolling(this);
		m_NormalAttackCache = new PlayerStateNormalAttack(this);
		m_ExtrAttackCache = new PlayerStateExtrAttack(this);
		// �v�f��������n�b�V��
		m_PlayerStatePool.reserve(static_cast<int>(PLAYER_STATE::MAX) - 1);

		m_PlayerStatePool.emplace(PLAYER_STATE::IDLE, new PlayerStateIdle(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::RUN, new PlayerStateRun(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_PARRY, m_ParryCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_NORMAL, m_NormalAttackCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_EXTR, m_ExtrAttackCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ROLLING, m_RollingCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::HITATTACK, new PlayerStateHitAttack(this));
	}
	// ������
	for (const std::pair<PLAYER_STATE, PlayerState*>& PlayerState : m_PlayerStatePool)
	{
		m_PlayerStatePool[PlayerState.first]->Init();
	}

	if (m_CurrentPlayerState == nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[PLAYER_STATE::IDLE];
	}

	// �e�N�X�`���̐���
	if (m_ParryGageCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		// �p���B�Q�[�W�̃o�b�O�O���E���h
		objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_PARRYFRAME, DEFAULT_SCALE_PARRYFRAME, PIVOT::LEFT_TOP, TEXTURE::BLACK, L"asset\\texture\\black.png");

		m_ParryGageCache = objManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D, DEFAULT_POS_PARRYGAGE, DEFAULT_SCALE_PARRYGAGE, PIVOT::LEFT_TOP, TEXTURE::PARRYGAGE, L"asset\\texture\\parryGage.png", true);
	}
	if (m_ParryGageCache != nullptr)
	{
		m_ParryGageCache->ChangeUVScaling({ 0.0f,1.0f });
	}
}

void PlayerStateMachine::Uninit()
{
	for (const auto& PlayerStatePool : m_PlayerStatePool)
	{
		// null�`�F�b�N
		if (PlayerStatePool.second != nullptr)
		{
			PlayerStatePool.second->Unit();
		}
	}
}
// ��Ɉړ��̏���
void PlayerStateMachine::Update(const float& deltaTime)
{
	// �쐬���ԓI�ɁAUpdate�ŕۑ�����
	if (m_CameraCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;
		m_CameraCache = objManager->GetCamera();

		if (m_CameraCache == nullptr) return;
	}
	if (m_BossCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_BossCache = objManager->GetBossEnemy();
	}

	// ���͂̑O�Ƀ��Z�b�g
	InputReset();

	// �U��
	if (InputManager::GetMouseRightPress())
	{
		m_IsParryAttackButton = true;
	}
	if (InputManager::GetMouseLeftPress())
	{
		m_IsNormalAttackButton = true;

		if (m_ParryCount >= EXTRATTACK_ACCEPT_PARRY_MAX)
		{
			m_IsExtrAttack = true;
		}
	}
	// ���
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsRollingButton = true;
	}
	// �ړ�
	if (InputManager::GetKeyPress('A'))
	{
		m_RandL = MOVE_DIRECTION::LEFT;
	}
	if (InputManager::GetKeyPress('D'))
	{
		m_RandL = MOVE_DIRECTION::RIGHT;
	}
	if (InputManager::GetKeyPress('W'))
	{
		m_FandB = MOVE_DIRECTION::FORWARD;
	}
	if (InputManager::GetKeyPress('S'))
	{
		m_FandB = MOVE_DIRECTION::BACKWARD;
	}

	//�W�����v
	if (m_IsGround && InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsJamp = true;
	}

	// �X�e�[�g�̍X�V�O�A���͏����̌�
	if (m_CameraCache != nullptr && m_BossCache != nullptr)
	{
		if (m_CameraCache->GetRendition())
		{
			InputReset();

			const XMFLOAT3& myPos = m_PlayerCache->GetPos();
			const XMFLOAT3& targetPos = m_BossCache->GetPos();
			const float& directionx = targetPos.x - myPos.x;
			const float& directionz = targetPos.z - myPos.z;

			float rotY = std::atan2(directionx, directionz);
			m_Rotation.y = rotY;
		}
	}

	// TODO :���S�X�e�[�g���쐬������폜
	if (m_PlayerCache->GetHealth() <= 0.0f)
	{
		InputReset();
	}

	// ���͏����̌��
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
		m_CurrentPlayerState->ChangeStateControl();
	}

	// �X�e�[�g�̍X�V�̌�ɌĂ�
	if (m_PlayerCache != nullptr)
	{
		m_PlayerCache->SetVelocityX(m_Velocity.x);
		m_PlayerCache->SetVelocityZ(m_Velocity.z);

		// Y����+
		m_PlayerCache->SetVelocityY(m_PlayerCache->GetVelocity().y + m_Velocity.y);

		m_PlayerCache->SetRotationY(m_Rotation.y);

		if (m_AnimeBlendTimeValue != 0.0f)
		{
			m_PlayerCache->SetBlendTimeValue(m_AnimeBlendTimeValue);
		}
	}

	// �Ō�Ƀ��Z�b�g
	m_IsGround = false;
	m_IsHitAttacked = false;
}

void PlayerStateMachine::SetPlayerState(const PLAYER_STATE& state)
{
	// �����X�e�[�g��������͂���
	if (m_CurrentPlayerState == m_PlayerStatePool[state]) return;

	// �I������
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Unit();
	}

	// �A�j���[�V�������x���A�O�̃X�e�[�g��������p���Ȃ��悤��
	SetAnimationSpeedValue(1.0f);

	// null�`�F�b�N
	if (m_PlayerStatePool[state] != nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[state];
	}

	// ������
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Init();
	}

	m_CurrentState = state;
}

void PlayerStateMachine::InitVelocity()
{
	m_Velocity = {};
}

void PlayerStateMachine::SetAnimationSpeedValue(const float& value)
{
	if (m_ParryCache == nullptr) return;

	m_PlayerCache->SetAnimationSpeedValue(value);
}

// �G�̍U�����v���C���[�Ə��ɓ����������ɔ���
bool PlayerStateMachine::CheckParry()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ATTACK_PARRY]) return false;

	if (m_ParryCache == nullptr) return false;
	
	if (m_ParryCache->CheckParryAccept())
	{
		if (!m_IsParry)
		{
			m_ParryCount++;
			m_IsParry = true;
		}
		if (m_ParryCount > EXTRATTACK_ACCEPT_PARRY_MAX)
		{
			m_ParryCount = EXTRATTACK_ACCEPT_PARRY_MAX;
		}
		if (m_ParryGageCache != nullptr)
		{
			m_ParryGageCache->ChangeUVScaling({(1.0f / EXTRATTACK_ACCEPT_PARRY_MAX) * m_ParryCount,1.0f });
		}
		return true;
	}
	return false;
}

// �G�̍U���������������ɔ���
bool PlayerStateMachine::CheckRolling()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ROLLING]) return false;

	if (m_RollingCache == nullptr) return false;

	return m_RollingCache->CheckRollingAccept();
}

// ���������������ɔ���
bool PlayerStateMachine::CheckAttack()
{
	if (m_CurrentPlayerState == m_PlayerStatePool[PLAYER_STATE::ATTACK_NORMAL])
	{
		if (m_NormalAttackCache == nullptr) return false;

		return m_NormalAttackCache->CheckAttackAccept();
	}

	if (m_CurrentPlayerState == m_PlayerStatePool[PLAYER_STATE::ATTACK_EXTR])
	{
		if (m_ExtrAttackCache == nullptr) return false;
		if (!m_ExtrAttackCache->GetAttackEnable()) return false;

		return m_ExtrAttackCache->CheckAttackAccept();
	}
	
	return false;
}

XMFLOAT3 PlayerStateMachine::GetCameraForward() const
{
	if (m_CameraCache != nullptr)
	{
		return m_CameraCache->GetForward();
	}
	return XMFLOAT3(0.0f,0.0f,0.0f);
}

XMFLOAT3 PlayerStateMachine::GetCameraRight() const
{
	if (m_CameraCache != nullptr)
	{
		return m_CameraCache->GetRight();
	}
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void PlayerStateMachine::HitGround()
{
	m_IsGround = true;
}

void PlayerStateMachine::UseExtrAttack()
{
	m_ParryCount = 0;
	if (m_ParryGageCache != nullptr)
	{
		m_ParryGageCache->ChangeUVScaling({ (1.0f / EXTRATTACK_ACCEPT_PARRY_MAX) * m_ParryCount,1.0f });
	}
}

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
#include "playerStateParryAttack.h"
#include "playerStateNormalAttack.h"
#include "playerState/playerStateRolling.h"
#include "playerState/playerStateExtrAttack.h"
#include "playerState/playerStateHitDamage.h"

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
		// 要素上限分リハッシュ
		m_PlayerStatePool.reserve(static_cast<int>(PLAYER_STATE::MAX) - 1);

		m_PlayerStatePool.emplace(PLAYER_STATE::IDLE, new PlayerStateIdle(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::RUN, new PlayerStateRun(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_PARRY, m_ParryCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_NORMAL, m_NormalAttackCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_EXTR, m_ExtrAttackCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ROLLING, m_RollingCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::HITDAMAGE, new PlayerStateHitDamage(this));
	}
	// 初期化
	for (const std::pair<PLAYER_STATE, PlayerState*>& PlayerState : m_PlayerStatePool)
	{
		m_PlayerStatePool[PlayerState.first]->Init();
	}

	if (m_CurrentPlayerState == nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[PLAYER_STATE::IDLE];
	}

	// テクスチャの生成
	if (m_ParryGageCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		// パリィゲージのバッググラウンド
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
		// nullチェック
		if (PlayerStatePool.second != nullptr)
		{
			PlayerStatePool.second->Unit();
		}
	}
}
// 主に移動の処理
void PlayerStateMachine::Update(const float& deltaTime)
{
	// 作成順番的に、Updateで保存する
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

	// 入力の前にリセット
	InputReset();

	// 攻撃
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
	// 回避
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsRollingButton = true;
	}
	// 移動
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

	//ジャンプ
	if (m_IsGround && InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsJamp = true;
	}

	// ステートの更新前、入力処理の後
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

	// TODO :死亡ステートを作成したら削除
	if (m_PlayerCache->GetHealth() <= 0.0f)
	{
		InputReset();
	}

	// 入力処理の後に
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
		m_CurrentPlayerState->ChangeStateControl();
	}

	// ステートの更新の後に呼ぶ
	if (m_PlayerCache != nullptr)
	{
		m_PlayerCache->SetVelocityX(m_Velocity.x);
		m_PlayerCache->SetVelocityZ(m_Velocity.z);

		// Yだけ+
		m_PlayerCache->SetVelocityY(m_PlayerCache->GetVelocity().y + m_Velocity.y);

		m_PlayerCache->SetRotationY(m_Rotation.y);

		if (m_AnimeBlendTimeValue != 0.0f)
		{
			m_PlayerCache->SetBlendTimeValue(m_AnimeBlendTimeValue);
		}
	}

	// 最後にリセット
	m_IsGround = false;
	m_IsHitAttacked = false;
}

void PlayerStateMachine::SetPlayerState(const PLAYER_STATE& state)
{
	// 同じステートだったらはじく
	if (m_CurrentPlayerState == m_PlayerStatePool[state]) return;

	// 終了処理
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Unit();
	}

	// アニメーション速度を、前のステートから引き継がないように
	SetAnimationSpeedValue(1.0f);

	// nullチェック
	if (m_PlayerStatePool[state] != nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[state];
	}

	// 初期化
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

// 敵の攻撃がプレイヤーと盾に当たった時に判定
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

// 敵の攻撃が当たった時に判定
bool PlayerStateMachine::CheckRolling()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ROLLING]) return false;

	if (m_RollingCache == nullptr) return false;

	return m_RollingCache->CheckRollingAccept();
}

// 剣が当たった時に判定
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

void PlayerStateMachine::UsedExtrAttack()
{
	m_ParryCount = 0;
	if (m_ParryGageCache != nullptr)
	{
		m_ParryGageCache->ChangeUVScaling({ (1.0f / EXTRATTACK_ACCEPT_PARRY_MAX) * m_ParryCount,1.0f });
	}
}

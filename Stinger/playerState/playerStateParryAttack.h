#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class Player;
// パリィ攻撃のステート / ダメージは無い
class PlayerStateParryAttack final :public PlayerState
{
private:
	float m_MinParryTime = 0.0f;
	float m_MaxParryTime = 0.0f;
	float m_SpeedAttenuateValue = 0.0f;		// 速度の減衰値
	bool m_ParryAccept = false;
	ObjectManager* m_ObjManagerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;

	bool CheckParryAccept();
};
#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class Player;
// �p���B�U���̃X�e�[�g / �_���[�W�͖���
class PlayerStateParryAttack final :public PlayerState
{
private:
	float m_MinParryTime = 0.0f;
	float m_MaxParryTime = 0.0f;
	float m_SpeedAttenuateValue = 0.0f;		// ���x�̌����l
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
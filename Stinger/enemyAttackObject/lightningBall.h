#pragma once
#include "enemyAttackObject.h"

// �O���錾
class LightningBallEffect;
class LightningBallCharge;
// �v���C���[�Ɍ������ėU�����闋�̒e
class LightningBall final: public EnemyAttackObject
{
private:
	LightningBallEffect* m_LightningBallEffCache = nullptr;			// ���̒e�̃r���{�[�h�G�t�F�N�g�Ǘ��p
	LightningBallCharge* m_LightningBallChargeCache = nullptr;		// ���̃`���[�W�̃p�[�e�B�N���Ǘ��p

	bool m_IsHoming = false;		// �z�[�~���O���̓v���C���[�̕����ɉ�]����
	float m_Speed = 0.0f;			// �e�̑��x
	virtual void AttackControl(const float& deltaTime)override;
	virtual bool CollisionDamageControl()override;
	virtual void Finish()override;
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	virtual void Attack()override;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& balletTime)override;
};
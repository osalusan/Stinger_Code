#pragma once
#include "enemyAttackObject.h"

// �O���錾
class LightningFallEffect;
class LightningCharge;
class MeshFiled;
// ����
class LightningFall final : public EnemyAttackObject
{
private:
	LightningFallEffect* m_LightningFallEffCache = nullptr;		// �����̃r���{�[�h�G�t�F�N�g�Ǘ��p
	LightningCharge* m_LightningChargeCache = nullptr;			// ���̃`���[�W�̃p�[�e�B�N���Ǘ��p
	MeshFiled* m_FiledCache = nullptr;							// �����̒��n�_��n�ʂɍ��킹���
	virtual void AttackControl(const float& deltaTime)override;
	virtual void Finish()override;
public:
	LightningFall() = delete;
	LightningFall(const GameObject* target);
	virtual void Attack()override;
	void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& baletTime);
};
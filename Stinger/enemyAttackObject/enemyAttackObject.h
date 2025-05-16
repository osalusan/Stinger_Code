#pragma once
#include "object/gameObject.h"

class BoxCollisionComponent;
// �G����������U���̊��N���X
class EnemyAttackObject : public GameObject
{
protected:
	const GameObject* m_TargetObject = nullptr;
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	bool m_IsAttack = false;
	float m_Damage = 0.0f;
	virtual void AttackControl(const float& deltaTime) = 0;			// �U�����L���ɂȂ��Ă��鎞�̏����������ōs��
	virtual bool CollisionDamageControl();							// �v���C���[�ɓ����������̃_���[�W�����������ōs��
	virtual void Finish() = 0;										// �U�����I�������ۂɁA�����Ő��l�����Z�b�g����
public:
	EnemyAttackObject() = delete;
	EnemyAttackObject(const GameObject* target);
	virtual void Update(const float& deltaTime)override final;

	// �^�X�N����U���𐶐����鎞�Ɏg�p
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& lifeTime);		// ���W�ƃ_���[�W�Ɛ������Ԃ�ݒ肵�Đ���

	// �������Ă���U���ɕύX��������Ƃ��Ɏg�p
	virtual void Attack() = 0;							// �����ƍU�������𕪂��čs���������p��
	virtual void KeepPos(const XMFLOAT3& shotPos);		// ���W���Œ�
};
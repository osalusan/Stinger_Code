#pragma once
#include "character/character.h"
#include <unordered_map>

enum class RANGE
{
	NONE = 0, // NONE�͂����ɂƂ��Ĉړ��ȊO�ł��邱�Ƃ��Ȃ�
	SHORT,
	MIDDLE,
	LONG
};

enum class ATTACK_PARTS
{
	NONE = 0,
	BODY,
	ARM,
	RIGHT_ARM,
	LEFT_ARM,
	RIGHT_LEG,
	LEFT_LEG,
	ALL,
	MAX
};

class BehaviorTree;
class BehaviorNode;
class Player;
class Polygon2D;

class BossEnemy :public Character {
protected:
	BehaviorTree* m_Tree = nullptr;
	std::vector<BoxCollisionComponent*> m_BoxCollisionCaches = {};
	std::unordered_map<std::string, ATTACK_PARTS> m_PartsCategory = {};
	Player* m_PlayerCache = nullptr;		// �_���[�W�p
	Polygon2D* m_EnemyHpCache = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_EnemySkillData = {};

	float m_StaminaValue = 0.0f;
	ATTACK_PARTS m_CurrentAttackParts = ATTACK_PARTS::NONE;	// ���ݗL���ȓ����蔻��̍U���ꏊ
	bool m_ParryRecoil = false;
	float m_AttackDamage = 0.0f;
	bool m_ParryPossibleAtk = false;
	bool m_AttackAccept = false;	// �Ӑ}���Ȃ��A���q�b�g�������Ȃ�悤��

	// �{�X�̃p�����[�^ / �q�N���X��Init�ŏ����ݒ�
	float m_MaxStamina = 0.0f;
	float m_ShortRange = 0.0f;
	float m_MiddleRange = 0.0f;
	float m_LongRange = 0.0f;
	float m_MinWaitTime = 0.0f;
	float m_MaxWaitTime = 0.0f;

	// �{�X�̌��݂̏��
	RANGE m_CurrentRange = RANGE::NONE;
	// �����s���̃m�[�h
	BehaviorNode* m_RunningNodeCache = nullptr;

	virtual void CollisionControl()override = 0;		// ���g�̑��̃I�u�W�F�N�g�̔��菈�����s��
	virtual void CustomCollisionInfo()override = 0;		// ���g�̃R���W�����̐ݒ���s��
	virtual void AnimationControl()override = 0;		// ���g�̃A�j���[�V�����̐ݒ���s��

	// �ړ��̏��Ńr�w�C�r�A�c���[�̐���
	virtual void MoveControl(const float& deltaTime)override final;
	virtual void ParameterControl(const float& deltaTime)override final;

	// �r�w�C�r�A�c���[���g�p����ꍇ�Ɏq�N���X�ŌĂ�
	void CreateTree();
	// CSV����G�l�~�[�̃f�[�^���擾
	void EnemyDataLoadCSV(const std::string& filePath);
public:
	BossEnemy() = delete;
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale, const XMFLOAT3& rot);
	virtual ~BossEnemy()override;
	virtual void TakeDamage(const float& atk)override;

	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	// ����������
	void RotToTarget(GameObject* obj, const float& deltaTime);
	// ��������
	void RotToTarget(GameObject* obj);
	
	bool UseStamina(const float& use);

	const BehaviorTree* GetBehaviourTree()const
	{
		return m_Tree;
	}	
	BehaviorNode* GetRunningNode()const
	{
		return m_RunningNodeCache;
	}
	const bool& GetParryRecoil()const
	{
		return m_ParryRecoil;
	}

	// �p�����[�^�n��Get
	const float& GetShortRange()const
	{
		return m_ShortRange;
	}
	const float& GetMiddelRange()const
	{
		return m_MiddleRange;
	}
	const float& GetLongRange()const
	{
		return m_LongRange;
	}
	const float& GetStamina()const
	{
		return m_StaminaValue;
	}
	const float& GetMaxStamina()const
	{
		return m_MaxStamina;
	}
	const float& GetMinWaitTime()const
	{
		return m_MinWaitTime;
	}
	const float& GetMaxWaitTime()const
	{
		return m_MaxWaitTime;
	}
	const ATTACK_PARTS& GetCurrentAttackParts()const
	{
		return m_CurrentAttackParts;
	}

	// ���݂̏�Ԃ�Get��Set
	void SetCurrentRange(const RANGE& range)
	{
		m_CurrentRange = range;
	}
	const RANGE& GetCurrentRange()const
	{
		return m_CurrentRange;
	}
	void SetRunningNode(BehaviorNode* node);

	void SetParryRecoil(const bool& parry)
	{
		m_ParryRecoil = parry;
	}
	void SetAttackParts(const ATTACK_PARTS& parts)
	{
		m_CurrentAttackParts = parts;
	}
	void SetAttackDamage(const float& attack)
	{
		m_AttackDamage = m_Attack * attack;
	}
	void SetParryPossibleAtk(const bool& possible)
	{
		m_ParryPossibleAtk = possible;
	}
	void SetAttackAccept(const bool& accept)
	{
		m_AttackAccept = accept;
	}

	const std::unordered_map<std::string, float>& GetSkillData(const std::string& skillName)const
	{
		if (m_EnemySkillData.count(skillName) >= 1)
		{
			return m_EnemySkillData.at(skillName);
		}
		// �e���|�����x���p
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};
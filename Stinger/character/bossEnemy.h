#pragma once
#include "character/character.h"
#include <unordered_map>
// �v���C���[�Ƃ̋����ɉ������͈� / �G�l�~�[���ƂɗL���������قȂ�
enum class RANGE
{
	NONE = 0, // NONE�͂����ɂƂ��Ĉړ��ȊO�ł��邱�Ƃ��Ȃ�
	SHORT,
	MIDDLE,
	LONG
};
// �U�����蕔��
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
// �{�X�G�l�~�[�p�̊��N���X
class BossEnemy :public Character {
protected:
	BehaviorTree* m_Tree = nullptr;
	std::vector<BoxCollisionComponent*> m_BoxCollisionCaches = {};
	std::unordered_map<std::string, ATTACK_PARTS> m_PartsCategory = {};
	Player* m_PlayerCache = nullptr;		// �_���[�W�p
	Polygon2D* m_EnemyHpCache = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_EnemySkillData = {};

	ATTACK_PARTS m_CurrentAttackParts = ATTACK_PARTS::NONE;		// ���ݗL���ȓ����蔻��̍U���ꏊ

	// �p���B�̃p�����[�^
	bool m_ParryRecoil = false;
	bool m_ParryPossibleAtk = false;

	// �U���̃p�����[�^
	float m_AttackDamage = 0.0f;								// �e�^�X�N�m�[�h����_���[�W���i�[���Ă��炤
	bool m_AttackAccept = false;								// �v���C���[�ɍU��������A�Ӑ}���Ȃ��A���q�b�g�������Ȃ�悤��

	// �{�X�̃p�����[�^
	float m_CurStaminaValue = 0.0f;								// ���݂̃X�^�~�i / �Z�̃o�����X�ׂ̈Ɏ���

	// �{�X�̃p�����[�^ / CSV����̓ǂݍ��݂Ő��l�𔽉f
	float m_MaxStamina = 0.0f;									// �X�^�~�i�̍ő�l
	float m_ShortRange = 0.0f;									// �ߋ����͈̔�
	float m_MiddleRange = 0.0f;									// �������͈̔�
	float m_LongRange = 0.0f;									// �������͈̔�
	float m_MinWaitTime = 0.0f;									// �ҋ@���̍ŏ��d������
	float m_MaxWaitTime = 0.0f;									// �ҋ@���̍ő�d������

	// �{�X�̌��݂̏��
	RANGE m_CurrentRange = RANGE::NONE;
	// �����s���̃m�[�h
	BehaviorNode* m_RunningNodeCache = nullptr;

	virtual void CollisionControl()override = 0;		
	virtual void CustomCollisionInfo()override = 0;		
	virtual void AnimationControl()override = 0;		

	// �ړ��̏��Ńr�w�C�r�A�c���[�̍X�V���s��
	virtual void MoveControl(const float& deltaTime)override final;
	// �{�X�̕ϓ��p�����[�^���Ǘ�
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

	// ���x�͒ǉ������ł���悤��
	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	// �^�[�Q�b�g�̕��ւ���������
	void RotToTarget(GameObject* obj, const float& deltaTime);
	// �^�[�Q�b�g�̕��֑����Ɍ���
	void RotToTarget(GameObject* obj);
	// �X�^�~�i�������ꍇ�Ɏg�p
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
		return m_CurStaminaValue;
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
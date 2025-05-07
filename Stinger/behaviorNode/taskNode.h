#pragma once
#include "behaviorNode.h"
#include <unordered_map>

// �O���錾
enum class ATTACK_PARTS;
class BossEnemy;
class Player;

class TaskNode : public BehaviorNode
{
private:
	float FindSkillData(const std::string& name);
	std::unordered_map<std::string, float> m_skillData = {};
protected:
	BossEnemy* m_BossCache = nullptr;
	Player* m_PlayerCache = nullptr;

	float m_CurrentTime = 0.0f;
	float m_CurrentAnimSpeedTime = 0.0f;
	float m_MaxAnimTime = 0.0f;

	bool m_UseDerivation = false;					// �h���Z�̏����ݒ��������
	bool m_EnableDerivation = false;				// �h���Z���g���邩
	int m_UseDerivNumber = 0;                       // �g�p����h���Z�̔ԍ�
	bool m_AttackTask = false;						// �U���Ɏg�p���Ă���^�X�N

	// �p�����[�^
	float m_DamageValue = 0.0f;						// �_���[�W�{��
	float m_SpeedValue = 0.0f;						// ���x�{��
	float m_AttackEnableTimeValue = 0.0f;			// �U������J�n����
	float m_AttackDisableTimeValue = 1.0f;			// �U������I������
	float m_BulletSpeed = 0.0f;						// �e��
	float m_AnimSpeedValue = 1.0f;					// �A�j���[�V�������x�{��
	float m_AnimChangeSpeedTime = 1.0f;				// �A�j���[�V�������x�{��
	float m_BulletActionTime = 0.0f;				// ���ˎ���
	bool m_ParryPossibleAtk = false;				// �p���B�����U���̏ꍇ�́ACSV�̕���0�ȊO�̐��l����͂���

	// �A�j���[�V�������g�p����ۂ́A�K���Ă�
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// �Z�̏ꍇ�͕K���Ă�
	void InitSkillData(const std::string& skillName);
	NODE_STATE UpdateChildren(const float& deltaTime);
	// �U�����g�p����ꍇ��Update�Ŏg�p
	void UseAttack(const ATTACK_PARTS& parts);

	// �G���[���� -1 ��Ԃ� / �������� m_UseDerivNumber �֊i�[
	int DerivationChance();
	// 
	NODE_STATE UpdateUseDerivationTask(const float& deltaTime);

protected:
	virtual void InitTask(const float& deltaTime);
	virtual void RunningTask(const float& deltaTime);

public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATE Update(const float& deltaTime) override;

	const float& GetMaxAnimTime()const
	{
		return m_MaxAnimTime;
	}

	// �p���B��_�E���ŋ����I��Task��ύX���鎞�Ɏg�p
	void CancelRunningTask();
};
#pragma once
#include "behaviorNode/taskNode.h"

// �O���錾
class LightningFall;
// ---------------------------------------------------------
// �^�X�N�̐����F�̂̎��͂ɋK���I�ȗ���3��ɕ����ĕ���
// ���l�F���̐����^�C�~���O�␔��cpp�ŊǗ�
// ---------------------------------------------------------
class LightningBarstTask final : public TaskNode
{
private:
	std::vector<LightningFall*> m_LightningFallCache = {};
	int m_UseLingtning = 0;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	~LightningBarstTask()override;
	virtual void Init()override;
};
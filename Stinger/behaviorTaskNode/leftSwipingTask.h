#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�����U�肩�Ԃ��čU��
// ���l�F�U������͍����`����̍��S��
// ---------------------------------------------------------
class LeftSwipingTask final : public TaskNode
{
private:
	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};
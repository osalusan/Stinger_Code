#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�E���U�肩�Ԃ��čU��
// ���l�F�U������͉E���`�E��̉E�S��
// ---------------------------------------------------------
class RightSwipingTask final : public TaskNode
{
private:
	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;

public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};
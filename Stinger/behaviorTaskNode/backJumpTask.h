#pragma once
#include "behaviorNode/taskNode.h"

// ---------------------------------------------------------
// �^�X�N�̐����F�W�����v���Ȃ�����ɉ�����
// ���l�F
// ---------------------------------------------------------
class BackJumpTask final : public TaskNode
{
private:
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};
#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�E�t�b�N
// ���l�F�E���`�E��܂ł̉E�����U������
// ---------------------------------------------------------
class RightPunchTask final : public TaskNode
{
private:
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};

#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F���S�^�X�N
// ���l�F�A�j���[�V�����̊Ǘ���X���[�Ȃǂ̉��o���Ǘ�
// ---------------------------------------------------------
class DeadTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
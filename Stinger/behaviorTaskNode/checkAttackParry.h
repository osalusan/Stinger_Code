#pragma once
#include "behaviorNode/taskNode.h"

// ---------------------------------------------------------
// �^�X�N�̐����F�p���B���ꂽ���m�F����ׂ����̃^�X�N
// ���l�F�p���B���m�F��������s���̃^�X�N�������I��������
// ---------------------------------------------------------
class CheckAttackParryTask final : public TaskNode
{
private:
	bool m_RunningChild = false;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
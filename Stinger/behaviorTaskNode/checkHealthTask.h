#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�G�̗̑͂𔻕ʂ���
// ���l�F�̗͂��ǂꂭ�炢�����f���A�����Ă��邩����ł��邩�𔻕�
// ---------------------------------------------------------
class CheckHealthTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
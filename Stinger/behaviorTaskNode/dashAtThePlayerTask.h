#pragma once
#include "behaviorNode/taskNode.h"
// �v���C���[��_���ă_�b�V��

class DashAtThePlayerTask final: public TaskNode
{
private:
	float m_Range = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

	void SetRange(const float& range)
	{
		m_Range = range;
	}
};
#pragma once
#include "behaviorNode/taskNode.h"

class ParryRecoilTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};

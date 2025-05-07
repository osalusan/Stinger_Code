#pragma once
#include "behaviorNode/taskNode.h"

// �O���錾
class Camera;

class CheckRenditionTask final : public TaskNode
{
private:
	int m_TaskCount = 0;
	bool m_AcceptRendition = false;
	Camera* m_CameraCache = nullptr;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};

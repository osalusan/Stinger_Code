#pragma once
#include "behaviorNode/taskNode.h"

class Camera;
// ---------------------------------------------------------
// �^�X�N�̐����F�ǂݍ��ݎ��Ɏg���A�ҋ@�p�A�j���[�V����
// ���l�F
// ---------------------------------------------------------
class LoadIdleTask final : public TaskNode
{
private:
	Camera* m_CameraCache = nullptr;

public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
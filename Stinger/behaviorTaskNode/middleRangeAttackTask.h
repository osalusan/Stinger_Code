#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�������Ŏg�p�������U���^�X�N���Ǘ�
// ���l�F�h��������h���m�����g�p���邽�߂ɍ쐬
// ---------------------------------------------------------
class MiddleRangeAttackTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
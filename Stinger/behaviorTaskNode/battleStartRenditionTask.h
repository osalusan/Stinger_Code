#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

// �O���錾
class Polygon2D;
// ---------------------------------------------------------
// �^�X�N�̐����F�o�g���X�^�[�g���o
// ���l�F�L�����N�^�[�̃A�j���[�V������e�N�X�`�����o�����s
// ---------------------------------------------------------
class BattleStartRenditionTask final : public TaskNode
{
private:
	Polygon2D* m_BattleStartTextCache = nullptr;
	XMFLOAT2 m_BSTextUV = { 0.0f,1.0f };
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};
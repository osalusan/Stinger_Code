#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�v���C���[�Ɍ������đ���
// ���l�FSUCCESS�̐ݒ肪�����̂ŁA�ŏ��ɒu���Ɩ����ɕ���������
// ---------------------------------------------------------
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
#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// �^�X�N�̐����F�G���v���C���[����ǂꂭ�炢����Ă��邩�𔻒�
// ���l�F������Ă΂Ȃ��ƁA�����ɉ����ĕω�����U�����g���Ȃ�
// ---------------------------------------------------------
class CheckRangeTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};
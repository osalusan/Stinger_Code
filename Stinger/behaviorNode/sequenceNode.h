#pragma once
#include "taskNode.h"

// ���ׂĂ̎q�m�[�h�����������ꍇ�ɐ���
class SequenceNode: public BehaviorNode 
{
public:
    virtual void Init()override;
    virtual NODE_STATE Update(const float& deltaTime) override;
};
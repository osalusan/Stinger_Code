#pragma once
#include "taskNode.h"

// すべての子ノードが成功した場合に成功
class SequenceNode: public BehaviorNode 
{
public:
    virtual void Init()override;
    virtual NODE_STATE Update(const float& deltaTime) override;
};
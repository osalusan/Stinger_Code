#pragma once
#include "taskNode.h"

// 最初に成功したノードで終了
class SelectorNode: public BehaviorNode 
{
protected:
    
public:
    virtual void Init()override;
    virtual NODE_STATE Update(const float& deltaTime) override;
};
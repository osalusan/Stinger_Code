#pragma once
#include "behaviorTree.h"
// ���[�h��ʂŎg�p����G�l�~�[�̓��]
class MawJLaygoLoadTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};
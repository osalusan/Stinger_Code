#pragma once
#include "behaviorTree.h"
// �o�g���Ŏg�p����G�l�~�[�̓��]
class MawJLaygoBattleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};
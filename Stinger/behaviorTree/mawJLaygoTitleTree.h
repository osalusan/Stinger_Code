#pragma once
#include "behaviorTree.h"
// �^�C�g����ʂŎg�p����G�l�~�[�̓��]
class MawJLaygoTitleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};
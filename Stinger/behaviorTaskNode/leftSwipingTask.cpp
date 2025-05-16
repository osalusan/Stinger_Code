#include "leftSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void LeftSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\leftSwiping_MawJ.fbx", "leftSwiping");
	m_TaskName = "������";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
	m_AttackTask = true;
}

void LeftSwipingTask::InitTask(const float& deltaTime)
{
	TaskNode::InitTask(deltaTime);
	m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
}

void LeftSwipingTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);
	// �_���[�W����
	UseAttack(ATTACK_PARTS::LEFT_ARM);
}
#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorNode/sequenceNode.h"
#include "behaviortaskNode/dashAtThePlayerTask.h"
#include "behaviorTaskNode/leftSwipingTask.h"
#include "behaviorTaskNode/rightSwipingTask.h"
#include "behaviortaskNode/checkRangeTask.h"
#include "behaviortaskNode/battleStartRenditionTask.h"
#include "behaviorTaskNode/checkHealthTask.h"
#include "behaviorTaskNode/deadTask.h"
#include "behaviorTaskNode/jumpAttackTask.h"
#include "behaviorTaskNode/waitTask.h"
#include "behaviorTaskNode/checkAttackParry.h"
#include "behaviorTaskNode/parryRecoilTask.h"
#include "behaviorTaskNode/airLightningBallTask.h"
#include "behaviorTaskNode/rightPunchTask.h"
#include "behaviorTaskNode/shortRangeAttackTask.h"
#include "behaviorTaskNode/backJumpTask.h"
#include "behaviorTaskNode/lightningBarstTask.h"
#include "behaviorTaskNode/middleRangeAttackTask.h"
#include "behaviorTaskNode/lightningFallFowardRainTask.h"
#include "behaviorTaskNode/checkRenditionTask.h"

void MawJLaygoBattleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	Player* player = objectManager->GetPlayer();
	if (player == nullptr) return;

	// -------------------------- �r�w�C�r�A�c���[�̍쐬 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	BehaviorNode* renditionTask = rootNode->AddTaskChild<CheckRenditionTask>(boss, player);
	if (renditionTask == nullptr) return;

	// ���o�p�̙��K
	renditionTask->AddTaskChild<BattleStartRenditionTask>(boss, player);

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("�̗͊Ǘ��V�[�P���X");
	if (healthSeqNode == nullptr) return;

	// ���S
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* parryRecoilNode = rootNode->AddNodeChild<SequenceNode>("�p���B�Ǘ��V�[�P���X");
	if (parryRecoilNode == nullptr) return;

	// �p���B�̊m�F
	BehaviorNode* ceckParry = parryRecoilNode->AddTaskChild<CheckAttackParryTask>(boss, player);
	if (ceckParry == nullptr) return;
	// �p���B���ꂽ
	ceckParry->AddTaskChild<ParryRecoilTask>(boss, player);


	// ���f�o�b�O�p��
	
	// rootNode->AddTaskChild<AirLightningBallTask>(boss, player);

	// ���f�o�b�O�p��


	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("�U���Ǘ��Z���N�^�[");
	if (attackSelNode == nullptr) return;

	// �ҋ@
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// �͈͂̊m�F
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// �U��

	// �ߋ���
	if (BehaviorNode* shortAttackTask = attackSelNode->AddTaskChild<ShortRangeAttackTask>(boss, player))
	{
		shortAttackTask->AddTaskChild<RightPunchTask>(25, boss, player);
		DERIVATION_DATA lightningBarstDerivData = { 0.6f,100,0.0f };
		shortAttackTask->AddTaskChild<LightningBarstTask>(lightningBarstDerivData, 5, boss, player);
		// �o�b�N�W�����v�h��
		if (BehaviorNode* backJump = shortAttackTask->AddTaskChild<BackJumpTask>(25, boss, player))
		{
			DERIVATION_DATA backJumpTo{ 0.6f,100,0.9f };
			backJump->AddTaskChild<LightningFallFowardRainTask>(backJumpTo,60, boss, player);
			backJump->AddTaskChild<JumpAttackTask>(backJumpTo,40, boss, player);
		}

		// ���U�肩�����h��
		if (BehaviorNode* leftSwiping = shortAttackTask->AddTaskChild<LeftSwipingTask>(45, boss, player))
		{
			DERIVATION_DATA derivToRightSwiping = { 0.7f,100,0.7f };
			DERIVATION_DATA derivToRightSwipingQuick = { 0.5f,100,0.35f };
			DERIVATION_DATA derivToBackJump = { 0.6f,100,0.4f };
			DERIVATION_DATA derivToLightningBarst = { 0.5f,80,0.4f };

			leftSwiping->AddTaskChild<RightSwipingTask>(derivToRightSwiping, 20, boss, player);
			leftSwiping->AddTaskChild<RightSwipingTask>(derivToRightSwipingQuick, 20, boss, player);
			leftSwiping->AddTaskChild<LightningBarstTask>(derivToLightningBarst, 15, boss, player);
			if (BehaviorNode* backJump = leftSwiping->AddTaskChild<BackJumpTask>(derivToBackJump, 45, boss, player))
			{
				DERIVATION_DATA lightningBDerivData = { 0.5f, 50 ,0.85f };
				backJump->AddTaskChild<LightningFallFowardRainTask>(lightningBDerivData, boss, player);
			}
		}
	}


	// ������
	if (BehaviorNode* middleAttackTask = attackSelNode->AddTaskChild<MiddleRangeAttackTask>(boss, player))
	{
		// �W�����v�U���h��
		if (BehaviorNode* jumpAttack = middleAttackTask->AddTaskChild<JumpAttackTask>(75,boss, player))
		{
			const int& jumpAChance = 70;
			DERIVATION_DATA lightningBallDerivData = { 0.75f,jumpAChance,0.3f };
			jumpAttack->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 50, boss, player);
			DERIVATION_DATA lightningBarstDerivData1 = { 0.7f,jumpAChance,0.6f };
			jumpAttack->AddTaskChild<LightningBarstTask>(lightningBarstDerivData1, 40, boss, player);
			// ��̃��C�g�j���O�o�[�X�g��蔭������������
			DERIVATION_DATA lightningBarstDerivData2 = { 0.5f,jumpAChance,0.45f };
			jumpAttack->AddTaskChild<LightningBarstTask>(lightningBarstDerivData2, 10, boss, player);
		}

		// �A������ / 2�i��,3�i�ڃL�����Z��
		if (BehaviorNode* lb1 = middleAttackTask->AddTaskChild<AirLightningBallTask>(20,boss, player))
		{
			DERIVATION_DATA lightningBallDerivData = { 0.83f,100,1.0f };
			// �Q����
			if (BehaviorNode* lb2 = lb1->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, boss, player))
			{
				DERIVATION_DATA jumpAttackDerivData = { 0.68f,100,0.2f };
				lb2->AddTaskChild<JumpAttackTask>(jumpAttackDerivData, 25, boss, player);

				// �R����
				if (BehaviorNode* lb3 = lb2->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 75, boss, player))
				{
					lb3->AddTaskChild<JumpAttackTask>(jumpAttackDerivData, boss, player);
				}		
			}
		}
		// ���O���U��
		middleAttackTask->AddTaskChild<LightningFallFowardRainTask>(5,boss, player);
	}

	// �U���s��
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// ��ԍŌ��
	CreateRoot(rootNode);
}

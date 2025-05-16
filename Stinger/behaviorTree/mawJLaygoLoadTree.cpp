#include "mawJLaygoLoadTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/loadScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorTaskNode/loadIdleTask.h"

void MawJLaygoLoadTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;

	// -------------------------- �r�w�C�r�A�c���[�̍쐬 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	rootNode->AddTaskChild<LoadIdleTask>(boss, nullptr);

	// ��ԍŌ��
	CreateRoot(rootNode);
}

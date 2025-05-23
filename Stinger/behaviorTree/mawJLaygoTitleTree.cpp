#include "mawJLaygoTitleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/titleScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorTaskNode/roaringInfinityTask.h"

void MawJLaygoTitleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;

	// -------------------------- ビヘイビアツリーの作成 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	rootNode->AddTaskChild<RoaringInfinityTask>(boss, nullptr);

	// 一番最後に
	CreateRoot(rootNode);
}

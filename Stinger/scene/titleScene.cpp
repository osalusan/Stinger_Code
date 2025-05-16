#include "titleScene.h"
#include "main/main.h"
#include "manager/objectManager.h"
#include "manager/sceneManager.h"
#include "manager/inputManager.h"
#include "manager/textureManager.h"
#include "manager/fbxModelManager.h"
#include "manager/objModelManager.h"
#include "manager/audioManager.h"
#include "polygon2D/polygon2D.h"
#include "scene/gameScene.h"
#include "camera/titleCamera.h"
#include "skydome/skydome.h"
#include "particle/spiralBlueorb.h"
#include "meshFiled/meshFiled.h"
#include "shadow/shadowVolume.h"
#include "behaviorTree/mawJLaygoTitleTree.h"
#include "character/mawJLaygo.h"
#include "staticMeshObject/box.h"
#include "staticMeshObject/staticMeshObject.h"
#include "staticMeshObject/moveRandSizeBox.h"

constexpr float DEFAULT_POSZ = 60.0f;
constexpr float NEXT_SCENE_TIME = 3.0f;
void TitleScene::Init()
{
	Scene::Init();
	AudioManager::ReservAudio(AUDIO::TITLE_BGM, "asset\\audio\\bgm\\arrival.wav", true, true);
	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->AddGameObject<TitleCamera>(OBJECT::CAMERA_MAIN);
	m_ObjectManager->AddGameObjectArg<MeshFiled>(OBJECT::FILED, XMFLOAT3(0.0f, -2.0f, 40.0f), TEXTURE::GROUND_TIGGER_ROCK_TEX, L"asset\\texture\\tiger_rock_diff_2k.dds", TEXTURE::GROUND_TIGGER_ROCK_NOR, L"asset\\texture\\tiger_rock_nor_gl_2k.dds");
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), PIVOT::CENTER, TEXTURE::TITLE, L"asset\\texture\\stinger.png");	
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9f), XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.15f), PIVOT::CENTER, TEXTURE::PUSHSPACEKEY, L"asset\\texture\\pushSpaceKey.png");
	m_ObjectManager->AddGameObjectArg<SkyDome>(OBJECT::SKYDOME, STATICMESH_MODEL::SKYDOME_TITLE, "asset\\model\\sky\\titleSky.obj");
	
	Camera* camera = m_ObjectManager->GetCamera();
	float cameraPosZ = camera->GetPos().z;
	for (int i = 0; i < 20; i++)
	{
		m_ObjectManager->AddGameObjectArg<MoveRandSizeBox>(OBJECT::STATICMESH, cameraPosZ, DEFAULT_POSZ);
	}

	m_ObjectManager->AddGameObjectArg<SpiralBlueOrb>(OBJECT::BILLBOARD,XMFLOAT3(0.0f,4.0f, DEFAULT_POSZ),false);

	CreateShadowVolume();
}

void TitleScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_NextScene = true;
		std::vector<GameObject*> objects = {};

		if (m_ObjectManager == nullptr) return;

		m_ObjectManager->GetGameObjects(objects, OBJECT::POLYGON2D);

		for (GameObject* obj : objects)
		{
			if (Polygon2D* poly = dynamic_cast<Polygon2D*>(obj))
			{
				poly->SetEnable(false);
			}
		}
		
		std::vector<GameObject*> particles = {};
		m_ObjectManager->GetGameObjects(particles,OBJECT::BILLBOARD);

		for (GameObject* particle : particles)
		{
			SpiralBlueOrb* spb = dynamic_cast<SpiralBlueOrb*>(particle);
			spb->SetEnable(true);
		}
	}
	if (m_NextScene)
	{
		if (m_NextSceneTime > NEXT_SCENE_TIME)
		{
			SceneManager::SetScene<GameScene>();
		}
		m_NextSceneTime += deltaTime;
	}
}

void TitleScene::CreateShadowVolume()
{
	if (m_ObjectManager == nullptr) return;

	//m_ObjectManager->AddGameObjectArg<ShadowVolume>(OBJECT::SHADOW, m_ObjectManager->GetBossEnemy());

	std::vector<GameObject*> staticMeshObj = {};
	m_ObjectManager->GetGameObjects(staticMeshObj,OBJECT::STATICMESH);

	for (GameObject* obj : staticMeshObj)
	{
		if (StaticMeshObject* staticMesh = dynamic_cast<StaticMeshObject*>(obj))
		{
			m_ObjectManager->AddGameObjectArg<ShadowVolume>(OBJECT::SHADOW, staticMesh);
		}
	}
}


#include "character.h"
#include "component/collisionComponent.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "renderer/fbxModelRenderer.h"
#include "component/collisionComponent.h"
#include "component/shaderComponent.h"
#include "scene/scene.h"

// ------------------------- protected -------------------------

void Character::ParameterControl(const float& deltaTime)
{
	// �ߋ����W�̕ۑ�
	m_RecordPosition = m_Position;

	if (m_Health <= 0)
	{
		if (m_FinishPerformance)
		{
			m_Enable = false;
		}
	}
}

void Character::ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path)
{
	FbxModelManager::ReservModel(animeModel, path);
	m_Model = animeModel;
}

void Character::CheckWorldWallPos()
{
	if (m_Position.x > m_WorldWall.x)
	{
		m_Position.x = m_WorldWall.x;
	}
	else if (m_Position.x < -m_WorldWall.x)
	{
		m_Position.x = -m_WorldWall.x;
	}
	if (m_Position.y > m_WorldWall.y)
	{
		m_Position.y = m_WorldWall.y;
	}
	else if (-m_Position.y < -m_WorldWall.y)
	{
		m_Position.x = 10.0f;
	}
	if (m_Position.z > m_WorldWall.z)
	{
		m_Position.z = m_WorldWall.z;
	}
	else if (m_Position.z < -m_WorldWall.z)
	{
		m_Position.z = -m_WorldWall.z;
	}
}

// ------------------------- public -------------------------
Character::Character()
{
	m_Model = ANIMETION_MODEL::MAX;
	m_BlendTimeValue = DEFAULT_BLEND_VALUE;
}

void Character::Init()
{
	GameObject::Init();

	if (m_MeshFiled != nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	MeshFiled* filed = objectManager->GetMeshFiled();

	m_MeshFiled = filed;
	m_WorldWall = scene->GetWorldWall();
}

void Character::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	m_Velocity.x = 0.0f;
	m_Velocity.z = 0.0f;

	if (m_EnableGravity)
	{
		m_CurrentGravityValue += deltaTime;
		m_Velocity.y -= m_GravityValue * m_CurrentGravityValue;
	}
	else
	{
		m_Velocity.y = 0.0f;
	}

	// �ړ�����
	MoveControl(deltaTime);
	// �A�j���[�V��������
	AnimationControl();

	// �A�j���[�V�����̃u�����h����
	m_AnimationTime += deltaTime * m_AnimSpeedValue;
	if (m_AnimationName != m_NextAnimationName && m_BlendRatio < 1.0f) 
	{ 
		m_BlendRatio += deltaTime * m_BlendTimeValue;
	}
	else if(m_AnimationName != m_NextAnimationName && m_BlendRatio > 1.0f)
	{
		m_AnimationName = m_NextAnimationName;
		m_AnimationTime = m_NextAnimationTime;
		m_NextAnimationTime = 0.0f;
		m_BlendRatio = 0.0f;
	}
	if (m_AnimationName == m_NextAnimationName && m_BlendRatio > 0.0f)
	{ 
		m_BlendRatio -= deltaTime * m_BlendTimeValue;
	}
	else if (m_AnimationName == m_NextAnimationName && m_BlendRatio < 0.0f)
	{
		m_BlendRatio = 0.0f;
	}

	// ���x����
	m_Velocity.x *= deltaTime;
	m_Velocity.y *= deltaTime;
	m_Velocity.z *= deltaTime;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// �����蔻��p�ϐ��̐ݒ� / �ړ���ɌĂ�
	CustomCollisionInfo();
	// �����蔻�菈��
	CollisionControl();
	// �����蔻��p�ϐ��̐ݒ� / �����蔻���ɌĂ�
	CustomCollisionInfo();
	// �p�����[�^�̐��� / �ړ��A�����蔻��̌�ɌĂ�
	ParameterControl(deltaTime);
}

void Character::Draw()
{
	GameObject::Draw();

	if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		if (m_AnimationName == m_NextAnimationName)
		{
			model->Update(m_AnimationName.c_str(), m_AnimationTime);
		}
		else if(m_AnimationName != m_NextAnimationName)
		{
			model->Update(m_AnimationName.c_str(), m_AnimationTime, m_NextAnimationName.c_str(), m_NextAnimationTime, m_BlendRatio);
		}
		
		model->Draw();
	}
}

void Character::InitGravity()
{
	m_CurrentGravityValue = 0.0f;
}

void Character::IsDead()
{
	if (m_Health <= 0.0f)
	{
		m_IsDead = true;
	}
}

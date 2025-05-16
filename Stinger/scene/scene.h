#pragma once
#include "main/main.h"

class ObjectManager;
// �V�[���̊���N���X
class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;		// �S�ẴI�u�W�F�N�g���Ǘ����Ă���N���X
	XMFLOAT3 m_WorldWall = {};						// �ő�ړ��͈͂̊i�[�p

public:
	virtual ~Scene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	ObjectManager* GetObjectManager()
	{
		return m_ObjectManager;
	}
	const XMFLOAT3& GetWorldWall()const
	{
		return m_WorldWall;
	}
};
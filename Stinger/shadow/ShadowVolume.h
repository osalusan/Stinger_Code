#pragma once
#include "object/gameObject.h"

// �O���錾
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;

class StaticMeshObject;
class Character;
class MeshFiled;
class ShadowVolume final:public GameObject
{
protected:
	// ���f���{�� / �R���X�g���N�^�ŏ�����
	STATICMESH_MODEL m_StaticModel;						
	ANIMETION_MODEL m_AnimeModel;						

	StaticMeshObject* m_StaticMeshCache = nullptr;
	Character* m_CharacterCache = nullptr;

	MeshFiled* m_MeshFiled = nullptr;
public:
	ShadowVolume() = delete;
	ShadowVolume(StaticMeshObject* staticMesh);
	ShadowVolume(Character* character);
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};
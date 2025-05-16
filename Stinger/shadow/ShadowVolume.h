#pragma once
#include "object/gameObject.h"

// �O���錾
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;

class StaticMeshObject;
class Character;
class MeshFiled;
// �X�e���V���V���h�E���g���N���X
class ShadowVolume final:public GameObject
{
protected:
	// ���f���{�� / �R���X�g���N�^�ŏ�����
	STATICMESH_MODEL m_StaticModel;						
	ANIMETION_MODEL m_AnimeModel;						

	StaticMeshObject* m_StaticMeshCache = nullptr;		// �e�Ƃ��Ďg�p
	Character* m_CharacterCache = nullptr;				// ���f���̃A�j���[�V�����𔽉f������悤�� / �X�e���V���V���h�E���Ə�肭�e��\���ł��Ȃ��̂ŁA�Ⴄ�e������������������

	MeshFiled* m_MeshFiled = nullptr;					// �e��n�ʂ̍����ɕ`��
public:
	ShadowVolume() = delete;
	ShadowVolume(StaticMeshObject* staticMesh);			// �e�Ƃ��ĕ`�悵����StaticMesh��������
	ShadowVolume(Character* character);					// �e�Ƃ��ĕ`�悵����Character��������
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};
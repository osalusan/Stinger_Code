#pragma once
#include "object/gameObject.h"

// �O���錾
enum class STATICMESH_MODEL;
class ObjModelRenderer;
// OBJ��ǂݍ���ŕ`�悷����N���X
class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;								// ���f���{�� / �R���X�g���N�^�ŏ�����
	BoxCollisionComponent* m_BoxCollCache = nullptr;		// �l�p�̓����蔻��p / TODO :�X�t�B�A�̓����蔻����쐬������2�ڂ�ǉ�
	// �����ݒ肵�����l��ۑ����Ă����p
	XMFLOAT3 m_OffsetCollPos = {};
	XMFLOAT3 m_OffsetCollScl = {};

	XMFLOAT4 m_Color = {1.0f,1.0f,1.0f,1.0f};
	XMMATRIX m_RotScaleMatrix = {};							//  �}�g���N�X�̉�]�Ƃ��ĕ`�揈���ⓖ���蔻��ɓ`����ꍇ�Ɏg�p

	bool m_Visible = true;		// �����Ȃ��Ȃ��Ă������蔻��͎c��

	virtual void MoveControl(const float& deltaTime) = 0;
public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);		// StaticMesh���Ŏ�ޕ�������ׂ�
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const ObjModelRenderer* GetModel()const;

	const STATICMESH_MODEL& GetStaticModel()const
	{
		return m_Model;
	}

	const XMMATRIX& GetRotationMatrixStaticMesh()const
	{
		return m_RotScaleMatrix;
	}

	void SetOffsetCollPos(const XMFLOAT3& pos)
	{
		m_OffsetCollPos = pos;
	}
	void SetOffsetCollScl(const XMFLOAT3& scl)
	{
		m_OffsetCollScl = scl;
	}
};
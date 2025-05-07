#pragma once
#include "staticMeshObject/staticmeshObject.h"

// �O���錾
enum class ANIMETION_MODEL;
class ShaderComponent;

class EquipmentObject final: public StaticMeshObject
{
private:
	const GameObject* m_ParentObject = nullptr;
	ShaderComponent* m_ShaderCache = nullptr;
	ANIMETION_MODEL m_ParentModel;				// �R���X�g���N�^�ŏ����ݒ�
	const char* m_BoneName = "";
	XMFLOAT3 m_OffsetScale = {};
	XMFLOAT3 m_OffsetRot = {};
	XMFLOAT3 m_OffsetPos = {};

	virtual void MoveControl(const float& deltaTime)override;
public:
	~EquipmentObject();
	EquipmentObject() = delete;
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale);
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale, const XMFLOAT3& rot);
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale, const XMFLOAT3& rot, const XMFLOAT3& pos);
};
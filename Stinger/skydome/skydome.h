#pragma once
#include "staticMeshObject/staticMeshObject.h"
#include <memory>

// �O���錾
class Camera;
// ��̕`�������N���X
class SkyDome final : public StaticMeshObject
{
private:
	Camera* m_CameraCache = nullptr;	// �h�[�����J�����ɂ��čs���悤��

	virtual void MoveControl(const float& deltaTime)override;
public:
	SkyDome() = delete;
	SkyDome(const STATICMESH_MODEL& model,std::string filePath);
	virtual ~SkyDome();
	virtual void Init()override;
};
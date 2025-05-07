#pragma once
#include "staticMeshObject/staticmeshObject.h"
#include <memory>

// �O���錾
class Camera;

class SkyDome final : public StaticMeshObject
{
private:
	Camera* m_CameraCache = nullptr;

	virtual void MoveControl(const float& deltaTime)override;
public:
	SkyDome() = delete;
	SkyDome(const STATICMESH_MODEL& model,std::string filePath);
	virtual ~SkyDome();
	virtual void Init()override;
};
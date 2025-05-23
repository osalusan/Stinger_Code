#pragma once
#include "staticMeshObject.h"
// ステージに配置用のオブジェクト
class Ruins final: public StaticMeshObject
{
private:
	virtual void MoveControl(const float& deltaTime)override;
public:
	Ruins() = delete;
	Ruins(const XMFLOAT3& position, const XMFLOAT3& scale, const XMFLOAT3& rotate);
};
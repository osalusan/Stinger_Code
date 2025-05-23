#pragma once
#include "staticMeshObject.h"
// 正方形の動かない箱
class Box : public StaticMeshObject 
{
private:
	virtual void MoveControl(const float& deltaTime)override;
public:
	Box();
	Box(const XMFLOAT3& position, const XMFLOAT3& scale);
	Box(const XMFLOAT3& position,const XMFLOAT3& scale,const XMFLOAT3& rotate);
};
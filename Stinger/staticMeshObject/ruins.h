#pragma once
#include "staticMeshObject.h"
// �X�e�[�W�ɔz�u�p�̃I�u�W�F�N�g
class Ruins final: public StaticMeshObject
{
private:
	virtual void MoveControl(const float& deltaTime)override;
public:
	Ruins() = delete;
	Ruins(const XMFLOAT3& position, const XMFLOAT3& scale, const XMFLOAT3& rotate);
};
#pragma once
#include "scene/scene.h"
// �f�o�b�O�Ŏg�p���������̃V�[��
class DebugScene final :public Scene
{

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};
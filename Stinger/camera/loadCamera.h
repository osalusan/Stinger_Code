#pragma once
#include "camera.h"
// ���[�h��ʂŎg�p����J����
class LoadCamera final :public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};
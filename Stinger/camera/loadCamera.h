#pragma once
#include "camera.h"

class LoadCamera final :public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};
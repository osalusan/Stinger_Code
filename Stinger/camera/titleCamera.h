#pragma once
#include "camera.h"
// �^�C�g����ʂŎg�p����J����
class TitleCamera final:public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};
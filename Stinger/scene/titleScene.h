#pragma once
#include "scene/scene.h"
// �^�C�g���̃I�u�W�F�N�g���Ǘ�����V�[��
class TitleScene final :public Scene
{
private:
	bool m_NextScene = false;
	float m_NextSceneTime = 0.0f;
public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;

	void CreateShadowVolume();
};

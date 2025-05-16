#pragma once
#include "scene/scene.h"
// �ǂݍ��ݒ��̃I�u�W�F�N�g���Ǘ����V�[��
class LoadScene final :public Scene
{
private:
	bool m_Load = false;

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;

	void LoadStart()
	{
		m_Load = true;
	}

	const bool& GetLoad()const
	{
		return m_Load;
	}
};
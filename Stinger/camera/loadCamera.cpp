#include "loadCamera.h"
#include "manager/inputManager.h"

void LoadCamera::Init()
{
	GameObject::Init();
	// �J�����̌���
	m_Target.z = 50.0f;
	m_Target.y = 5.0f;
	m_Position.z = -10.0f;
	m_Position.y = m_Target.y;
}





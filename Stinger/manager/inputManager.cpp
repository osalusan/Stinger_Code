#include "manager/inputManager.h"

BYTE InputManager::m_OldKeyState[256] = { "" };
BYTE InputManager::m_KeyState[256] = { ""};


void InputManager::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void InputManager::Update()
{
	// �L�[�{�[�h���擾�ł��Ȃ��ꍇ
	if (!GetKeyboardState(m_KeyState)) return;

	memcpy( m_OldKeyState, m_KeyState, 256 );
}
//��������
bool InputManager::GetKeyRelease(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

//�����Ă����
bool InputManager::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool InputManager::GetMouseLeftPress()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}
bool InputManager::GetMouseRightPress()
{
	return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

#pragma once
#include "main/main.h"
// �Q�[�������ׂĂ�f���͂��Ǘ�����N���X
class InputManager final
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
public:
	static void Init();
	static void Update();

	// �ȈՓI�ȍ\���ׁ̈A������2�̃L�[���͂��s��ꂽ��ARelese������ɓ��삵�Ȃ�
	static bool GetKeyRelease(BYTE KeyCode);
	static bool GetKeyPress(BYTE KeyCode );

	static bool GetMouseLeftPress();
	static bool GetMouseRightPress();
	
};

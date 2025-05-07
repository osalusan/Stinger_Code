#include "main.h"
#include "manager/sceneManager.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wrl.h>
#include <iostream>
#include <time.h>
#include "imgui/imgui_impl_win32.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "Stinger";
const char* GAME_VERSION = "1.2.0";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;
int g_Seed = 0;
bool g_WindowActive = false;
bool g_ShowMouse = false;

HWND GetWindow()
{
	return g_Window;
}
bool GetWindowActive()
{
	return g_WindowActive;
}
int XorShiftInt(int& state)
{
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

int GetRandom()
{
	return XorShiftInt(g_Seed);
}

// �{�[�_���X�E�C���h�E
void SetBorderlessWindow(HWND hwnd)
{
	// ���݂̃��j�^�[�̉𑜓x���擾
	HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	if (GetMonitorInfo(hMonitor, &mi))
	{
		// �{�[�_�[�𖳂���
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
	}
}

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(1847201);

	// �����̃V�[�h��ݒ�
	srand(static_cast<unsigned int>(time(0)));

	g_Seed = static_cast<int>(time(0));

	WNDCLASSEX wcex = { 0 };
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	#if _DEBUG
	
	#else // relese�̎��̂�
	 //�{�[�_���X�E�C���h�E
	SetBorderlessWindow(g_Window);
	#endif

	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);
	
	SceneManager::Init();

	// DeltaTime�p�ϐ�
	LARGE_INTEGER frequency = { 0 };
	LARGE_INTEGER prevTime = { 0 }, currTime = { 0 };

	// ���g���̏�����
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&prevTime);

	// �t���[���̐���
	const double& targetDeltaTime = 1.0 / 60.0;

	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			// �}�E�X�J�[�\���̕\��
#if _DEBUG
			ShowCursor(true);
#else
			if (g_ShowMouse != g_WindowActive)
			{
				g_ShowMouse = g_WindowActive;
				ShowCursor(!g_ShowMouse);
			}			
#endif

			// ���݂̎��Ԃ��擾
			QueryPerformanceCounter(&currTime);
			// �o�ߎ��Ԃ�b�Ōv�Z
			const double& deltaTime = static_cast<double>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;

			if (deltaTime >= targetDeltaTime)
			{
				// FPS���v�Z
				float deltaTimeFloat = static_cast<float>(deltaTime);
				
				// ����ݒ� / �u���C�N�|�C���g�p
				if (deltaTimeFloat >= 0.2f)
				{
					deltaTimeFloat = 0.0f;
				}

				SceneManager::Update(deltaTimeFloat);

				SceneManager::Draw();

				prevTime = currTime;
			}
		}
	}

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	SceneManager::Uninit();

	return (int)msg.wParam;
}


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		// ������return����ƁAOS �ɂ͓`��炸ImGui���ŏ���������
		return true;
	}

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_ACTIVATE:
		g_WindowActive = (wParam != WA_INACTIVE);

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



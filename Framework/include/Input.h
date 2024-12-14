#pragma once
#include "dinput.h"
#include <InlineUtil.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {

public:

	//�������֐�
	HRESULT InitDirectInput(HINSTANCE hInst, HWND hwnd);

	//�I���֐�
	void Term();

	//���͂̊J�n
	void SetInput();

	//�w�肵���L�[�̓��͏󋵂��m�F����
	BOOL GetInputState(int key);

private:

	LPDIRECTINPUT8 m_InputInterface;
	LPDIRECTINPUTDEVICE8 m_InputDevice;
	char m_DIKeys[256];

};
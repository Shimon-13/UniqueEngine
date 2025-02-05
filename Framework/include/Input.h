#pragma once
#include "dinput.h"
#include <InlineUtil.h>
#include <DirectXMath.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {

public:

	enum eInputTypes {
		KEYBOARD = 0,
		MOUSE,
	};

	//�������֐�
	HRESULT InitDirectInput(HINSTANCE hInst, HWND hwnd, eInputTypes type);

	//�I���֐�
	void Term();

	//���͂̊J�n
	void K_SetInput();	//�L�[�{�[�h�p
	void M_SetInput();	//�}�E�X�p

	//�w�肵���L�[�̓��͏󋵂��m�F����
	BOOL K_GetInputState(int key);
	BOOL M_GetInputState(byte button);
	DirectX::XMFLOAT2 M_GetInputVelocity();

private:

	LPDIRECTINPUT8 m_InputInterface;
	LPDIRECTINPUTDEVICE8 m_InputDevice;
	char m_DIKeys[256];
	DIMOUSESTATE m_DIMouse;
};
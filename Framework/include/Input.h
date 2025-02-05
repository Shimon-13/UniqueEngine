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

	//初期化関数
	HRESULT InitDirectInput(HINSTANCE hInst, HWND hwnd, eInputTypes type);

	//終了関数
	void Term();

	//入力の開始
	void K_SetInput();	//キーボード用
	void M_SetInput();	//マウス用

	//指定したキーの入力状況を確認する
	BOOL K_GetInputState(int key);
	BOOL M_GetInputState(byte button);
	DirectX::XMFLOAT2 M_GetInputVelocity();

private:

	LPDIRECTINPUT8 m_InputInterface;
	LPDIRECTINPUTDEVICE8 m_InputDevice;
	char m_DIKeys[256];
	DIMOUSESTATE m_DIMouse;
};
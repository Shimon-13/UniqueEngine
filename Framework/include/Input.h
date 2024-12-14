#pragma once
#include "dinput.h"
#include <InlineUtil.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {

public:

	//初期化関数
	HRESULT InitDirectInput(HINSTANCE hInst, HWND hwnd);

	//終了関数
	void Term();

	//入力の開始
	void SetInput();

	//指定したキーの入力状況を確認する
	BOOL GetInputState(int key);

private:

	LPDIRECTINPUT8 m_InputInterface;
	LPDIRECTINPUTDEVICE8 m_InputDevice;
	char m_DIKeys[256];

};
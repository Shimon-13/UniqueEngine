#include "Input.h"

HRESULT Input::InitDirectInput(HINSTANCE hInst, HWND hwnd) {
	
	//DirectInput8の生成
	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_InputInterface, nullptr);
	if (FAILED(hr)) {
		return S_FALSE;
	}

	//デバイスの生成
	hr = m_InputInterface->CreateDevice(GUID_SysKeyboard, &m_InputDevice, nullptr);
	if (FAILED(hr)) {
		Term();
		return S_FALSE;
	}

	hr = m_InputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		Term();
		return S_FALSE;
	}

	//入力の取り方、前方にいるときのみ取得し入力を独占しない
	hr = m_InputDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		Term();
		return S_FALSE;
	}

	m_InputDevice->Acquire();

	return S_OK;
}

//終了関数
void Input::Term() {
	
	m_InputDevice->Unacquire();

	SafeRelease(m_InputDevice);
	SafeRelease(m_InputInterface);

}

//入力の開始
void Input::SetInput() {
	
	//現在の入力を保持する
	ZeroMemory(m_DIKeys, sizeof(m_DIKeys));
	auto hr = m_InputDevice->GetDeviceState(sizeof(m_DIKeys), (LPVOID)m_DIKeys);
	if (FAILED(hr)) {
		//失敗なら再開させてもう一度取得
		m_InputDevice->Acquire();
		m_InputDevice->GetDeviceState(sizeof(m_DIKeys), (LPVOID)m_DIKeys);
	}
}

//指定したキーの入力情報を確認
BOOL Input::GetInputState(int key) {
	if (m_DIKeys[key] & 0x80) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
#include "Input.h"

HRESULT Input::InitDirectInput(HINSTANCE hInst, HWND hwnd, eInputTypes type) {
	
	//DirectInput8�̐���
	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_InputInterface, nullptr);
	if (FAILED(hr)) {
		return S_FALSE;
	}

	// �f�o�C�X�ɂ���Đݒ�ύX
	switch (type) {

	case KEYBOARD:

		// �f�o�C�X�̐���
		hr = m_InputInterface->CreateDevice(GUID_SysKeyboard, &m_InputDevice, nullptr);
		if (FAILED(hr)) {
			Term();
			return S_FALSE;
		}

		// �t�H�[�}�b�g�w��
		hr = m_InputDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) {
			Term();
			return S_FALSE;
		}
		break;

	case MOUSE:

		//�f�o�C�X�̐���
		hr = m_InputInterface->CreateDevice(GUID_SysMouse, &m_InputDevice, nullptr);
		if (FAILED(hr)) {
			Term();
			return S_FALSE;
		}

		hr = m_InputDevice->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr)) {
			Term();
			return S_FALSE;
		}

	}

	//���͂̎����A�O���ɂ���Ƃ��̂ݎ擾�����͂�Ɛ肵�Ȃ�
	hr = m_InputDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		Term();
		return S_FALSE;
	}

	m_InputDevice->Acquire();

	hr = m_InputDevice->Poll();
	if (FAILED(hr)) {
		return S_FALSE;
	}

	return S_OK;
}

//�I���֐�
void Input::Term() {
	
	if (m_InputDevice != nullptr) {
		m_InputDevice->Unacquire();
	}

	SafeRelease(m_InputDevice);
	SafeRelease(m_InputInterface);

}

//���͂̊J�n
void Input::K_SetInput() {
	
	//���݂̓��͂�ێ�����
	ZeroMemory(m_DIKeys, sizeof(m_DIKeys));
	auto hr = m_InputDevice->GetDeviceState(sizeof(m_DIKeys), (LPVOID)m_DIKeys);
	if (FAILED(hr)) {
		//���s�Ȃ�ĊJ�����Ă�����x�擾
		m_InputDevice->Acquire();
		m_InputDevice->GetDeviceState(sizeof(m_DIKeys), (LPVOID)m_DIKeys);
	}
}

void Input::M_SetInput() {
	auto hr = m_InputDevice->GetDeviceState(sizeof(m_DIMouse), (LPVOID)&m_DIMouse);
	if (FAILED(hr)) {
		//���s�Ȃ�ĊJ�����Ă�����x�擾
		m_InputDevice->Acquire();
		m_InputDevice->GetDeviceState(sizeof(m_DIMouse), (LPVOID)&m_DIMouse);
	}
}

//�w�肵���L�[�̓��͏����m�F
BOOL Input::K_GetInputState(int key) {
	if (m_DIKeys[key] & 0x80) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL Input::M_GetInputState(byte button) {
	if (m_DIMouse.rgbButtons[button] & 0x80) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

DirectX::XMFLOAT2 Input::M_GetInputVelocity() {
	return DirectX::XMFLOAT2(m_DIMouse.lX, m_DIMouse.lY);
}
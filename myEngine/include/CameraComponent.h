#pragma once

#include <DirectXMath.h>

#include "ComponentBase.h"

// �R���X�g���N�^---------------------------
// @param windowWidth �E�B���h�E�̕�
// @param windowHeight �E�B���h�E�̍���
// ------------------------------------------
class CameraComponent : public ComponentBase {

public:
	CameraComponent();
	~CameraComponent();

	bool Init(uint32_t windowWidth = 0, uint32_t windowHeight = 0);

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetRotation(float x, float y, float z);

private:
	DirectX::XMFLOAT3 m_CameraPosition;
	DirectX::XMFLOAT3 m_CameraTarget;
	DirectX::XMFLOAT4 m_Rotation;

	size_t m_windowWidth;
	size_t m_windowHeight;

	float m_FovY;								//������p
	float m_Aspect;								//�A�X�y�N�g��
	DirectX::XMFLOAT2 m_FrustumCullingDistance; //������̍ŋߒl�ƍŉ��l

};
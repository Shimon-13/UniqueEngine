#pragma once

#include "ComponentManager.h"

class CameraComponent : public Component {

public:
	CameraComponent();
	~CameraComponent();

	bool Init(uint32_t windowWidth, uint32_t windowHeight);
	void Term();

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

private:
	DirectX::XMFLOAT3 m_CameraPosition;
	DirectX::XMFLOAT3 m_CameraTarget;

	float m_FovY;								//������p
	float m_Aspect;								//�A�X�y�N�g��
	DirectX::XMFLOAT2 m_FrustumCullingDistance; //������̍ŋߒl�ƍŉ��l

};
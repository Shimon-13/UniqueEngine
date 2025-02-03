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

	float m_FovY;								//垂直画角
	float m_Aspect;								//アスペクト比
	DirectX::XMFLOAT2 m_FrustumCullingDistance; //視錐台の最近値と最遠値

};
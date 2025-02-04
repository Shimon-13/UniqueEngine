#pragma once

#include <DirectXMath.h>

#include "ComponentBase.h"

// コンストラクタ---------------------------
// @param windowWidth ウィンドウの幅
// @param windowHeight ウィンドウの高さ
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

	float m_FovY;								//垂直画角
	float m_Aspect;								//アスペクト比
	DirectX::XMFLOAT2 m_FrustumCullingDistance; //視錐台の最近値と最遠値

};
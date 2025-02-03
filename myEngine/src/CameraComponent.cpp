#include "CameraComponent.h"

CameraComponent::CameraComponent()
	: m_CameraPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_CameraTarget(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_FovY(0.0f)
	, m_Aspect(0.0f)
	, m_FrustumCullingDistance(DirectX::XMFLOAT2(0.0f, 0.0f))
{}

CameraComponent::~CameraComponent() {
	Term();
}

bool CameraComponent::Init(uint32_t const windowWidth, uint32_t const windowHeight) {
	m_CameraPosition = DirectX::XMFLOAT3(0.0f, -5.0f, 0.0f);
	m_CameraTarget = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);


	// 垂直画角とアスペクト比の設定.
	m_FovY = DirectX::XMConvertToRadians(37.5f);
	m_Aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	m_FrustumCullingDistance = DirectX::XMFLOAT2(1.0f, 1000.0f);
}

void CameraComponent::Term() {

}

DirectX::XMMATRIX CameraComponent::GetViewMatrix() const {
	auto camPos = DirectX::XMLoadFloat3(&m_CameraPosition);
	auto camTarget = DirectX::XMLoadFloat3(&m_CameraTarget);
	auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMMatrixLookAtRH(camPos, camTarget, upward);
}

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const {
	return DirectX::XMMatrixPerspectiveFovRH(m_FovY, m_Aspect, m_FrustumCullingDistance.x, m_FrustumCullingDistance.y);
}
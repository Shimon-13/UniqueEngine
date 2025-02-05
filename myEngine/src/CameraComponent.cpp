#include "CameraComponent.h"

CameraComponent::CameraComponent()
	: m_CameraPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_CameraTarget(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_Rotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
	, m_windowWidth(0)
	, m_windowHeight(0)
	, m_FovY(0.0f)
	, m_Aspect(0.0f)
	, m_FrustumCullingDistance(DirectX::XMFLOAT2(0.0f, 0.0f))
{}

CameraComponent::~CameraComponent() {
	Term();
}

bool CameraComponent::Init(uint32_t const windowWidth, uint32_t const windowHeight) {

	if (windowWidth == 0 || windowHeight == 0) {
		ELOG("Values Are Not Set In windowWidth or windowHeight");
		return false;
	}

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_CameraPosition = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_CameraTarget = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);


	// 垂直画角とアスペクト比の設定.
	m_FovY = DirectX::XMConvertToRadians(37.5f);
	m_Aspect = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);

	m_FrustumCullingDistance = DirectX::XMFLOAT2(1.0f, 1000.0f);

	return true;
}

void CameraComponent::SetPosition(float x, float y, float z) {
	m_CameraPosition.x = x;
	m_CameraPosition.y = y;
	m_CameraPosition.z = z;
}

void CameraComponent::SetTarget(float x, float y, float z) {
	m_CameraTarget.x = x;
	m_CameraTarget.y = y;
	m_CameraTarget.z = z;

}

void CameraComponent::SetRotation(float x, float y, float z) {
	auto quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
	DirectX::XMStoreFloat4(&m_Rotation, quat);
}

DirectX::XMMATRIX CameraComponent::GetViewMatrix() const {
	auto camPos = DirectX::XMLoadFloat3(&m_CameraPosition);
	auto camTarget = DirectX::XMLoadFloat3(&m_CameraTarget);
	auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMMatrixLookAtRH(camPos, camTarget, upward) * DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&m_Rotation));
}

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const {
	return DirectX::XMMatrixPerspectiveFovRH(m_FovY, m_Aspect, m_FrustumCullingDistance.x, m_FrustumCullingDistance.y);
}
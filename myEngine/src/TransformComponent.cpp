#include "TransformComponent.h"

TransformComponent::TransformComponent()
	: m_Position(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_Scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
	, m_Rotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
{	}

TransformComponent::~TransformComponent() {
	Term();
}

bool TransformComponent::Init(ComPtr<ID3D12Device> pDevice, DescriptorPool* pPool) {
	if (pDevice == nullptr || pPool == nullptr) {
		return false;
	}

	if (!m_Buffer.Init(pDevice.Get(), pPool, sizeof(TransformMatrices))) {
		ELOG("m_Buffer.Init() Failed.");
		return false;
	}

	return true;
}

void TransformComponent::Term() {
	m_Buffer.Term();
}

D3D12_GPU_VIRTUAL_ADDRESS TransformComponent::GetAddress() {
	return m_Buffer.GetAddress();
}

void TransformComponent::SetBuffer(const DirectX::XMMATRIX& proj, const DirectX::XMMATRIX& view) {
	m_Buffer.GetPtr<TransformMatrices>()->Proj = proj;
	m_Buffer.GetPtr<TransformMatrices>()->View = view;
	m_Buffer.GetPtr<TransformMatrices>()->World = GetMatrix();
}

DirectX::XMMATRIX TransformComponent::GetMatrix() const {
	auto position = DirectX::XMLoadFloat3(&m_Position);
	auto scale = DirectX::XMLoadFloat3(&m_Scale);
	auto rotation = DirectX::XMLoadFloat4(&m_Rotation);

	return DirectX::XMMatrixAffineTransformation(scale, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), rotation, position);
}

DirectX::XMFLOAT3 TransformComponent::GetPosition() const {
	return m_Position;
}

DirectX::XMFLOAT3 TransformComponent::GetScale() const {
	return m_Scale;
}

DirectX::XMFLOAT4 TransformComponent::GetQuaternionRotation() const {
	return m_Rotation;
}

void TransformComponent::SetPosition(const float x, const float y, const float z) {
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void TransformComponent::SetScale(const float x, const float y, const float z) {
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void TransformComponent::SetRotation(const float x, const float y, const float z) {
	auto quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
	DirectX::XMStoreFloat4(&m_Rotation, quat);
}
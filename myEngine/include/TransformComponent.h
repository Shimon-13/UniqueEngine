#pragma once

#include "Logger.h"
#include <FileUtil.h>
#include "ComponentManager.h"

class TransformComponent : public Component {

public:
	TransformComponent();
	~TransformComponent();
	void Term();

	DirectX::XMMATRIX GetMatrix() const;
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetScale() const;
	DirectX::XMFLOAT3 GetEularRotation() const;
	DirectX::XMFLOAT4 GetQuaternionRotation() const;

	void SetPosition(const float x, const float y, const float z);
	void SetScale(const float x, const float y, const float z);
	void SetRotation(const float x, const float y, const float z);

private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT4 m_Rotation;

	ConstantBuffer m_TransformBuffer;

};
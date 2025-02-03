#pragma once

#include "Logger.h"
#include <FileUtil.h>
#include "ComponentManager.h"

struct TransformMatrices
{
	DirectX::XMMATRIX   World;      //!< ���[���h�s��ł�.
	DirectX::XMMATRIX   View;       //!< �r���[�s��ł�.
	DirectX::XMMATRIX   Proj;       //!< �ˉe�s��ł�.
};

class TransformComponent : public Component {

public:
	TransformComponent();
	~TransformComponent();
	bool Init(ID3D12Device* pDevice, DescriptorPool* pPool);
	void Term();

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;
	bool SetMatrices(DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj);

	DirectX::XMMATRIX GetMatrix() const;
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetScale() const;
	DirectX::XMFLOAT3 GetEularRotation() const;
	DirectX::XMFLOAT4 GetQuaternionRotation() const;

	void SetPosition(float const x, float const y, float const z);
	void SetScale(float const x, float const y, float const z);
	void SetRotation(float const x, float const y, float const z);

private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT4 m_Rotation;

	ConstantBuffer m_TransformBuffer;	//�t���[���̐����K�v?
	size_t m_BufferSize;

};
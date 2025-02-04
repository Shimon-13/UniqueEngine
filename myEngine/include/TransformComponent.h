#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <memory>
#include <DescriptorPool.h>
#include <ConstantBuffer.h>

#include "ComponentBase.h"

struct TransformMatrices
{
	DirectX::XMMATRIX   World;      //!< ���[���h�s��ł�.
	DirectX::XMMATRIX   View;       //!< �r���[�s��ł�.
	DirectX::XMMATRIX   Proj;       //!< �ˉe�s��ł�.
};

// �R���X�g���N�^-----------------------------------------
// @param pDevice �f�o�C�X�|�C���^
// @param pPool �f�B�X�N���v�^�v�[���|�C���^(POOL_TYPE_RES)
// @param pCamera �`��p�J�����R���|�[�l���g(shared_ptr)
// -------------------------------------------------------
class TransformComponent : public ComponentBase {

public:
	TransformComponent();
	~TransformComponent();

	bool Init(ID3D12Device* pDevice, DescriptorPool* pPool);
	void OnUpdate() override;
	void Term() override;

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress();
	void SetBuffer(const DirectX::XMMATRIX& proj, const DirectX::XMMATRIX& view);

	DirectX::XMMATRIX GetMatrix() const;
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetScale() const;
	// DirectX::XMFLOAT3 GetEularRotation() const;
	DirectX::XMFLOAT4 GetQuaternionRotation() const;

	void SetPosition(float const x, float const y, float const z);
	void SetScale(float const x, float const y, float const z);
	void SetRotation(float const x, float const y, float const z);

private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT4 m_Rotation;

	ConstantBuffer m_Buffer;
};
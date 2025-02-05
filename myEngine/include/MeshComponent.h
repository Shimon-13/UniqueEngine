#pragma once

#include <memory>
#include <d3d12.h>
#include <DescriptorPool.h>
#include <string>
#include <vector>
#include <Material.h>
#include <FileUtil.h>
#include <Logger.h>
#include <Mesh.h>
#include <InlineUtil.h>

#include "ComponentBase.h"

// コンストラクタ-------------------------------------------
// @param pDevice デバイスポインタ
// @param pQueue キューポインタ
// @param pPool ディスクリプタプールポインタ(POOL_TYPE_RES)
// @param path メッシュのファイルパス
// ---------------------------------------------------------
class MeshComponent : public ComponentBase{

public:

	MeshComponent();
	~MeshComponent();

	bool Init(ComPtr<ID3D12Device> pDevice, ComPtr<ID3D12CommandQueue> pQueue, DescriptorPool* pPool, const wchar_t* path);
	void Term() override;

	void DrawInstance(ID3D12GraphicsCommandList* pCmd, D3D12_GPU_VIRTUAL_ADDRESS transform);	//ルートシグネチャに依存する
	std::wstring GetPath() const;

private:
	std::wstring m_Path;
	std::vector<Mesh*> m_pMesh;
	Material m_Material;
};
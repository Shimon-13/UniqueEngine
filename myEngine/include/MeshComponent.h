#pragma once

#include <Mesh.h>
#include "Logger.h"
#include <FileUtil.h>
#include <Material.h>
#include <inlineUtil.h>
#include "ComponentManager.h"

class MeshComponent : public Component{

public:
	MeshComponent();
	~MeshComponent();
	bool Init(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool, const wchar_t* path);
	void Term();

	std::wstring GetPath() const;

private:
	std::wstring m_Path;
	std::vector<Mesh*> m_pMesh;
	Material m_Material;
};
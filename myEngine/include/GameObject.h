#pragma once

#include "ComponentManager.h"

class GameObject {

public:

	ComponentManager m_ComponentManager;

	GameObject(std::string const name, ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool);
	std::string GetName() const;

private:
	std::string const m_Name;
};
#include "GameObject.h"

GameObject::GameObject(std::string const name, ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool)
	: m_Name(name)
{
	m_ComponentManager.Init(pDevice, pQueue, pPool);
}

std::string GameObject::GetName() const {
	return m_Name;
}
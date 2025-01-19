#include "ComponentManager.h"
#include <DirectXMath.h>

class GameObject {

public:

	ComponentManager m_ComponentManager;

	GameObject(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool);

private:

};

GameObject::GameObject(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool){
	m_ComponentManager.Init(pDevice, pQueue, pPool);
}
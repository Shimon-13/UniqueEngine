#pragma once

#include <memory>
#include "MeshComponent.h"
#include "TransformComponent.h"

class Component {};

union InitData {
	const wchar_t* MeshPath;
};


class ComponentManager {

public:
	enum eCOMPONENTS {
		TRANSFORM = 0,
		MESH,

		ALL,
	};

	ComponentManager();

	bool Init(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool);
	Component* GetComponent(eCOMPONENTS type) const;
	bool AddComponent(eCOMPONENTS type, InitData& Data);

private:
	std::unique_ptr<Component*> m_pComponents[ALL];

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pQueue;
	DescriptorPool* m_pPool;

};
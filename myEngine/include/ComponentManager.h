#pragma once

#include <memory>
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

class Component {};

union InitData {
	struct MESH {
		const wchar_t* MeshPath;
	}mesh;
	struct CAMERA {
		size_t width;
		size_t height;
	}camera;
};


class ComponentManager {

public:
	enum eCOMPONENTS {
		TRANSFORM = 0,
		MESH,
		CAMERA,

		ALL,
	};

	ComponentManager();

	bool Init(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool);
	void Term();

	Component* GetComponent(eCOMPONENTS type) const;	//使用時はstatic_castを用いてポインタの型を変換する必要がある
	bool AddComponent(eCOMPONENTS type, InitData& Data);

private:
	std::unique_ptr<Component> m_pComponents[ALL];

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pQueue;
	DescriptorPool* m_pPool;

};
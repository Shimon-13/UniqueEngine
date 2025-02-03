#include "ComponentManager.h"

ComponentManager::ComponentManager()
	: m_pDevice(nullptr)
	, m_pQueue(nullptr)
	, m_pPool(nullptr)
{
	for (size_t i = 0; i < ALL; ++i) {
		m_pComponents[i] = std::make_unique<Component>(nullptr);
	}
}

bool ComponentManager::Init(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool) {
	m_pDevice = pDevice;
	m_pQueue = pQueue;
	m_pPool = pPool;
}

void ComponentManager::Term() {
	m_pDevice = nullptr;
	m_pQueue = nullptr;
	m_pPool = nullptr;
}

Component* ComponentManager::GetComponent(eCOMPONENTS type) const {
	return m_pComponents[type].get();
}

bool ComponentManager::AddComponent(eCOMPONENTS type, InitData& data) {

	//追加済みかチェック
	if (m_pComponents[type].get() != nullptr) {
		
		return false;
	}

	switch (type) {

	case TRANSFORM:
		TransformComponent* transCmp = new (std::nothrow) TransformComponent();

		if (transCmp == nullptr) {
			ELOG("Out of Memory On Generating TransformComponent.");
			return false;
		}

		m_pComponents[TRANSFORM].reset(transCmp);

		if (transCmp->Init(m_pDevice, m_pPool)) {
			return false;
		}

		break;

	case MESH:
		MeshComponent* meshCmp = new (std::nothrow) MeshComponent();

		if (meshCmp == nullptr) {
			ELOG("Out of Memory On Generating MeshComponent.");
			return false;
		}

		m_pComponents[MESH].reset(meshCmp);

		if (!meshCmp->Init(m_pDevice, m_pQueue, m_pPool, data.mesh.MeshPath)) {
			return false;
		}
		break;

	case CAMERA:
		CameraComponent* camCmp = new (std::nothrow) CameraComponent();

		if (camCmp == nullptr) {
			ELOG("Out of Memory On Generating CameraComponent.");
			return false;
		}

		m_pComponents[CAMERA].reset(camCmp);

		if (!camCmp->Init(data.camera.width, data.camera.height)) {
			return false;
		}

		break;
	}

	return true;
}
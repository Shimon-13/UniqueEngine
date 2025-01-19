#include "ComponentManager.h"

ComponentManager::ComponentManager()
	: m_pDevice(nullptr)
	, m_pQueue(nullptr)
	, m_pPool(nullptr)
{
	for (size_t i = 0; i < ALL; ++i) {
		m_pComponents[i] = std::make_unique<Component*>(nullptr);
	}
}

bool ComponentManager::Init(ID3D12Device* pDevice, ID3D12CommandQueue* pQueue, DescriptorPool* pPool) {
	m_pDevice = pDevice;
	m_pQueue = pQueue;
	m_pPool = pPool;
}

Component* ComponentManager::GetComponent(eCOMPONENTS type) const {
	return *m_pComponents[type];
}

bool ComponentManager::AddComponent(eCOMPONENTS type, InitData& data) {

	//追加済みかチェック
	if (*m_pComponents[type] != nullptr) {
		
		return false;
	}

	switch (type) {

	case MESH:
		MeshComponent* meshCmp = new (std::nothrow) MeshComponent();

		if (meshCmp == nullptr) {
			ELOG("Out of Memory On Generating MeshComponent.");
			return false;
		}

		*m_pComponents[MESH] = meshCmp;

		if (!meshCmp->Init(m_pDevice, m_pQueue, m_pPool, data.MeshPath)) {
			return false;
		}
		break;

	case TRANSFORM:
		TransformComponent* transCmp = new (std::nothrow) TransformComponent();

		if (transCmp == nullptr) {
			ELOG("Out of Memory On Generating TransformComponent.");
			return false;
		}

		*m_pComponents[TRANSFORM] = transCmp;
	}

	return true;
}
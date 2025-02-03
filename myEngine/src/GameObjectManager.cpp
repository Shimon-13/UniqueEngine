#include "GameObjectManager.h"

GameObject* GameObjectManager::GetGameObject(std::string name) {
	if (!m_GameObjects.count(name)) {
		ELOG("GameObject Not Found.");
		return nullptr;
	}
	return m_GameObjects[name].get();
}

void GameObjectManager::SetGameObject(std::string name, GameObject* obj) {

	if (name.empty() || obj == nullptr) {
		ELOG("GameObjectManager::SetGameObject() Failed.");
	}
	else {
		m_GameObjects[name] = std::make_unique<GameObject>(*obj);
	}
}
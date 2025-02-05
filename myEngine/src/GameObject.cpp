#include "GameObject.h"


void GameObject::SetName(std::string name) {
	m_Name = name;
}

std::string GameObject::GetName() const {
	return m_Name;
}

void GameObject::RemoveComponent() {

	for (auto it = m_Components.begin(); it != m_Components.end();) {

		// コンポーネントの中身なし
		if (it->second == nullptr) {
			m_Components.erase(it->first);
			ELOG("The Component Contains Nothing");
			return;
		}

		it->second->Term();
		m_Components.erase(it->first);
	}
}

void GameObject::Update() {

	for (auto&& components : m_Components) {
		if (components.second == nullptr) {
			continue;
		}

		components.second->OnUpdate();
	}
}
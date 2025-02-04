#include "GameObject.h"


void GameObject::SetName(std::string name) {
	m_Name = name;
}

std::string GameObject::GetName() const {
	return m_Name;
}

void GameObject::Update() {

	for (auto&& components : m_Components) {
		if (components.second == nullptr) {
			continue;
		}

		components.second->OnUpdate();
	}
}
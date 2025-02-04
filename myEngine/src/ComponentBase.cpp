#include "ComponentBase.h"

void ComponentBase::Init() {
	
}

void ComponentBase::OnUpdate() {

}

void ComponentBase::Term() {

}

ComponentBase::ComponentBase()
	: m_Owner(nullptr) {

}

std::weak_ptr<GameObject> ComponentBase::GetOwner() const {
	return m_Owner;
}

void ComponentBase::SetOwner(std::shared_ptr<GameObject> owner) {
	m_Owner = owner;
}
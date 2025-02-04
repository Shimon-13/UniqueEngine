#include "GameObject.h"

template<typename ComponentType, typename ...ArgTypes>
void GameObject::AddComponent(std::string name, ArgTypes ...args) {


	// �o�^�ς݂��`�F�b�N
	auto it = m_Components.find(name);

	if (it == m_Components.end()) {

		// �C���X�^���X�쐬
		std::shared_ptr<ComponentBase> instance = std::make_shared<ComponentType>(args);
		if (instance == nullptr) {
			ELOG("AddComponent() Failed.");
			return;
		}

		// �R���|�[�l���g�̏��L�҂Ɏ��g���Z�b�g
		instance->SetOwner(shared_from_this());

		// �R���|�[�l���g�������ăR���|�[�l���g��ۑ�
		m_Components[name] = instance;
	}
	else {
		ELOG("The ComponentName is already registered. Name : %s", name);
	}

}

template<typename ComponentType>
void GameObject::RemoveComponent() {

	std::string name = typeid(ComponentType).name();

	// �o�^�ς݂��`�F�b�N
	auto it = m_Components.find(name);

	// �R���|�[�l���g�͑��݂��Ȃ�
	if (it == m_Components.end()) {
		ELOG("The Component Is Not Exist.");
		return;
	}

	// �R���|�[�l���g�̒��g�Ȃ�
	if (it->second == nullptr) {
		m_Components.erase(name);
		ELOG("The Component Contains Nothing");
		return;
	}

	it->second->Term();
	m_Components.erase(name);
}

template<typename ComponentType>
std::weak_ptr<ComponentBase> GameObject::GetComponent() const {

	std::string name = typeid(ComponentType).name();

	// �o�^�ς݂��`�F�b�N
	auto it = m_Components.find(name);
	if (it == m_Components.end()) {
		ELOG("There Is No Such Component Name.");
		return std::weak_ptr<ComponentBase>();
	}

	return it->second;

}

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
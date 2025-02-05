#include "ObjectManager.h"

ObjectManager::~ObjectManager() {
	Term();
}

void ObjectManager::GenerateObject(std::string name) {
	
	// �C���X�^���X����
	std::shared_ptr<GameObject> instance = std::make_shared<GameObject>();
	// �I�u�W�F�N�g�����Z�b�g
	std::string objName = CreateObjName(name);

	instance->SetName(objName);

	// �I�u�W�F�N�g�����X�g�ɒǉ����A�C�e���[�^�擾
	m_lInstances.emplace_back(instance);
	auto objItr = std::prev(m_lInstances.end());

	//�}�b�v�ɓo�^
	m_GameObjectItr[objName] = objItr;

}

std::weak_ptr<GameObject> ObjectManager::GetGameObject(std::string const name) const{

	auto it = m_GameObjectItr.find(name);
	if (it == m_GameObjectItr.end() || *it->second == nullptr) {
		ELOG("There Is No Such Object Name.");
		return std::weak_ptr<GameObject>();
	}
	else {
		return *it->second;
	}

}

void ObjectManager::DestroyObject(std::string name) {
	auto it = m_GameObjectItr.find(name);

	// ���̖��O��������Ȃ����A�}�b�v�̒��g���Ȃ������ꍇ
	if (it == m_GameObjectItr.end() || it->second->get() == nullptr) {
		// ���g���Ȃ������ꍇ
		if (it != m_GameObjectItr.end()) {
			m_GameObjectItr.erase(name);
		}
	}
	// �������ꍇ
	else {
		it->second->get()->RemoveComponent();
		// �}�b�v�̕R�Â����폜
		m_GameObjectItr.erase(name);
		// �C���X�^���X�̍폜
		m_lInstances.erase(it->second);
	}
}

std::string ObjectManager::CreateObjName(std::string name) 
{

	
	// �����̖��O�����݂��Ă��邩�`�F�b�N
	auto it = m_GameObjectItr.find(name);

	// ���݂��Ȃ�������Z�b�g
	if (it == m_GameObjectItr.end()) {
		return name;
	}

	// ���݂���ꍇ�A���ɔԍ���t������
	size_t i = 1;
	std::string newName;
	bool isExist = true;
	while (isExist) {

		newName = name + std::to_string(i);

		// �d���m�F
		it = m_GameObjectItr.find(newName);
		if (it == m_GameObjectItr.end()) {
			isExist = false;
			return newName;
		}

	}

	return "NO NAME";
}

void ObjectManager::Update() {
	for (auto&& object : m_lInstances) {
		if (object.get() == nullptr) {
			continue;
		}
		else {
			object->Update();
		}
	}
}

void ObjectManager::Term() {

	for (auto it = m_lInstances.begin(); it != m_lInstances.end();) {
			m_GameObjectItr.erase(it->get()->GetName());
			it = m_lInstances.erase(it);
	}

}
#include "ObjectManager.h"

ObjectManager::~ObjectManager() {
	Term();
}

void ObjectManager::GenerateObject(std::string name) {
	
	// インスタンス生成
	std::shared_ptr<GameObject> instance = std::make_shared<GameObject>();
	// オブジェクト名をセット
	std::string objName = CreateObjName(name);

	instance->SetName(objName);

	// オブジェクトをリストに追加し、イテレータ取得
	m_lInstances.emplace_back(instance);
	auto objItr = std::prev(m_lInstances.end());

	//マップに登録
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

	// その名前が見つからないか、マップの中身がなかった場合
	if (it == m_GameObjectItr.end() || it->second->get() == nullptr) {
		// 中身がなかった場合
		if (it != m_GameObjectItr.end()) {
			m_GameObjectItr.erase(name);
		}
	}
	// 見つけた場合
	else {
		it->second->get()->RemoveComponent();
		// マップの紐づけを削除
		m_GameObjectItr.erase(name);
		// インスタンスの削除
		m_lInstances.erase(it->second);
	}
}

std::string ObjectManager::CreateObjName(std::string name) 
{

	
	// 引数の名前が存在しているかチェック
	auto it = m_GameObjectItr.find(name);

	// 存在しなかったらセット
	if (it == m_GameObjectItr.end()) {
		return name;
	}

	// 存在する場合、後ろに番号を付け足す
	size_t i = 1;
	std::string newName;
	bool isExist = true;
	while (isExist) {

		newName = name + std::to_string(i);

		// 重複確認
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
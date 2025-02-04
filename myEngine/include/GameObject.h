#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <Logger.h>

#include "ComponentBase.h"

class ComponentBase;

class GameObject : public std::enable_shared_from_this<GameObject>{

public:
	template<typename ComponentType, typename ...ArgTypes>
	void AddComponent(std::string name, ArgTypes ...args);
	template<typename ComponentType>
	void RemoveComponent();
	template<typename ComponentType>
	std::weak_ptr<ComponentBase> GetComponent() const;
	void SetName(std::string name);
	std::string GetName() const;
	void Update();

private:
	std::string m_Name;

	std::unordered_map<std::string, std::shared_ptr<ComponentBase>> m_Components;
};

template<typename ComponentType, typename ...ArgTypes>
void GameObject::AddComponent(std::string name, ArgTypes ...args) {


	// 登録済みかチェック
	auto it = m_Components.find(name);

	if (it == m_Components.end()) {

		// インスタンス作成
		std::shared_ptr<ComponentBase> instance = std::make_shared<ComponentType>();
		if (instance == nullptr) {
			ELOG("AddComponent() Failed.");
			return;
		}

		static_cast<ComponentType*>(instance.get())->Init(args...);

		// コンポーネントの所有者に自身をセット
		instance->SetOwner(shared_from_this());

		// コンポーネント名をつけてコンポーネントを保存
		m_Components[name] = instance;
	}
	else {
		ELOG("The ComponentName is already registered. Name : %s", name);
	}

}

template<typename ComponentType>
void GameObject::RemoveComponent() {

	std::string name = typeid(ComponentType).name();

	// 登録済みかチェック
	auto it = m_Components.find(name);

	// コンポーネントは存在しない
	if (it == m_Components.end()) {
		ELOG("The Component Is Not Exist.");
		return;
	}

	// コンポーネントの中身なし
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

	// 登録済みかチェック
	auto it = m_Components.find(name);
	if (it == m_Components.end()) {
		ELOG("There Is No Such Component Name.");
		return std::weak_ptr<ComponentBase>();
	}

	return it->second;

}
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <Logger.h>

#include "ComponentBase.h"

class ComponentBase;

class GameObject{

public:
	template<typename ComponentType, typename ...ArgTypes>
	void AddComponent(ArgTypes ...args);
	void RemoveComponent();
	template<typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent() const;
	void SetName(std::string name);
	std::string GetName() const;
	void Update();

private:
	std::string m_Name;

	std::unordered_map<std::string, std::shared_ptr<ComponentBase>> m_Components;
};

template<typename ComponentType, typename ...ArgTypes>
void GameObject::AddComponent(ArgTypes ...args) {

		// インスタンス作成
		std::shared_ptr<ComponentBase> instance = std::make_shared<ComponentType>();
		if (instance == nullptr) {
			ELOG("Generating Component is Failed.");
			return;
		}

		static_cast<ComponentType*>(instance.get())->Init(args...);

		// コンポーネント名をつけてコンポーネントを保存
		m_Components[typeid(ComponentType).name()] = instance;

}


template<typename ComponentType>
std::weak_ptr<ComponentType> GameObject::GetComponent() const {

	std::string name = typeid(ComponentType).name();

	// 登録済みかチェック
	auto it = m_Components.find(name);
	if (it == m_Components.end()) {
		ELOG("There Is No Such Component Name.");
		return std::weak_ptr<ComponentType>();
	}

	return std::dynamic_pointer_cast<ComponentType>(it->second);

}
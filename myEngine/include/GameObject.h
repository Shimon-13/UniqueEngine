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
#pragma once

#include "GameObject.h"

class GameObjectManager {

public:

	GameObject* GetGameObject(std::string name);
	void SetGameObject(std::string name, GameObject* obj);

private:

	std::map<std::string, std::unique_ptr<GameObject>> m_GameObjects;

};
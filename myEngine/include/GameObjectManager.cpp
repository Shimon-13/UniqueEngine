#include "GameObject.h"

class GameObjectManager {

public:
	std::vector<std::unique_ptr<GameObject>> m_GameObjects;

};
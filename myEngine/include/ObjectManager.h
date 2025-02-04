#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <Logger.h>

#include "GameObject.h"

class ObjectManager {


public:

	std::shared_ptr<GameObject> GenerateObject(std::string name);
	std::weak_ptr<GameObject> GetGameObject(std::string const name) const;
	void DestroyObject(std::string name);
	void Update();

private:

	// �d�����Ȃ����O�𐶐�����
	std::string CreateObjName(std::string name);

	// �Q�[���I�u�W�F�N�g���w���C�e���[�^�ƃQ�[���I�u�W�F�N�g����R�Â���
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>::iterator> m_GameObjectItr;

	// �Q�[���I�u�W�F�N�g�̃C���X�^���X
	std::list<std::shared_ptr<GameObject>> m_lInstances;

};
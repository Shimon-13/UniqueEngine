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

	// 重複しない名前を生成する
	std::string CreateObjName(std::string name);

	// ゲームオブジェクトを指すイテレータとゲームオブジェクト名を紐づける
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>::iterator> m_GameObjectItr;

	// ゲームオブジェクトのインスタンス
	std::list<std::shared_ptr<GameObject>> m_lInstances;

};
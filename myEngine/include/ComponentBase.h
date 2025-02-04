#pragma once

#include <memory>

#include "GameObject.h"

class GameObject;

class ComponentBase {

public:

	ComponentBase();

	// 初期化処理
	virtual void Init();

	// 更新処理
	virtual void OnUpdate();

	// 終了処理
	virtual void Term();


	//コンポーネントの所有者を取得
	std::weak_ptr<GameObject> GetOwner() const;

	//コンポーネントの所有者をセット
	void SetOwner(std::shared_ptr<GameObject> owner);

private:

	//コンポーネントの所有者
	std::shared_ptr<GameObject> m_Owner;
};
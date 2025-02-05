#pragma once

#include <memory>

#include "GameObject.h"

class GameObject;

class ComponentBase {

public:

	ComponentBase();

	// ����������
	virtual void Init();

	// �X�V����
	virtual void OnUpdate();

	// �I������
	virtual void Term();
};
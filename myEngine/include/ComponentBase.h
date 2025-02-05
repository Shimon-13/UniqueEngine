#pragma once

#include <memory>

#include "GameObject.h"

class GameObject;

class ComponentBase {

public:

	ComponentBase();

	// ‰Šú‰»ˆ—
	virtual void Init();

	// XVˆ—
	virtual void OnUpdate();

	// I—¹ˆ—
	virtual void Term();
};
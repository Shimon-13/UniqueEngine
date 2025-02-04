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


	//�R���|�[�l���g�̏��L�҂��擾
	std::weak_ptr<GameObject> GetOwner() const;

	//�R���|�[�l���g�̏��L�҂��Z�b�g
	void SetOwner(std::shared_ptr<GameObject> owner);

private:

	//�R���|�[�l���g�̏��L��
	std::shared_ptr<GameObject> m_Owner;
};
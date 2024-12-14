#include <ConstantBuffer.h>
#include <Mesh.h>
#include <Material.h>
#include <DirectXMath.h>

class GameObject {

public:
	GameObject();

private:
	std::wstring m_Path;
	std::vector<Mesh*> m_pMesh;
	std::vector<Material*> m_pMaterial;
	ConstantBuffer* m_pCB;
	DirectX::XMMATRIX m_Position;
};

GameObject::GameObject()
	: m_Path()
	, m_pMesh()
	, m_pMaterial()
	, m_pCB(nullptr)
	, m_Position(DirectX::XMMatrixIdentity())
	{ /* DO NOTHING */ }
#include "MeshComponent.h"

MeshComponent::MeshComponent()
{ /* DO NOTHING */ }

MeshComponent::~MeshComponent() {
	Term();
}

bool MeshComponent::Init(ComPtr<ID3D12Device> pDevice, ComPtr<ID3D12CommandQueue> pQueue, DescriptorPool* pPool, const wchar_t* path) {

	// �t�@�C���p�X������.
	if (!SearchFilePath(path, m_Path))
	{
		ELOG("Error : File Not Found.");
		return false;
	}

	//�t�@�C���p�X����f�B���N�g���̃p�X���擾
	std::wstring dir = GetDirectoryPath(m_Path.c_str());

	std::vector<ResMesh>        resMesh;
	std::vector<ResMaterial>    resMaterial;

	// ���b�V�����\�[�X�����[�h.
	if (!LoadMesh(m_Path.c_str(), resMesh, resMaterial))
	{
		ELOG("Error : Load Mesh Failed. filepath = %ls", m_Path.c_str());
		return false;
	}

	// ��������\��.
	m_pMesh.reserve(resMesh.size());

	// ���b�V����������.
	for (size_t i = 0; i < resMesh.size(); ++i)
	{
		// ���b�V������.
		auto mesh = new(std::nothrow) Mesh();

		// �`�F�b�N.
		if (mesh == nullptr)
		{
			ELOG("Error : Out of memory.");
			return false;
		}

		// ����������.
		if (!mesh->Init(pDevice.Get(), resMesh[i]))
		{
			ELOG("Error : Mesh Initialize Failed.");
			return false;
		}

		// ����������o�^.
		m_pMesh.push_back(mesh);
	}

	// �������œK��.
	m_pMesh.shrink_to_fit();

	// �}�e���A��������.
	if (!m_Material.Init(
		pDevice.Get(),
		pPool,
		0,
		resMaterial.size()))
	{
		ELOG("Error : Material::Init() Failed.");
		return false;
	}

	// ���\�[�X�o�b�`��p��.
	DirectX::ResourceUploadBatch batch(pDevice.Get());

	// �o�b�`�J�n.
	batch.Begin();

	// �e�N�X�`���ݒ�.
	for (size_t i = 0; i < resMaterial.size(); ++i)
	{
		std::wstring path = dir + resMaterial[i].DiffuseMap;
		m_Material.SetTexture(i, TU_DIFFUSE, path, batch);
	}

	// �o�b�`�I��.
	auto future = batch.End(pQueue.Get());

	// �o�b�`������ҋ@.
	future.wait();

	return true;
}


void MeshComponent::Term() 
{
	// ���b�V���j��.
	for (size_t i = 0; i < m_pMesh.size(); ++i)
	{
		SafeTerm(m_pMesh[i]);
	}
	m_pMesh.clear();
	m_pMesh.shrink_to_fit();

	// �}�e���A���j��.
	m_Material.Term();
}

void MeshComponent::DrawInstance(ID3D12GraphicsCommandList* pCmd) {
	for (size_t i = 0; i < m_pMesh.size(); ++i)
	{
		// �}�e���A��ID���擾.
		auto id = m_pMesh[i]->GetMaterialId();

		// �e�N�X�`����ݒ�.
		pCmd->SetGraphicsRootDescriptorTable(1, m_Material.GetTextureHandle(id, TU_DIFFUSE));

		// ���b�V����`��.
		m_pMesh[i]->Draw(pCmd);
	}
}

std::wstring MeshComponent::GetPath() const {
	return m_Path;
}
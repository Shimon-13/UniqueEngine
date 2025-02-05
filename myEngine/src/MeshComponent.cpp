#include "MeshComponent.h"

MeshComponent::MeshComponent()
{ /* DO NOTHING */ }

MeshComponent::~MeshComponent() {
	Term();
}

bool MeshComponent::Init(ComPtr<ID3D12Device> pDevice, ComPtr<ID3D12CommandQueue> pQueue, DescriptorPool* pPool, const wchar_t* path) {

	// ファイルパスを検索.
	if (!SearchFilePath(path, m_Path))
	{
		ELOG("Error : File Not Found.");
		return false;
	}

	//ファイルパスからディレクトリのパスを取得
	std::wstring dir = GetDirectoryPath(m_Path.c_str());

	std::vector<ResMesh>        resMesh;
	std::vector<ResMaterial>    resMaterial;

	// メッシュリソースをロード.
	if (!LoadMesh(m_Path.c_str(), resMesh, resMaterial))
	{
		ELOG("Error : Load Mesh Failed. filepath = %ls", m_Path.c_str());
		return false;
	}

	// メモリを予約.
	m_pMesh.reserve(resMesh.size());

	// メッシュを初期化.
	for (size_t i = 0; i < resMesh.size(); ++i)
	{
		// メッシュ生成.
		auto mesh = new(std::nothrow) Mesh();

		// チェック.
		if (mesh == nullptr)
		{
			ELOG("Error : Out of memory.");
			return false;
		}

		// 初期化処理.
		if (!mesh->Init(pDevice.Get(), resMesh[i]))
		{
			ELOG("Error : Mesh Initialize Failed.");
			return false;
		}

		// 成功したら登録.
		m_pMesh.push_back(mesh);
	}

	// メモリ最適化.
	m_pMesh.shrink_to_fit();

	// マテリアル初期化.
	if (!m_Material.Init(
		pDevice.Get(),
		pPool,
		0,
		resMaterial.size()))
	{
		ELOG("Error : Material::Init() Failed.");
		return false;
	}

	// リソースバッチを用意.
	DirectX::ResourceUploadBatch batch(pDevice.Get());

	// バッチ開始.
	batch.Begin();

	// テクスチャ設定.
	for (size_t i = 0; i < resMaterial.size(); ++i)
	{
		std::wstring path = dir + resMaterial[i].DiffuseMap;
		m_Material.SetTexture(i, TU_DIFFUSE, path, batch);
	}

	// バッチ終了.
	auto future = batch.End(pQueue.Get());

	// バッチ完了を待機.
	future.wait();

	return true;
}


void MeshComponent::Term() 
{
	// メッシュ破棄.
	for (size_t i = 0; i < m_pMesh.size(); ++i)
	{
		SafeTerm(m_pMesh[i]);
	}
	m_pMesh.clear();
	m_pMesh.shrink_to_fit();

	// マテリアル破棄.
	m_Material.Term();
}

void MeshComponent::DrawInstance(ID3D12GraphicsCommandList* pCmd) {
	for (size_t i = 0; i < m_pMesh.size(); ++i)
	{
		// マテリアルIDを取得.
		auto id = m_pMesh[i]->GetMaterialId();

		// テクスチャを設定.
		pCmd->SetGraphicsRootDescriptorTable(1, m_Material.GetTextureHandle(id, TU_DIFFUSE));

		// メッシュを描画.
		m_pMesh[i]->Draw(pCmd);
	}
}

std::wstring MeshComponent::GetPath() const {
	return m_Path;
}
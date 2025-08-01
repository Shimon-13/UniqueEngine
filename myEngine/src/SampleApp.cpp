﻿//-----------------------------------------------------------------------------
// File : SampleApp.cpp
// Desc : Sample Application Module.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SampleApp.h"
#include "FileUtil.h"
#include "Logger.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"


///////////////////////////////////////////////////////////////////////////////
// SampleApp class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
SampleApp::SampleApp(uint32_t width, uint32_t height)
: App(width, height)
, m_RotateAngle(0.0)
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
SampleApp::~SampleApp()
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      初期化時の処理です.
//-----------------------------------------------------------------------------
bool SampleApp::OnInit()
{
    //ゲームオブジェクトの初期化
    {
        GameObject* camera = new GameObject("camera", m_pDevice.Get(), m_pQueue.Get(), m_pPool[POOL_TYPE_RES]);
        GameObject* obj1 = new GameObject("teapot", m_pDevice.Get(), m_pQueue.Get(), m_pPool[POOL_TYPE_RES]);
        m_GameObjMgr.SetGameObject(camera->GetName(), camera);
        m_GameObjMgr.SetGameObject(obj1->GetName(), obj1);

        InitData data = {};

        data.camera.width = m_Width;
        data.camera.height = m_Height;
        m_GameObjMgr.GetGameObject(camera->GetName())->m_ComponentManager.AddComponent(ComponentManager::CAMERA, data);
        m_GameObjMgr.GetGameObject(camera->GetName())->m_ComponentManager.AddComponent(ComponentManager::TRANSFORM, data);


        m_GameObjMgr.GetGameObject(obj1->GetName())->m_ComponentManager.AddComponent(ComponentManager::TRANSFORM, data);

        data.mesh.MeshPath = L"../res/teapot/teapot.obj";
        m_GameObjMgr.GetGameObject(obj1->GetName())->m_ComponentManager.AddComponent(ComponentManager::MESH, data);
    }

    // ルートシグニチャの生成.
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        // ディスクリプタレンジを設定.
        D3D12_DESCRIPTOR_RANGE range = {};
        range.RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors                    = 1;
        range.BaseShaderRegister                = 0;
        range.RegisterSpace                     = 0;
        range.OffsetInDescriptorsFromTableStart = 0;

        // ルートパラメータの設定.
        D3D12_ROOT_PARAMETER param[2] = {};
        param[0].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[0].Descriptor.ShaderRegister = 0;
        param[0].Descriptor.RegisterSpace  = 0;
        param[0].ShaderVisibility          = D3D12_SHADER_VISIBILITY_VERTEX;

        // ルートパラメータを設定.
        param[1].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param[1].DescriptorTable.NumDescriptorRanges = 1;
        param[1].DescriptorTable.pDescriptorRanges   = &range;
        param[1].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_PIXEL;

        // スタティックサンプラーの設定.
        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter              = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler.AddressU            = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressV            = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressW            = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.MipLODBias          = D3D12_DEFAULT_MIP_LOD_BIAS;
        sampler.MaxAnisotropy       = 1;
        sampler.ComparisonFunc      = D3D12_COMPARISON_FUNC_NEVER;
        sampler.BorderColor         = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        sampler.MinLOD              = -D3D12_FLOAT32_MAX;
        sampler.MaxLOD              = +D3D12_FLOAT32_MAX;
        sampler.ShaderRegister      = 0;
        sampler.RegisterSpace       = 0;
        sampler.ShaderVisibility    = D3D12_SHADER_VISIBILITY_PIXEL;

        // ルートシグニチャの設定.
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters      = 2;
        desc.NumStaticSamplers  = 1;
        desc.pParameters        = param;
        desc.pStaticSamplers    = &sampler;
        desc.Flags              = flag;

        ComPtr<ID3DBlob> pBlob;
        ComPtr<ID3DBlob> pErrorBlob;

        // シリアライズ
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf());
        if (FAILED(hr))
        { return false; }

        // ルートシグニチャを生成.
        hr = m_pDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(m_pRootSig.GetAddressOf()));
        if (FAILED(hr))
        {
            ELOG( "Error : Root Signature Create Failed. retcode = 0x%x", hr );
            return false;
        }
    }

    // パイプラインステートの生成.
    {
        std::wstring vsPath;
        std::wstring psPath;

        // 頂点シェーダを検索.
        if (!SearchFilePath(L"../bin/SimpleTexVS.cso", vsPath))
        {
            ELOG( "Error : Vertex Shader Not Found.");
            return false;
        }

        // ピクセルシェーダを検索.
        if (!SearchFilePath(L"../bin/SimpleTexPS.cso", psPath))
        {
            ELOG( "Error : Pixel Shader Node Found.");
            return false;
        }

        ComPtr<ID3DBlob> pVSBlob;
        ComPtr<ID3DBlob> pPSBlob;

        // 頂点シェーダを読み込む.
        auto hr = D3DReadFileToBlob( vsPath.c_str(), pVSBlob.GetAddressOf() );
        if ( FAILED(hr) )
        {
            ELOG( "Error : D3DReadFiledToBlob() Failed. path = %ls", vsPath.c_str() );
            return false;
        }

        // ピクセルシェーダを読み込む.
        hr = D3DReadFileToBlob( psPath.c_str(), pPSBlob.GetAddressOf() );
        if ( FAILED(hr) )
        {
            ELOG( "Error : D3DReadFileToBlob() Failed. path = %ls", psPath.c_str() );
            return false;
        }

        // グラフィックスパイプラインステートを設定.
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout            = MeshVertex::InputLayout;
        desc.pRootSignature         = m_pRootSig.Get();
        desc.VS                     = { pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize() };
        desc.PS                     = { pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize() };
        desc.RasterizerState        = DirectX::CommonStates::CullNone;
        desc.BlendState             = DirectX::CommonStates::Opaque;
        desc.DepthStencilState      = DirectX::CommonStates::DepthDefault;
        desc.SampleMask             = UINT_MAX;
        desc.PrimitiveTopologyType  = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets       = 1;
        desc.RTVFormats[0]          = m_ColorTarget[0].GetViewDesc().Format;
        desc.DSVFormat              = m_DepthTarget.GetViewDesc().Format;
        desc.SampleDesc.Count       = 1;
        desc.SampleDesc.Quality     = 0;

        // パイプラインステートを生成.
        hr = m_pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
        if ( FAILED(hr) )
        {
            ELOG( "Error : ID3D12Device::CreateGraphicsPipelineState() Failed. retcode = 0x%x", hr );
            return false;
        }

        m_RotateAngle = 0.0f;
    }

    return true;
}

//-----------------------------------------------------------------------------
//      終了時の処理です.
//-----------------------------------------------------------------------------
void SampleApp::OnTerm()
{
    // メッシュ破棄.
    for(size_t i=0; i<m_pMesh.size(); ++i)
    { SafeTerm(m_pMesh[i]); }
    m_pMesh.clear();
    m_pMesh.shrink_to_fit();

    // マテリアル破棄.
    m_Material.Term();

    // 変換バッファ破棄.
    for(size_t i=0; i<m_Transform.size(); ++i)
    { SafeTerm(m_Transform[i]); }
    m_Transform.clear();
    m_Transform.shrink_to_fit();
}

//-----------------------------------------------------------------------------
//      描画時の処理です.
//-----------------------------------------------------------------------------
void SampleApp::OnRender()
{
    // 更新処理.
    {
        if (m_Keyboard.GetInputState(DIK_A)) {
            m_RotateAngle -= 0.025f;
        }
        if (m_Keyboard.GetInputState(DIK_D)) {
            m_RotateAngle += 0.025f;
        }

        auto camTrans = static_cast<TransformComponent*>(m_GameObjMgr.GetGameObject("camera")->m_ComponentManager.GetComponent(ComponentManager::TRANSFORM));
        auto camera = static_cast<CameraComponent*>(m_GameObjMgr.GetGameObject("camera")->m_ComponentManager.GetComponent(ComponentManager::CAMERA));

        camTrans->SetRotation(0.0f, m_RotateAngle, 0.0f);

        auto transform = static_cast<TransformComponent*>(m_GameObjMgr.GetGameObject("teapot")->m_ComponentManager.GetComponent(ComponentManager::TRANSFORM));
        transform->SetMatrices(camera->GetViewMatrix(), camera->GetProjectionMatrix());
    }

    // コマンドリストの記録を開始.
    auto pCmd = m_CommandList.Reset();

    // 書き込み用リソースバリア設定.
    DirectX::TransitionResource(pCmd,
        m_ColorTarget[m_FrameIndex].GetResource(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    // ディスクリプタ取得.
    auto handleRTV = m_ColorTarget[m_FrameIndex].GetHandleRTV();
    auto handleDSV = m_DepthTarget.GetHandleDSV();

    // レンダーターゲットを設定.
    pCmd->OMSetRenderTargets(1, &handleRTV->HandleCPU, FALSE, &handleDSV->HandleCPU);

    // クリアカラー.
    float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

    // レンダーターゲットをクリア.
    pCmd->ClearRenderTargetView(handleRTV->HandleCPU, clearColor, 0, nullptr);

    // 深度ステンシルビューをクリア.
    pCmd->ClearDepthStencilView(handleDSV->HandleCPU, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // 描画処理.
    {
        ID3D12DescriptorHeap* const pHeaps[] = {
            m_pPool[POOL_TYPE_RES]->GetHeap()
        };

        auto trans = static_cast<TransformComponent*>(m_GameObjMgr.GetGameObject("teapot")->m_ComponentManager.GetComponent(ComponentManager::TRANSFORM));

        pCmd->SetGraphicsRootSignature( m_pRootSig.Get() );
        pCmd->SetDescriptorHeaps( 1, pHeaps );
        pCmd->SetGraphicsRootConstantBufferView( 0, trans->GetAddress() );
        pCmd->SetPipelineState( m_pPSO.Get() );
        pCmd->RSSetViewports( 1, &m_Viewport );
        pCmd->RSSetScissorRects( 1, &m_Scissor );

        auto mesh = static_cast<MeshComponent*>(m_GameObjMgr.GetGameObject("teapot")->m_ComponentManager.GetComponent(ComponentManager::MESH));
        mesh->DrawInstance(pCmd);
    }

    // 表示用リソースバリア設定.
    DirectX::TransitionResource(pCmd,
        m_ColorTarget[m_FrameIndex].GetResource(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    // コマンドリストの記録を終了.
    pCmd->Close();

    // コマンドリストを実行.
    ID3D12CommandList* pLists[] = { pCmd };
    m_pQueue->ExecuteCommandLists( 1, pLists );

    // 画面に表示.
    Present(1);
}

//-----------------------------------------------------------------------------
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
        m_ObjMgr.GenerateObject("mainCamera");
        m_ObjMgr.GenerateObject("player");

        if (!m_ObjMgr.GetGameObject("mainCamera").expired()) {
            m_ObjMgr.GetGameObject("mainCamera").lock()->AddComponent<CameraComponent>(m_Width, m_Height);
        }
        if (!m_ObjMgr.GetGameObject("player").expired()) {
            m_ObjMgr.GetGameObject("player").lock()->AddComponent<TransformComponent>(m_pDevice, m_pPool[POOL_TYPE_RES]);
            m_ObjMgr.GetGameObject("player").lock()->AddComponent<MeshComponent>(m_pDevice, m_pQueue, m_pPool[POOL_TYPE_RES], L"../res/player/player.fbx");
        }
        

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
    m_ObjMgr.DestroyObject("mainCamera");
    m_ObjMgr.DestroyObject("player");
}

//-----------------------------------------------------------------------------
//      描画時の処理です.
//-----------------------------------------------------------------------------
void SampleApp::OnRender()
{
    // 更新処理.
    {

        if (m_ObjMgr.GetGameObject("mainCamera").expired()) {
            ELOG("The Object Or Component Is Nothing.");
            if (m_ObjMgr.GetGameObject("mainCamera").lock()->GetComponent<CameraComponent>().expired()) {
                ELOG("The Object Or Component Is Nothing.");
            }
        }
        auto camera = m_ObjMgr.GetGameObject("mainCamera").lock()->GetComponent<CameraComponent>().lock();

        if (m_ObjMgr.GetGameObject("player").expired()) {
            ELOG("The Object Or Component Is Nothing.");
            if (m_ObjMgr.GetGameObject("player").lock()->GetComponent<TransformComponent>().expired()) {
                ELOG("The Object Or Component Is Nothing.");
            }
        }
        auto transform = m_ObjMgr.GetGameObject("player").lock()->GetComponent<TransformComponent>().lock();

        float moveX = 0.0f;
        float moveY = 0.0f;
        float moveZ = 0.0f;

        if (m_Keyboard.K_GetInputState(DIK_A)) {
            moveX += 0.025f;
        }
        if (m_Keyboard.K_GetInputState(DIK_D)) {
            moveX -= 0.025f;
        }
        if (m_Keyboard.K_GetInputState(DIK_W)) {
            moveZ += 0.025f;
        }
        if (m_Keyboard.K_GetInputState(DIK_S)) {
            moveZ -= 0.025f;
        }

        transform->SetPosition(transform->GetPosition().x + moveX, 0.0f, transform->GetPosition().z + moveZ);

        transform->SetBuffer(camera->GetProjectionMatrix(), camera->GetViewMatrix());   //　座標変換用の行列更新

        m_ObjMgr.Update();   //必ず更新
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


        if (m_ObjMgr.GetGameObject("player").expired()) {
            if (m_ObjMgr.GetGameObject("player").lock()->GetComponent<TransformComponent>().expired()) {
                ELOG("The Object Or Component Is Nothing.");
            }
        }

        auto teapotrans = m_ObjMgr.GetGameObject("player").lock()->GetComponent<TransformComponent>().lock();

        if (m_ObjMgr.GetGameObject("player").lock()->GetComponent<MeshComponent>().expired()) {
            ELOG("The Object Or Component Is Nothing.");
        }

        auto teapotMesh = m_ObjMgr.GetGameObject("player").lock().get()->GetComponent<MeshComponent>().lock();

        pCmd->SetGraphicsRootSignature( m_pRootSig.Get() );
        pCmd->SetDescriptorHeaps( 1, pHeaps );
        pCmd->SetGraphicsRootConstantBufferView( 0, teapotrans->GetAddress() );
        pCmd->SetPipelineState( m_pPSO.Get() );
        pCmd->RSSetViewports( 1, &m_Viewport );
        pCmd->RSSetScissorRects( 1, &m_Scissor );


         teapotMesh->DrawInstance(pCmd);


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

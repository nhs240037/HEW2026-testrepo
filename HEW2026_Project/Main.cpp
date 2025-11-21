#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"
#include "SceneGame.h"
#include "Defines.h"
#include "ShaderList.h"
#include"CsvData.h"

#include <fstream>   // ファイル操作用
#include <sstream>   // 文字列ストリーム用
#include <string>
#include <vector>
#include<DirectXMath.h>

//--- グローバル変数
Scene* g_pScene; HWND g_hWnd = nullptr;

// DirectX デバイス関連
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pTexSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

// --- アプリケーションのリソース ---
// (これまでに説明した要素)

// 1. シェーダー
ID3D11VertexShader* g_pVertexShader = nullptr;
ID3D11PixelShader* g_pPixelShader = nullptr;
ID3D11InputLayout* g_pInputLayout = nullptr;

// 2. ポリゴン (頂点バッファ)
ID3D11Buffer* g_pVertexBuffer = nullptr;

// 3. テクスチャ (あなたのTextureクラス)
Texture* g_pMyTexture = nullptr;
ID3D11SamplerState* g_pSamplerState = nullptr;

// 4. 色変更用 (定数バッファ)
ID3D11Buffer* g_pConstantBufferColor = nullptr;
DirectX::XMFLOAT4     g_vTintColor(1.0f, 1.0f, 1.0f, 1.0f); // 更新処理で変更する色

// 頂点構造体
struct MyVertex
{
	DirectX::XMFLOAT3 pos; // 座標
	DirectX::XMFLOAT2 uv;  // UV座標
};

// 定数バッファ構造体 (HLSL側に合わせる)
struct ConstantBufferMatrix
{
	DirectX::XMMATRIX worldViewProjection;
};

struct ConstantBufferColor
{
	DirectX::XMFLOAT4 tintColor;
};

// --- 関数のプロトタイプ ---
HRESULT InitDevice(); // 初期化
void    TexUpdate();     // 更新
void    Render();     // 描画
void    Cleanup();    // 終了
void    SetTextureColor(const DirectX::XMFLOAT4& color);



HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }

	// 他機能初期化
	Geometory::Init();
	Sprite::Init();
	InitInput();
	ShaderList::Init();

	// シーン
	g_pScene = new SceneGame();

	CsvData& csv = CsvData::get_instance();
	csv.Init();

	return hr;
}

void Uninit()
{
	if (g_pScene) delete g_pScene;

	ShaderList::Uninit();
	UninitInput();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();
}

void Update()
{
	UpdateInput();
	g_pScene->RootUpdate();
}

void Draw()
{
	BeginDrawDirectX();

	// 軸線の表示
#ifdef _DEBUG
	// グリッド
	DirectX::XMFLOAT4 lineColor(0.5f, 0.5f, 0.5f, 1.0f);
	float size = DEBUG_GRID_NUM * DEBUG_GRID_MARGIN;
	for (int i = 1; i <= DEBUG_GRID_NUM; ++i)
	{
		float grid = i * DEBUG_GRID_MARGIN;
		DirectX::XMFLOAT3 pos[2] = {
			DirectX::XMFLOAT3(grid, 0.0f, size),
			DirectX::XMFLOAT3(grid, 0.0f,-size),
		};
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = pos[1].x = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = size;
		pos[1].x = -size;
		pos[0].z = pos[1].z = grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].z = pos[1].z = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
	}
	// 軸
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(size,0,0), DirectX::XMFLOAT4(1,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,size,0), DirectX::XMFLOAT4(0,1,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,size), DirectX::XMFLOAT4(0,0,1,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(-size,0,0),  DirectX::XMFLOAT4(0,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,-size),  DirectX::XMFLOAT4(0,0,0,1));

	Geometory::DrawLines();

	// カメラの値
	static bool camAutoSwitch = false;
	static bool camUpDownSwitch = true;
	static float camAutoRotate = 1.0f;
	if (IsKeyTrigger(VK_RETURN)) {
		camAutoSwitch ^= true;
	}
	if (IsKeyTrigger(VK_SPACE)) {
		camUpDownSwitch ^= true;
	}

	DirectX::XMVECTOR camPos;
	if (camAutoSwitch) {
		camAutoRotate += 0.01f;
	}
	camPos = DirectX::XMVectorSet(
		cosf(camAutoRotate) * 5.0f,
		3.5f * (camUpDownSwitch ? 1.0f : -1.0f),
		sinf(camAutoRotate) * 5.0f,
		0.0f);

	// ジオメトリ用カメラ初期化
	DirectX::XMFLOAT4X4 mat[2];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			camPos,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f)
	));
	Geometory::SetView(mat[0]);
	Geometory::SetProjection(mat[1]);
#endif

	g_pScene->RootDraw();
	EndDrawDirectX();
}

HRESULT InitDevice()
{
	// 1. デバイス、スワップチェーン、レンダーターゲットビューの作成
	// (D3D11CreateDeviceAndSwapChain, GetBuffer, CreateRenderTargetView ...)
	// ...

	// 2. ビューポートの設定
	// (RSSetViewports ...)
	// ...

	// 3. シェーダーのコンパイルと作成
	// (D3DCompileFromFile, CreateVertexShader, CreatePixelShader ...)
	// ...

	// 4. 入力レイアウトの作成
	// (CreateInputLayout ...)
	// ...

	// 5. 頂点バッファの作成
	// (CreateBuffer で g_pVertexBuffer を作成)
	// ...

	// 6. テクスチャの読み込み (あなたのクラスを使用)
	g_pMyTexture = new Texture();
	g_pMyTexture->Create("Assets/Texture/Fade.png"); // または .tga

	// 7. サンプラーステートの作成
	// (CreateSamplerState で g_pSamplerState を作成)
	// ...

	// 8. 定数バッファの作成 (色変更用)
	// (CreateBuffer で g_pConstantBufferColor を作成)
	// ...

	return S_OK;
}

void TexUpdate()
{
	// 描画処理で使うために、色情報を更新しておく
	g_vTintColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// (ここでオブジェクトの座標計算なども行う)
}

void Render()
{
	// 1. レンダーターゲットをクリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // (背景色)
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	// 2. 定数バッファの更新 (Updateの結果をGPUへ転送)
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (SUCCEEDED(g_pImmediateContext->Map(g_pConstantBufferColor, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		ConstantBufferColor* pCB = (ConstantBufferColor*)mappedResource.pData;
		pCB->tintColor = g_vTintColor; // Update() で計算した色
		g_pImmediateContext->Unmap(g_pConstantBufferColor, 0);
	}

	// 3. IA (Input Assembler) ステージの設定
	// (IASetInputLayout, IASetVertexBuffers, IASetPrimitiveTopology ...)
	// ...

	// 4. VS (Vertex Shader) ステージの設定
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);

	// 5. PS (Pixel Shader) ステージの設定
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);

	// 【テクスチャ貼りと色変更】
	// シェーダーに必要なリソースをすべてセット
	ID3D11ShaderResourceView* pSRV = g_pMyTexture->GetResource();
	g_pImmediateContext->PSSetShaderResources(0, 1, &pSRV);           // t0: テクスチャ
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerState);        // s0: サンプラー
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBufferColor); // b0: 色情報

	// 6. 描画コマンドの発行
	g_pImmediateContext->Draw(4, 0); // (例: 頂点4つで四角形を描画)

	// 7. バックバッファとフロントバッファを交換 (画面に表示)
	g_pTexSwapChain->Present(0, 0);
}

void Cleanup()
{
	// 解放漏れ (メモリリーク) を防ぐため、
	// 作成したものをすべて SAFE_RELEASE (または delete) する

	SAFE_RELEASE(g_pConstantBufferColor);
	SAFE_RELEASE(g_pSamplerState);

	if (g_pMyTexture)
	{
		delete g_pMyTexture;
		g_pMyTexture = nullptr;
	}

	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pInputLayout);
	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(g_pVertexShader);

	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pTexSwapChain);
	SAFE_RELEASE(g_pImmediateContext);
	SAFE_RELEASE(g_pd3dDevice);

	// (ウィンドウの破棄)
}

void SetTextureColor(const DirectX::XMFLOAT4& color)
{
	// グローバル変数の値を更新するだけ
	g_vTintColor = color;
}
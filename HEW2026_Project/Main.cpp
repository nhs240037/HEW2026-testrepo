#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"
#include "SceneGame.h"
#include "Defines.h"
#include "ShaderList.h"
#include "CsvData.h"
#include "CameraDebug.h"
#include "Sound.h"

//ImGuiの必要ファイルをインクルード
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
// 情報を伝えるためのヘッダーファイル
#include "Transfer.h"


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


	// csvのデータを取得
	CsvData& csv = CsvData::get_instance();
	csv.Init();
	
	// カメラのインスタンス取得
	CAMERA_INS
	c_pos.m_posY = 10.0f * DEBUG_DISTANCE;
	c_pos.m_posZ = -10.0f * DEBUG_DISTANCE;
	// 値を入れるためのインスタンスを取得
	Transfer& tran = Transfer::GetInstance();
	tran.Init();
	tran.camera.eyePos.x = 0.0f;
	tran.camera.eyePos.y = 10.0f * DEBUG_DISTANCE;
	tran.camera.eyePos.z = -10.0f * DEBUG_DISTANCE;

	// サウンドマネージャを取得
	SoundManager& sound = SoundManager::GetInstance();
	sound.Init();

	return hr;
}

void Uninit()
{
	if (g_pScene) delete g_pScene;

	ShaderList::Uninit();
	UninitInput();
	Sprite::Uninit();
	Geometory::Uninit();

	SE_INS;
	sound.Uninit();
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


	// ───────────────────────
	// ImGui 複数ウィンドウ
	// ───────────────────────
#ifdef _DEBUG
	static bool show_main_window	= true;
	static bool show_player_window	= false;
	static bool show_camera_window	= false;
	static bool show_stage_window	= false;
	static bool show_item_window	= false;
	static bool show_order_window	= false;
	static bool show_ui_window		= false;

	CAMERA_INS
		TRAN_INS
		SoundManager& sound = SoundManager::GetInstance();

	if (IsKeyPress('U') && IsKeyPress('I') && IsKeyPress('O') && IsKeyPress('P'))
		show_main_window = true;

	if (show_main_window)
	{
		ImGui::Begin("Setting", &show_main_window);
		// カメラの位置情報の表示と変更に伴った更新

		if (ImGui::Button("Init"))
			tran.Init();

		ImGui::Checkbox("Player Setting",&show_player_window);
		ImGui::Checkbox("Camera Setting", &show_camera_window);
		ImGui::Checkbox("Stage Setting", &show_stage_window);
		ImGui::Checkbox("Item Setting", &show_item_window);
		ImGui::Checkbox("Order Setting", &show_order_window);
		ImGui::Checkbox("UI Setting", &show_ui_window);

		// -----------Sound Debug-----------//
		if (ImGui::Button("UpMoney"))
			sound.PlaySE(0);
		if (ImGui::Button("ride"))
			sound.PlaySE(1);
		if (ImGui::Button("show order"))
			sound.PlaySE(2);
		if (ImGui::Button("order success"))
			sound.PlaySE(3);
		if (ImGui::Button("walk"))
			sound.PlaySE(4);
		if (ImGui::Button("drop"))
			sound.PlaySE(5);

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
	// -----------プレイヤー-----------
	if (show_player_window)
	{
		ImGui::Begin("Player Setting", &show_player_window);
		//-----------プレイヤー-----------//
		// プレイヤーの位置情報
		float p_pos[2] = { tran.player.pos.x ,tran.player.pos.y };

		ImGui::SliderFloat2("Pos", p_pos, -12.0f, 12.0f);

		tran.player.pos.x = p_pos[0];
		tran.player.pos.y = p_pos[1];

		// プレイヤーの最大速度
		float p_speed[2];
		p_speed[0] = tran.player.maxSpeed.x;
		p_speed[1] = tran.player.maxSpeed.y;
		// プレイヤーの加速度
		float p_velocity = tran.player.velocity;
		ImGui::SliderFloat("Player Speed Up", &p_velocity, 0.0f, 0.50f);
		tran.player.velocity = p_velocity;

		// プレイヤーの減速度
		float p_speedDown = tran.player.speedDown;

		ImGui::SliderFloat("Player Speed Down", &p_speedDown, 0.25f, 1.10);

		tran.player.speedDown = p_speedDown;

		ImGui::SliderFloat2("Player Max Speed", p_speed, 0.0f, 10.0f);
		tran.player.maxSpeed.x = p_speed[0];
		tran.player.maxSpeed.y = p_speed[1];

		ImGui::End();
	}

	// -----------カメラ-----------
	if (show_camera_window)
	{
		ImGui::Begin("Camera Setting",&show_camera_window);
		ImGui::Text("Camera EyePosition");
		// カメラの位置情報
		float camera_pos[3] = { tran.camera.eyePos.x,tran.camera.eyePos.y,tran.camera.eyePos.z };

		ImGui::SliderFloat3("Camera Pos", camera_pos, -1000.0f, 1000.0f);

		if (camera_pos[0] == 0.0f && camera_pos[2] == 0.0f && camera_pos[1] == 0.0f)
		{
			camera_pos[0] = 0.01f;
			camera_pos[1] = 0.01f;
			camera_pos[2] = 0.01f;
		}
		// 注視点
		tran.camera.eyePos.x = camera_pos[0];
		tran.camera.eyePos.y = camera_pos[1];
		tran.camera.eyePos.z = camera_pos[2];
		float camera_lookPos[3] = {tran.camera.lookPos.x,tran.camera.lookPos.y,tran.camera.lookPos.z};
		ImGui::SliderFloat3("Camera Look",camera_lookPos,-500.0f,500.0f);
		tran.camera.lookPos.x = camera_lookPos[0];
		tran.camera.lookPos.y = camera_lookPos[1];
		tran.camera.lookPos.z = camera_lookPos[2];
		ImGui::End();
	}

	// -----------ステージ-----------
	if (show_stage_window)
	{
		ImGui::Begin("Stage", &show_stage_window);
		int column = tran.stage.column;
		int row = tran.stage.row;
		ImGui::SliderInt("Stage Width", &column, 0, 20);
		ImGui::SliderInt("Stage Height", &row, 0, 20);

		tran.stage.column = column;
		tran.stage.row = row;

		ImGui::End();
	}

	// -----------食材-----------
	if (show_item_window)
	{
		ImGui::Begin("Item", &show_stage_window);

		float downSpeed = tran.item.downSpeed;
		ImGui::SliderFloat("Item DownSpeed", &downSpeed, 0.0f, 1.0f);
		tran.item.downSpeed = downSpeed;

		static bool size_lock;
		ImGui::Checkbox("Item Size Interlocking", &size_lock);
		if (!size_lock)
		{
			float size[3] = { tran.item.size.x,tran.item.size.y,tran.item.size.z };
			ImGui::SliderFloat3("Item Size", size, 0.0f, 10.0f);
			tran.item.size.x = size[0];
			tran.item.size.y = size[1];
			tran.item.size.z = size[2];
		}
		else
		{
			float size = tran.item.size.x;
			ImGui::SliderFloat("Item Size", &size, 0.0f, 10.0f);
			tran.item.size.x = size;
			tran.item.size.y = size;
			tran.item.size.z = size;
		}

		float repopTime = tran.item.repopTime;
		ImGui::SliderFloat("Item RepopTime", &repopTime, 0.017f, 5.0f);
		tran.item.repopTime = repopTime;

		ImGui::End();
	}

	// -----------オーダー-----------
	if (show_order_window)
	{
		ImGui::Begin("Order", &show_stage_window);

		float size[2] = { tran.order.size.x,tran.order.size.y };
		ImGui::SliderFloat2("Order size", size, 0.0f, 10.0f);
		tran.order.size.x = size[0];
		tran.order.size.y = size[1];

		float TimeLimit = tran.order.TimeLimit;
		ImGui::SliderFloat("Order TimeLimit", &TimeLimit, 5, 3600);
		if (ImGui::Button("Apply"));// 多分誰かがタイマーを設定した数値にリセットしてくれるはず
		tran.order.TimeLimit = TimeLimit;
		float repopTime = tran.order.repopTime;
		ImGui::SliderFloat("Order RepopTime",&repopTime,0.017f,20.0f);
		tran.order.repopTime = repopTime;

		ImGui::End();
	}

	// -----------UI-----------
	if (show_ui_window)
	{
		ImGui::Begin("UI", &show_stage_window);
		ImGui::Text("Nothing");
		ImGui::End();
	}


#endif
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
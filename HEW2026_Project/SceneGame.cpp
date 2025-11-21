#include "SceneGame.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "Camera.h"
#include "CameraDebug.h"
#include "Score.h"
#include <ctime>

SceneGame::SceneGame()
		: m_pBlock{nullptr}, m_menu{}, csv(CsvData::get_instance())
{

	srand((unsigned)time(nullptr));

	//--- モデルの描画
	RenderTarget *pRTV = GetDefaultRTV();
	DepthStencil *pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);

	m_pModel = new Model();
	m_pScore = new CScore();
	m_pTimer = new CTimer();
	m_pNextItem = new NextItem();
	m_pCamera = new CameraDebug();
	m_pPlayer = new Player();

	float x = RandomFloat(-5.0f, 5.0f);
	float z = RandomFloat(-5.0f, 5.0f);
	int randIndex = rand() % Block::None; // or correct enum size
	Block::Block_Color randColor = static_cast<Block::Block_Color>(randIndex);
	m_pBlock[0] = new Block(m_pNextItem->Next(), x, z);
	m_pBlock[0]->GetCamera(m_pCamera);

	m_pPlayer->SetCamera(m_pCamera);
	m_pBlock[0]->SetCollision({m_pPlayer->GetPos().x, m_pPlayer->GetPos().z});

	csv.Init();

	for (int i = 0; i < 5; i++)
	{
		m_menu[i] = csv.GetHambuger()->type[i];
	}
}

SceneGame::~SceneGame()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pScore)
	{
		delete m_pScore;
		m_pScore = nullptr;
	}
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		delete m_pBlock[i];
		m_pBlock[i] = nullptr;
	}

	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
	SAFE_DELETE(m_pNextItem);
}

void SceneGame::Update()
{
	m_pCamera->Update();
	m_pPlayer->SetCamera(m_pCamera);
	m_pPlayer->Update();
	// Update の先頭でプレイヤー位置を各ブロックに渡す
	DirectX::XMFLOAT3 playerPos = m_pPlayer->GetPos();
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		if (m_pBlock[i] != nullptr)
		{
			m_pBlock[i]->SetPlayerPos(playerPos);
			m_pBlock[i]->GetCamera(m_pCamera);
		}
	}
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (m_pBlock[i] == nullptr)
			continue;
		if (m_pBlock[i]->GetState() == Block::BlockState::Block_Catch)
		{
			for (int j = 0; j < MAX_BLOCK; j++)
			{

				if (m_pBlock[j] != nullptr)
					continue;

				float x = RandomFloat(-5.0f, 5.0f);
				float z = RandomFloat(-5.0f, 5.0f);
				int randIndex = rand() % Block::None; // or correct enum size
				Block::Block_Color randColor = static_cast<Block::Block_Color>(randIndex);
				m_pBlock[j] = new Block(m_pNextItem->Next());
				m_pBlock[j]->GetCamera(m_pCamera);
				m_pBlock[j]->SetStep(m_pBlock[i]->GetStep() + 1);
				break;
			}
			m_pBlock[i]->SetState(Block::BlockState::Block_Catched);
		}
		if (m_pBlock[i]->GetState() == Block::BlockState::Block_Idle)
		{
			float x = RandomFloat(-5.0f, 5.0f);
			float z = RandomFloat(-5.0f, 5.0f);
			int randIndex = rand() % Block::None; // or correct enum size
			Block::Block_Color randColor = static_cast<Block::Block_Color>(randIndex);
			m_pBlock[i] = new Block(m_pNextItem->Next(), x, z);
			m_pBlock[i]->GetCamera(m_pCamera);
		}
		if (m_pBlock[i]->GetState() == Block::BlockState::Block_Drop ||
				m_pBlock[i]->GetState() == Block::BlockState::Block_Catched)
		{
			m_pBlock[i]->Update();
		}
	}
	if (IsKeyTrigger('F'))
	{
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			m_pBlock[i] = nullptr;
		}
		float x = RandomFloat(-5.0f, 5.0f);
		float z = RandomFloat(-5.0f, 5.0f);
		int randIndex = rand() % Block::None; // or correct enum size
		Block::Block_Color randColor = static_cast<Block::Block_Color>(randIndex);
		m_pBlock[0] = new Block(m_pNextItem->Next(), x, z);
		m_pBlack[0]->GetCamera(m_pCamera);
		m_pScore->AddScore(10);
	}
	if (m_pTimer)
	{
		m_pTimer->Update();
	}

	if (m_pScore)
	{
		m_pScore->Update();
	}
}

void SceneGame::Draw()
{
	//--- １つ目の地面
	DirectX::XMMATRIX T;			// 位置情報を作成
	DirectX::XMMATRIX R;			// 回転情報を作成
	DirectX::XMMATRIX S;			// 拡縮を作成
	DirectX::XMMATRIX mat;		// それぞれを統合するマトリクスを作成
	DirectX::XMFLOAT4X4 fMat; // 行列の格納先

	// 　頂点シェーダーに渡す変換行列を作成
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world, view, proj;

	T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	R = DirectX::XMMatrixRotationY(0.0f);
	mat = S * R * T;
	world = mat;
	world = T;
	view = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	proj = DirectX::XMMatrixPerspectiveFovLH(
			(1.0f / 6.0f) * 3.1415f * 2.0f, // FovAngleY
			16.0f / 9,											// AspectRatio
			0.001f,													// NearZ
			10.0f														// FarZ
	);

	// 　計算用のデータから読み取り用のデータに変換
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));

	// モデルに変換行列を設定
	wvp[1] = m_pCamera->GetViewMatrix();
	wvp[2] = m_pCamera->GetProjectionMatrix();

	// 　シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp); // 　引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと

	Geometory::SetView(m_pCamera->GetViewMatrix(true));
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix(true));
	// Spriteへの設定
	Sprite::SetView(m_pCamera->GetViewMatrix(true));
	Sprite::SetProjection(m_pCamera->GetProjectionMatrix(true));

	// 　モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	// 　複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
	//  分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
	//  切り替える。
	if (false)
		for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
		{
			// モデルのメッシュを取得
			Model::Mesh mesh = *m_pModel->GetMesh(i);
			// メッシュに割り当てられているマテリアルを取得
			Model::Material material = *m_pModel->GetMaterial(mesh.materialID);
			// シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);
			// モデルの描画
			m_pModel->Draw(i);
		}

	if (m_pPlayer)
	{
		m_pPlayer->Draw();
		// DirectX::XMVECTOR A = DirectX::XMVectorSet(0.0f, 10.0f, -10.0f, 0.0);
		// DirectX::XMVECTOR P = DirectX::XMVectorSet(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, m_pPlayer->GetPos().z, 0.0f);
		// A = DirectX::XMVectorAdd(A, P);
		// m_pCamera->SetPos({A.m128_f32[0], A.m128_f32[1], A.m128_f32[2]});
		Collision::Info collisionA = m_pPlayer->GetCollision(); // プレイヤーの当たり判定

		// 地面、または障害物の当たり判定
		// ここでは先に設定していたスケールに合わせたボックスを地面として作成
		Collision::Info collisionB = {};
		collisionB.type = Collision::eBox;
		collisionB.box.center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // 地面の中心位置
		collisionB.box.size = DirectX::XMFLOAT3(3.0f, 0.2f, 5.0f);	 // 地面のサイズ（X, Y, Z）

		// collisionA、collisionBを使用して当たり判定の実行
		Collision::Result result = Collision::Hit(collisionA, collisionB);

		// 判定
		if (result.isHit)
		{
			result.other = collisionB;
			m_pPlayer->OnCollision(result); // プレイヤーの当たり判定発生時の処理

			// 地面、または障害物の当たり判定発生時の処理(必要に応じて)
			// ここではデバッグ用に当たったボックスを描画して可視化する例を行う
			// DirectX::XMMATRIX Tcol = DirectX::XMMatrixTranslation(
			//	collisionB.box.center.x,
			//	collisionB.box.center.y,
			//	collisionB.box.center.z);

			// DirectX::XMMATRIX Scol = DirectX::XMMatrixScaling(
			//	collisionB.box.size.x,
			//	collisionB.box.size.y,
			//	collisionB.box.size.z);

			// DirectX::XMMATRIX matCol = DirectX::XMMatrixTranspose(Scol * Tcol);
			// DirectX::XMFLOAT4X4 fMatCol;
			// DirectX::XMStoreFloat4x4(&fMatCol, matCol);
			// Geometory::SetWorld(fMatCol);
			// Geometory::DrawBox();
			//  必要ならここでサウンド再生やスコア処理、障害物側の反応などを追加
		}
	}
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (m_pBlock[i] != nullptr)
		{
			m_pBlock[i]->SetPlayerPos(m_pPlayer->GetPos());
			m_pBlock[i]->Draw();
		}
	}

	if (true)
	{
		//--- １つ目の地面
		T = DirectX::XMMatrixTranslation(0.0f, -.2f, 0.0f);						 // 天面がグリッドよりも下に来るように移動
		S = DirectX::XMMatrixScaling(10.0f * 2.0f, 0.2f, 6.0f * 2.0f); // 地面となるように、前後左右に広く、上下に狭くする
		mat = S * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 fMat; // 行列の格納先
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat); // ボックスに変換行列を設定
		Geometory::DrawBox();
	}

	if (m_pScore)
	{
		m_pScore->Draw();
	}

	if (m_pTimer)
	{
		m_pTimer->Draw();
	}

	if (m_pNextItem)
	{
		m_pNextItem->Draw();
	}
}

float RandomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}
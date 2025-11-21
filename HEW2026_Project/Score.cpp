#include "Score.h"
#include "Texture.h"
#include <Sprite.h>
#include "Defines.h"
#include <Input.h>

CScore::CScore()
	:m_pScoreTex(nullptr)
	,m_pGaugeTex(nullptr)
	,m_rate(0.1f)
	,m_targetScore(100)
	,m_currentScore(0)
{
	m_pScoreTex = new Texture();
	if (FAILED(m_pScoreTex->Create("Assets/Texture/Number.png")))
		MessageBox(NULL, "Texture load failed.Nunber.png", "Error", MB_OK);
}

CScore::~CScore()
{
	if (m_pScoreTex)
	{
		delete m_pScoreTex;
		m_pScoreTex = nullptr;
	}
}

void CScore::Update()
{}

void CScore::Draw()
{
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,										//左
		static_cast<float>(SCREEN_WIDTH),			//右
		0.0f,										//下
		static_cast<float>(SCREEN_HEIGHT),			//上
		0.0f,										//近クリップ
		1.0f										//遠クリップ
	);

	//転置してXMFLOAT4X4(GPU読み取り用)
	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));

	//スプライト描画用に設定
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	{//文字を出力するために必ず必要な処理
		//画面端からのパディング
		const float PADDING = 150.0f;

		//描画開始位置
		DirectX::XMFLOAT2 startPos;

		//X座標
		startPos.x = static_cast<float>(SCREEN_WIDTH) - PADDING;
		//Y座標
		startPos.y = static_cast<float>(SCREEN_HEIGHT) - 50.0f;

		DirectX::XMFLOAT2 digitSize = { 40.0f,60.0f };	//一桁サイズ
		const float DIGIT_SPACING = 5.0f;				//桁間のスペース

		std::string scoreStr = std::to_string(m_targetScore);
		int numDigits = scoreStr.length();

		for (int i = 0; i < numDigits; i++)
		{
			int number = scoreStr[i] - '0';

			//現在の桁の中心X座標を計算
			float currentX = startPos.x + i * (digitSize.x + DIGIT_SPACING) + digitSize.x * 0.5f;

			DrawDigit(number, { currentX,startPos.y }, digitSize);
		}
	}

	int score = m_currentScore;
	{//文字を出力するために必ず必要な処理
	//画面端からのパディング
		const float PADDING = 250.0f;

		//描画開始位置
		DirectX::XMFLOAT2 startPos;

		//X座標
		startPos.x = static_cast<float>(SCREEN_WIDTH) - PADDING;
		//Y座標
		startPos.y = static_cast<float>(SCREEN_HEIGHT)-50.0f;

		DirectX::XMFLOAT2 digitSize = { 40.0f,60.0f };	//一桁サイズ
		const float DIGIT_SPACING = 5.0f;				//桁間のスペース

		std::string scoreStr = std::to_string(score);
		int numDigits = scoreStr.length();

		for (int i = 0; i < numDigits; i++)
		{
			int number = scoreStr[numDigits - 1 - i] - '0';

			//桁位置を右から左へずらす
			float currentX = startPos.x - i * (digitSize.x + DIGIT_SPACING);

			DrawDigit(number, { currentX,startPos.y }, digitSize);
		}
	}
}

void CScore::AddScore(int amount)
{
	if (m_currentScore >= m_targetScore)
		MessageBox(NULL, "おめでとう！", "目標達成", MB_OK);
		m_currentScore += amount;
}

void CScore::DrawDigit(int number, DirectX::XMFLOAT2 centerPos, DirectX::XMFLOAT2 size)
{
	//UV座標の計算
	DirectX::XMFLOAT4 uvRect = CalculateUVForNumber(number);

	//UVPos(左上のUV座標)とUVScale（幅、高さ）を計算
	DirectX::XMFLOAT2 uvPos = { uvRect.x,uvRect.y };
	DirectX::XMFLOAT2 uvScale;
	uvScale.x = uvRect.z - uvRect.x;
	uvScale.y = uvRect.w - uvRect.y;

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(centerPos.x, centerPos.y, 0.0f);

	//Y軸反転スケーリング
	const float UP_DOWN_FLIP = -1.0f;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, UP_DOWN_FLIP, 1.0f);
	
	DirectX::XMMATRIX mWorld = T;

	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));

	Sprite::SetWorld(world);
	Sprite::SetTexture(m_pScoreTex);	//描画するテクスチャ
	Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });	//白固定

	//オフセットサイズの設定
	Sprite::SetOffset({0.0f,0.0f });
	Sprite::SetSize(size);

	//UV切り出し情報
	Sprite::SetUVPos(uvPos);
	Sprite::SetUVScale(uvScale);

	//描画実行
	Sprite::Draw();
}

DirectX::XMFLOAT4 CScore::CalculateUVForNumber(int number)
{
	int X = 0; // グリッドのX位置 (列)
	int Y = 0; // グリッドのY位置 (行)

	// 描画したい数字がテクスチャのどのグリッドにあるかを決定
	if (number >= 0 && number <= 7) {
		X = number % NUM_COLS; // 4で割った余りが列 (0, 1, 2, 3)
		Y = number / NUM_COLS; // 4で割った商が行 (0, 1)
	}
	else if (number == 8) {
		// 8: 3行目の1列目 (X=0, Y=2)
		X = 0;
		Y = 2;
	}
	else if (number == 9) {
		// 9: 3行目の2列目 (X=1, Y=2)
		X = 1;
		Y = 2;
	}
	else if (number == 10) {
		// 桁区切り用のカンマ (','): 3行目の3列目 (X=2, Y=2)
		// 描画ロジックに応じて、特別な値を渡してカンマを取得できるようにする
		X = 2;
		Y = 2;
	}
	else {
		// 範囲外の数字が渡された場合、エラー処理またはデフォルトの UV を返す
		return DirectX::XMFLOAT4(0.0f, 0.0f, GRID_W, GRID_H); // 0 の UV を返す
	}

	// --- UV座標の計算 (正規化座標) ---
	// U_min, V_min はグリッドの左上、U_max, V_max はグリッドの右下

	float U_min = X * GRID_W;
	float V_min = Y * GRID_H;
	float U_max = (X + 1) * GRID_W;
	float V_max = (Y + 1) * GRID_H;

	// { Umin, Vmin, Umax, Vmax } の形式で返す
	return DirectX::XMFLOAT4(U_min, V_min, U_max, V_max);
}

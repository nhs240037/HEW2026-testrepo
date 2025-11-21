#include "Timer.h"
#include "Texture.h"
#include <Sprite.h>
#include "Defines.h"
#include <sstream>
#include <iomanip>
#include <cmath>

constexpr int NUM_COLS = 4; // Number.png の数字の列数
constexpr float GRID_W = 1.0f / NUM_COLS; // 正規化U座標の幅 
constexpr float GRID_H = 1.0f / 3.0f;     // 正規化V座標の高さ

CTimer::CTimer()
	:m_pTimerTex(nullptr)
{
	InitializeTimer();
	m_pTimerTex = new Texture();
	if (FAILED(m_pTimerTex->Create("Assets/Texture/Number.png")))
		MessageBox(NULL, "Texture load failed.Nunber.png", "Error", MB_OK);

}

CTimer::~CTimer()
{
	if (m_pTimerTex)
	{
		delete m_pTimerTex;
		m_pTimerTex = nullptr;
	}
}

void CTimer::InitializeTimer()
{
	//CPUの周波数を取得
	QueryPerformanceFrequency(&m_cpuFrequency);
	//最初の時刻を取得
	QueryPerformanceCounter(&m_lastTime);

	m_totalTime = START_TIME_SECONDS;
}

void CTimer::Update()
{
	// 時間計測ロジック
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	LONGLONG elapsedTicks = currentTime.QuadPart - m_lastTime.QuadPart;
	double deltaTime = (double)elapsedTicks / m_cpuFrequency.QuadPart;

	m_totalTime -= deltaTime;
	if (m_totalTime < 0.0f)
	{
		m_totalTime = 0.0;
		MessageBox(NULL, "GAMEOVER", "Gameover", MB_OK);
	}

	m_lastTime = currentTime;
}

void CTimer::Draw()
{
	DirectX::XMMATRIX mView = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(SCREEN_WIDTH),
		0.0f,
		static_cast<float>(SCREEN_HEIGHT),
		0.0f,
		1.0f
	);
	DirectX::XMFLOAT4X4	view, proj;
	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	std::string timerStr = TimeToString(m_totalTime);

	// タイマー描画の開始位置 (例: 画面左上に表示)
	{
		const float PADDING_X = 50.0f;
		const float PADDING_Y = 50.0f;

		DirectX::XMFLOAT2 startPos;
		startPos.x = PADDING_X;
		startPos.y = static_cast<float>(SCREEN_HEIGHT) - PADDING_Y;

		DirectX::XMFLOAT2 digitSize = { 40.0f,60.0f };
		const float DIGIT_SPACING = 5.0f;

		int numDigits = timerStr.length();

		for (int i = 0; i < numDigits; i++)
		{
			char c = timerStr[i];
			int number = -1;

			// 文字に応じてインデックスを決定
			if (c >= '0' && c <= '9')
			{
				number = c - '0';
			}
			else if (c == ':')
			{
				number = 11; // コロン用のインデックス
			}
			else if (c == '.')
			{
				number = 11; // ピリオド用のインデックス
			}

			float currentX = startPos.x + i * (digitSize.x + DIGIT_SPACING) + digitSize.x * 0.5f;

			if (number != -1)
			{
				DrawDigit(number, { currentX,startPos.y }, digitSize);
			}
		}
	}
}

void CTimer::DrawDigit(int number, DirectX::XMFLOAT2 centerPos, DirectX::XMFLOAT2 size)
{
	// DrawDigit の内容は CScore のものと全く同じロジックです。
	// CalculateUVForNumber を呼び出し、Sprite::Draw を実行します。
	DirectX::XMFLOAT4 uvRect = CalculateUVForNumber(number);

	DirectX::XMFLOAT2 uvPos = { uvRect.x,uvRect.y };
	DirectX::XMFLOAT2 uvScale;
	uvScale.x = uvRect.z - uvRect.x;
	uvScale.y = uvRect.w - uvRect.y;

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(centerPos.x, centerPos.y, 0.0f);

	const float UP_DOWN_FLIP = -1.0f;
	DirectX::XMMATRIX mWorld = T;

	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));

	Sprite::SetWorld(world);
	Sprite::SetTexture(m_pTimerTex);	// Timer のテクスチャを設定
	Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });

	Sprite::SetOffset({ 0.0f,0.0f });
	Sprite::SetSize(size);

	Sprite::SetUVPos(uvPos);
	Sprite::SetUVScale(uvScale);

	Sprite::Draw();
}

std::string CTimer::TimeToString(double timeInSeconds)
{
	// 時間を分解
	int totalMillis = static_cast<int>(timeInSeconds * 1000);

	int minutes = totalMillis / 60000;
	int seconds = (totalMillis % 60000) / 1000;
	int millis = (totalMillis % 1000) / 10; // 10ミリ秒単位 (下2桁)

	std::stringstream ss;

	ss << std::setw(2) << std::setfill('0') << minutes;
	ss << ":";
	ss << std::setw(2) << std::setfill('0') << seconds;
	ss << ".";
	ss << std::setw(2) << std::setfill('0') << millis;

	return ss.str();
}

DirectX::XMFLOAT4 CTimer::CalculateUVForNumber(int number)
{
	int X = 0;
	int Y = 0;

	if (number >= 0 && number <= 7) {
		X = number % NUM_COLS;
		Y = number / NUM_COLS;
	}
	else if (number == 8) {
		X = 0; Y = 2;
	}
	else if (number == 9) {
		X = 1; Y = 2;
	}
	else if (number == 10) { // コロン ':'
		X = 2; Y = 2;
	}
	else if (number == 11) { // ピリオド '.'
		X = 3; Y = 2;
	}
	else {
		return DirectX::XMFLOAT4(0.0f, 0.0f, GRID_W, GRID_H);
	}

	float U_min = X * GRID_W;
	float V_min = Y * GRID_H;
	float U_max = (X + 1) * GRID_W;
	float V_max = (Y + 1) * GRID_H;

	return DirectX::XMFLOAT4(U_min, V_min, U_max, V_max);
}

#pragma once
#include <windows.h>
#include <string>
#include "Texture.h"
#include <DirectXMath.h>

class CTimer
{
public:
	CTimer();
	~CTimer();

	void InitializeTimer();
	virtual void Update();
	virtual void Draw();
protected:
	//経過時間計測用
	LARGE_INTEGER m_cpuFrequency;
	LARGE_INTEGER m_lastTime;
	double m_totalTime = 0.0;	//合計経過時間(秒)
	double m_limitTime = 3 * 60.0;	//制限時間

	//テクスチャと描画設定
	Texture* m_pTimerTex;		//テクスチャ

	//ヘルパー関数
	std::string TimeToString(double timeInSeconds);
	DirectX::XMFLOAT4 CalculateUVForNumber(int number);
	void DrawDigit(int number, DirectX::XMFLOAT2 centerPos, DirectX::XMFLOAT2 size);
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// \file   Transfer.h
// \brief  ImGuiに値を飛ばすためのクラス
//		   シングルトンで構成
//		   表示させたい情報をメンバ変数に追加
//		   追加する変数はすべてpublicに(ゲッターセッターの設定がめんどいだけ)
// 
// \author 山本郁也
// \date   December 2025
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#pragma once

// インスタンス取得マクロ:tranでアクセス
#define TRAN_INS Transfer& tran = Transfer::GetInstance();
#define _TRANSFER

#include <vector>

class Transfer
{
private:
	Transfer() = default;
	~Transfer() = default;

public:
	Transfer(const Transfer&) = delete;
	Transfer& operator=(const Transfer&) = delete;
public:
	static Transfer& GetInstance()
	{
		static Transfer ins;
		return ins;
	}
	// 初期化処理:大体0.0f初期化->各値設定を行ってください
	void Init()
	{
		// プレイヤー
		player.pos = { 0.0f,0.0f };
		player.maxSpeed = { 5.0f ,5.0f};
		player.velocity = 0.01f;
		player.speedDown = 0.99f;
		// カメラ
		camera.eyePos = { 0.0f,110.0f,-110.0f };
		camera.lookPos = {0.0f,0.0f,0.0f};
		// ステージ
		stage.column = 12;
		stage.row = 6;
		// 食材
		item.downSpeed = 0.1f;
		item.size = { 1.0f,1.0f,1.0f };
		item.repopTime = 2.0f;
		// オーダー
		order.size = { 1.0f,1.0f };
		order.TimeLimit = 180.0f;	//三分
		order.repopTime = 10.0f;
		order.金額.push_back(100.0f);
		// UI
		ui.pos = { 0.0f,0.0f };
		ui.size = { 1.0f,1.0f };
		ui.color = { 1.0f,1.0f,1.0f,1.0f };
	}
public:
	struct f2
	{
		float x;
		float y;
	};
	struct f3
	{
		float x;
		float y;
		float z;
	};
	struct RGBa
	{
		float r;
		float g;
		float b;
		float a;
	};
	struct n2
	{
		int x;
		int y;
	};
private:
	struct PlayerInfo
	{
		f2 pos;				// 位置
		f2 maxSpeed;		// 最大速度
		float velocity;		// 加速度
		float speedDown;	// 減速
	};
	struct CameraInfo
	{
		f3 eyePos;	// カメラ位置
		f3 lookPos;	// 注視点
	};
	struct StageInfo
	{
		int column;	//ステージの横の幅
		int row;	//ステージの縦の幅
	};
	struct ItemInfo
	{
		float downSpeed;	// 食材の落下速度
		f3 size;			// 食材のサイズ
		float repopTime;	// 食材の生成頻度(生成までのフレーム数 default:60)
	};
	struct OrderInfo
	{
		f2 size;			// オーダーサイズ
		float TimeLimit;	// オーダー制限時間
		float repopTime;	// オーダー頻度
		std::vector<float> 金額;			// 各オーダーの値段
	};
	struct UiInfo
	{
		f2 pos;
		f2 size;
		RGBa color;
	};
public:// 各数値
	PlayerInfo player;	// プレイヤーの位置情報
	CameraInfo camera;	// カメラの位置情報
	StageInfo stage;	// ステージの各情報
	ItemInfo item;		// 食材の各情報
	OrderInfo order;	// オーダーの設定
	UiInfo ui;			// え？
};


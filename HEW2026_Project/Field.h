#pragma once

#include"Geometory.h"
#include"Defines.h"
#include<DirectXMath.h>
#include"CsvData.h"
#include"Model.h"
#include<string>
#include "Camera.h"
#include"CameraDebug.h"
#include "Sprite.h"

#define MAX_FIELD_WIDTH		(20)
#define MAX_FIELD_HEIGHT	(12)

// フィールドのモデルを静的に読み込むためシングルトンで管理し
// 随時参照できるようにする
class Field_Model
{
private:
	Field_Model();
	~Field_Model();
private:
	struct f2
	{
		float x;
		float z;
	};
	struct Field_wh
	{
		float width;	// フィールドの横幅
		float height;	// フィールドの奥行
	};
	struct Field_Wall
	{
		f2 back;	// フィールドの奥の壁の位置
		f2 front;	// フィールドの手前の壁の位置
		f2 left;	// フィールドの左の壁の位置
		f2 right;	// フィールドの右の壁の位置
	};
	struct Field_State
	{
		Field_wh widhei;// フィールドの横幅と奥行を管理
		Field_Wall wall;// フィールドの四方の壁の位置を管理
	};
	struct Field_Model_Type
	{
		Model* Green_Top;		// 緑 縁なし
		Model* Green_Corner;	// 緑 端縁
		Model* Green_Side;		// 緑 横縁
		Model* White_Top;		// 白 縁なし
		Model* White_Corner;	// 白 端縁
		Model* White_Side;		// 白 横縁
	};
public:
	enum Field_Model_No
	{
		GT,GC,GS,
		WT,WC,WS,
	};
private :
	Field_State m_field_s;
	Model* m_pModel; // あってないようなもの
	Field_Model_Type m_f_m_n;
public:
	static Field_Model& get_instance();
	Field_State GetFieldState();
	Model* GetFieldModel();
	Model* GetFieldModel(Field_Model_No get);
	Model* GetFieldModel(int get);
};

class Field
{
public:
	Field();
	~Field();
	void Update();
	void Draw();

	void SetCamera(Camera *set);
private:
	Field_Model& m_field_model;
	CsvData& csv;
	DirectX::XMMATRIX m_pos;
	Model* m_pModel[6];
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX m_dxpos;
	Camera* m_pCamera;
public:
	Model* m_pModel_G_Top;
	Model* m_pModel_G_Corner;
	Model* m_pModel_G_Side;
	Model* m_pModel_W_Top;
	Model* m_pModel_W_Corner;
	Model* m_pModel_W_Side;
};

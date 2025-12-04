#include "Player.h"
#include"Defines.h"
#include"Main.h"
#include"Sprite.h"
#include "ShaderList.h"

#include"CsvData.h"
#include"Sound.h"

#define PRE(in) IsKeyPress(in)


Player::Index PosToIndex(Player::float2 f2pos);// インデックス座標からフィールド上のざひょうに変換
Player::float2 IndexToPos(Player::Index idx);// フィールド上の座標から インデックス座標に変換

enum eGolfBallShotStep {
	SHOT_WAIT,		// 球を打つのを待つ
	SHOT_KEEP,		// キー入力開始
	SHOT_RELEASE,	// キー入力をやめた（球を打つ
};


Player::Player()
	: m_pCamera(nullptr)
	, m_isStop(false)
	, m_shotStep(SHOT_WAIT)
	, m_power(0.0f)
	, m_move()
	, m_f2Velocity{ 0.0f,0.0f }
	, m_idx{}
	, m_angle(0.0f)
	, m_shadowPos{ 0.0f,0.0f,0.0f }
	, csv(CsvData::get_instance())
	, m_pModel(nullptr)
	, tran(Transfer::GetInstance())
{
	m_collision.type = Collision::eBox;
	m_collision.box = {
		m_pos, DirectX::XMFLOAT3(m_collision.box.size)
	};
	m_pShadowTex = new Texture();
	if (FAILED(m_pShadowTex->Create("Assets/Texture/Shadow.png"))) 
	{
		MessageBox(NULL, "Texture load failed.\nPlayer.cpp", "Error", MB_OK);
	}

	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/Prototype/MD_Player.fbx", 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
	}

	DirectX::XMMATRIX view, proj;
	m_dxpos = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_dxpos *= DirectX::XMMatrixScaling(
		csv.GetPlayerState().size.x,
		csv.GetBlockState().height,
		csv.GetPlayerState().size.y);

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	proj = DirectX::XMMatrixPerspectiveFovLH
	(
		(1.0f / 6.0f) * 3.1415f * 2.0f,	// FovAngleY
		16.0f / 9,	// AspectRatio
		0.001f,	// NearZ
		10.0f	// FarZ
	);
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
}

Player::~Player()
{
	if (m_pShadowTex) {
		delete m_pShadowTex;
		m_pShadowTex = nullptr;
	}
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

// プレイヤーの動きを更新するための関数
Player::float2 PlayerMoveGrid(Player::float2 pos)
{
	static bool isPress;
	static int nPressCount;
	if (IsKeyPress(VK_LEFT) ||
		IsKeyPress(VK_RIGHT) ||
		IsKeyPress(VK_UP) ||
		IsKeyPress(VK_DOWN))
	{
		if (isPress)
		{
			nPressCount++;
			if (nPressCount > 5)
			{
				nPressCount = 0;
			}
			else
				return pos;
		}
		else
			isPress = true;
	}
	if (IsKeyPress('A'))
	{
		pos.x -= FIELD_WIDTH - (IsKeyPress(VK_SHIFT) * -FIELD_WIDTH);
	}
	if (IsKeyPress('D'))
	{
		pos.x += FIELD_WIDTH + (IsKeyPress(VK_SHIFT) * FIELD_WIDTH);
	}
	if (IsKeyPress('W'))
	{
		pos.y += FIELD_HEIGHT + (IsKeyPress(VK_SHIFT) * FIELD_WIDTH);
	}
	if (IsKeyPress('S'))
	{
		pos.y -= FIELD_HEIGHT - (IsKeyPress(VK_SHIFT) * -FIELD_WIDTH);
	}
	return pos;
}

Player::float2 PlayerMoveSmooth(Player::float2 pos)
{
	CsvData& csv = CsvData::get_instance();
	TRAN_INS

	static bool isPress;
	static int nPressCount;
	if (IsKeyPress(VK_LEFT) ||
		IsKeyPress(VK_RIGHT) ||
		IsKeyPress(VK_UP) ||
		IsKeyPress(VK_DOWN))
	{
		if (isPress)
		{
			nPressCount++;
			if (nPressCount > 5)
			{
				nPressCount = 0;
			}
			else
				return pos;
		}
		else
			isPress = true;
	}
	if (IsKeyPress('A'))
	{
		pos.x -= tran.player.velocity;
	}
	if (IsKeyPress('D'))
	{
		pos.x += tran.player.velocity;
	}
	if (IsKeyPress('W'))
	{
		pos.y += tran.player.velocity;
	}
	if (IsKeyPress('S'))
	{
		pos.y -= tran.player.velocity;
	}
	return pos;
}

void Player::Update()
{
	// カメラが設定されてない場合は処理しない
	if (!m_pCamera) { return; }

	m_collision.box.center = m_pos;	// 更新処理後に当たり判定の位置を更新

	TRAN_INS;
	m_pos.x = tran.player.pos.x;
	m_pos.z = tran.player.pos.y;

#ifdef _DEBUG
	// 回転処理
	if (IsKeyPress('Q'))
	{
		m_angle -= csv.GetSpeed();
	}
	if (IsKeyPress('E'))
	{
		m_angle += csv.GetSpeed();
	}
	if (IsKeyPress('C'))
	{
		m_move.y -= csv.GetSpeed();
	}
	if (IsKeyPress('X'))
	{
		m_move.y += csv.GetSpeed();
	}

#endif
	UpdateControl();
	UpdateMove();	// 摩擦の処理
	UpdateWall();

#ifdef _DEBUG
	tran.player.pos.x = m_pos.x;
	tran.player.pos.y = m_pos.z;
#endif
}

void Player::Draw()
{
	Texture* g_pMyTexture = new Texture();
	HRESULT hr = g_pMyTexture->Create("Assets/Texture/white.png"); // ← 読み込みたい画像
	if (FAILED(hr))
	{
		// エラー処理
	}
	DirectX::XMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f };
	Sprite::SetColor(color);
	DirectX::XMMATRIX T; // 天面がグリッドよりも下に来るように移動
	DirectX::XMMATRIX S; // 地面となるように、前後左右に広く、上下に狭くする
	DirectX::XMMATRIX mat;


	T = DirectX::XMMatrixTranslation(m_pos.x, .25f, m_pos.z); // 天面がグリッドよりも下に来るように移動
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_angle);
	S = DirectX::XMMatrixScaling(FIELD_WIDTH, 0.5f, FIELD_WIDTH); // 地面となるように、前後左右に広く、上下に狭くする
	mat = S * Ry * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat; // 行列の格納先
	DirectX::XMStoreFloat4x4(&fMat, mat);

	//---------- ここから影の描画 ----------//

	float rate = (m_pos.y - m_shadowPos.y) / METER(4.0f);	// 距離が近ければ0,遠ければ1
	float scale = (1.0f - rate);								// rateを0なら1、1なら0になるよう反転
	m_shadowPos.x = m_pos.x;
	m_shadowPos.z = m_pos.z;
	Sprite::SetSize(DirectX::XMFLOAT2{ scale,0.0f});
	// 影を表示するための行列計算
	DirectX::XMMATRIX Sprite_S = DirectX::XMMatrixScaling(scale, scale, scale);	// scaleを元に拡縮
	DirectX::XMMATRIX Sprite_Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-90.0f)); 	// スプライトを横に倒す
	DirectX::XMMATRIX Sprite_T = DirectX::XMMatrixTranslation(		// Zファイティング回避で少し浮かす
		m_pos.x, 0.0f, m_pos.z);
	DirectX::XMMATRIX Sprite_mat = Sprite_S * Sprite_Rx * Sprite_T;	// それぞれの数値をマージ
	Sprite_mat = DirectX::XMMatrixTranspose(mat);// 行列変換を行う
	DirectX::XMFLOAT4X4 Sprite_fMat; // 描画専用変数を定義
	DirectX::XMStoreFloat4x4(&Sprite_fMat, Sprite_mat);//mWorldを転置してmatに格納

	// 場所を指定
	m_dxpos = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(
			(IsKeyPress('D') * -90.0f) + (IsKeyPress('A') * 90.0f) + 
			(IsKeyPress('W') * 180.0f) + (IsKeyPress('S') * 0.0f)
		))
		* DirectX::XMMatrixTranslation(m_pos.x, 0.5f, m_pos.z);

	//　計算用のデータから読み取り用のデータに変換
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));

	// モデルに変換行列を設定
	wvp[1] = m_pCamera->GetViewMatrix();
	wvp[2] = m_pCamera->GetProjectionMatrix();

	//　シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp);	//　引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと


	Geometory::SetView(m_pCamera->GetViewMatrix(true));
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix(true));
	// Spriteへの設定
	Sprite::SetView(m_pCamera->GetViewMatrix(true));
	Sprite::SetProjection(m_pCamera->GetProjectionMatrix(true));

	//　モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	//　複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
	// 分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
	// 切り替える。
	for (int i = 0; i < m_pModel->GetMeshNum(); ++i) 
	{
		// モデルのメッシュを取得
		Model::Mesh mesh = *m_pModel->GetMesh(i);
		// メッシュに割り当てられているマテリアルを取得
		Model::Material	material = *m_pModel->GetMaterial(mesh.materialID);
		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);
		// モデルの描画
		m_pModel->Draw(i);
	}
}


void Player::SetCamera(Camera *camera)
{
	m_pCamera = camera;
}

void Player::UpdateShot()
{
	switch (m_shotStep) {
		// 球の打ちはじめ
	case SHOT_WAIT:
		if (IsKeyTrigger('Z')) {
			m_power = 0.0f;
			m_shotStep = SHOT_KEEP;
		}
		break;


		// キー入力継続中
	case SHOT_KEEP:
		m_power += 0.1f;	 // 球を打ち出す力を溜める
		if (IsKeyRelease('Z')) {
			m_shotStep = SHOT_RELEASE;
		}
		break;

		// 球を打つ
	case SHOT_RELEASE:
	{
		// 打ち出す計算
		DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();						//カメラの位置を取得
		DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);			//カメラの位置を計算用の型に変換
		DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&m_pos);				//自分の位置を計算用の型に変換
		DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vPos, vCamPos);	//カメラから自分の位置に向かうベクトルを計算
		vec = DirectX::XMVector3Normalize(vec);		//ベクトルの正規化
		vec = DirectX::XMVectorScale(vec, m_power);		//正規化したベクトルを、溜めた力に応じて伸ばす
		DirectX::XMStoreFloat3(&m_move, vec);//移動のデータm_moveに計算したvecを設定する（計算用の型から保存用の型に変換;
		
		// 打ち出し後の情報を設定
		m_isStop = false;			// 移動している！
		m_shotStep = SHOT_WAIT;		// また再び打つ
		break;
	}
	}
}

void Player::UpdateControl()
{
	if (IsKeyPress('A'))
	{
		m_move.x -= tran.player.velocity;
	}
	if (IsKeyPress('D'))
	{
		m_move.x += tran.player.velocity;
	}
	if (IsKeyPress('W'))
	{
		m_move.z += tran.player.velocity;
	}
	if (IsKeyPress('S'))
	{
		m_move.z -= tran.player.velocity;
	}

	// Pressなので連続入力時に特定フレーム毎(20)にのみ反応する処理
	static int pressCount;
	if ((PRE('W') || PRE('A') || PRE('S') || PRE('D')) && pressCount <= 0)
	{
		pressCount = 20;
		SE_INS_So.PlaySE(4);
	}
	if (pressCount > 0)
	{
		pressCount--;
	}

}

void Player::UpdateMove()
{
	// Transferのインスタンスを取得
	TRAN_INS


	// 移動処理
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	// 減速処理(空気抵抗
	m_move.x *= tran.player.speedDown;
	m_move.y *= tran.player.speedDown;
	m_move.z *= tran.player.speedDown;

	//この作品の場合はこれより下の処理は不要なのでスキップ
	return;
	// 重力
	m_move.y -= MSEC(GRAVITY);

	// 地面接触判定
	if (m_pos.y < 0.0f) {
		// 接触時の減速処理
		m_move.x *= 0.95f;
		m_move.y *= 0.5f;
		m_move.z *= 0.95f;

		// バウンド処理
		m_move.y = -m_move.y; // Yの移動方向を反転
		if (m_move.y < CMETER(5.0f)) {	// バウンドが小さいか判定
			m_move.y = 0.0f;
			m_pos.y = 0.0f;
		}
		else {
			// 地面にめり込んでいるので、バウンドした場合の位置に変更
			m_pos.y = -m_pos.y;
		}
	}
	// 停止判定
	float speed;
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);//移動情報を計算用の型に変換
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vMove);//vMoveから移動量を計算
	DirectX::XMStoreFloat(&speed, vLen);		// speedにvLenを格納
	if (speed < CMSEC(30.0f)) 
	{	// 1秒間に30cmぐらい進むスピードであれば停止
		m_isStop = true;
		m_shotStep = SHOT_WAIT;
	}
}

void Player::UpdateWall()
{
	if (m_pos.x > MAX_FIELD_WIDTH - HALF(PLAYER_WIDTH))
	{
		m_pos.x = MAX_FIELD_WIDTH - HALF(PLAYER_WIDTH);
		m_move.x = 0.0f;
	}
	if (m_pos.x < -MAX_FIELD_WIDTH + HALF(PLAYER_WIDTH))		
	{
		m_pos.x = -MAX_FIELD_WIDTH + HALF(PLAYER_WIDTH);
		m_move.x = 0.0f;
	}
	if (m_pos.z > MAX_FIELD_HEIGHT - HALF(PLAYER_WIDTH))		
	{
		m_pos.z = MAX_FIELD_HEIGHT - HALF(PLAYER_WIDTH);
		m_move.z = 0.0f;
	}
	if (m_pos.z < -MAX_FIELD_HEIGHT + HALF(PLAYER_WIDTH))	
	{
		m_pos.z = -MAX_FIELD_HEIGHT + HALF(PLAYER_WIDTH);
		m_move.z = 0.0f;
	}

}

Player::float2 IndexToPos(Player::Index idx)
{
	Player::float2 f2pos;
	f2pos.x = idx.x * FIELD_WIDTH + HALF(FIELD_WIDTH);
	f2pos.y = idx.z * FIELD_HEIGHT + (FIELD_HEIGHT);
	return f2pos;
}

Player::Index PosToIndex(Player::float2 pos)
{
	Player::Index idx;
	idx.x = pos.x / FIELD_WIDTH;
	idx.z = pos.y / FIELD_HEIGHT;
	return idx;
}

void Player::OnCollision(Collision::Result collision)
{
	// 計算に必要な情報を事前に計算
	DirectX::XMVECTOR vHitPos = DirectX::XMLoadFloat3(&collision.point);
	DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&collision.normal);
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
	vNormal = DirectX::XMVector3Normalize(vNormal);

	// 反射の計算
	DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vNormal, vMove);
	vDot = DirectX::XMVectorScale(vDot, 2.0f);
	vDot = DirectX::XMVectorMultiply(vNormal, DirectX::XMVectorAbs(vDot));
	vMove = DirectX::XMVectorAdd(vMove, vDot);
	DirectX::XMStoreFloat3(&m_move, vMove);

	// 反射後の補正
	if (collision.other.type == Collision::eBox) {
		// ボックスに衝突した場合、衝突位置の補正
		Collision::Box other = collision.other.box;
		if (collision.normal.x != 0.0f)
			m_pos.x =
			other.center.x + collision.normal.x * (other.size.x + collision.other.box.size.x) * 0.5f;
		else if (collision.normal.y != 0.0f)
			m_pos.y =
			other.center.y + collision.normal.y * (other.size.y + collision.other.box.size.y) * 0.5f;
		else
			m_pos.z =
			other.center.z + collision.normal.z * (other.size.z + collision.other.box.size.z) * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.8f;
		m_move.y *= 0.6f;
		m_move.z *= 0.8f;
	}
	else {
		// 斜面に衝突した場合の位置の補正
		m_pos.x = collision.point.x + collision.normal.x * collision.other.box.size.x * 0.5f;
		m_pos.y = collision.point.y + collision.normal.y * collision.other.box.size.y * 0.5f;
		m_pos.z = collision.point.z + collision.normal.z * collision.other.box.size.z * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.2f;
		m_move.y *= 0.5f;
		m_move.z *= 0.2f;
	}
}

void Player::SetShadow(DirectX::XMFLOAT3 pos)
{

	m_shadowPos = pos;
}

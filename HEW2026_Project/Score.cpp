#include "Score.h"
#include "Texture.h"
#include <Sprite.h>
#include "Defines.h"

CScore::CScore()
	:m_pScoreTex(nullptr)
	,m_pGaugeTex(nullptr)
	,m_rate(0.1f)
{
	m_pScoreTex = new Texture();
	if (FAILED(m_pScoreTex->Create("Nunber.png")))
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
{
}

void CScore::Draw()
{
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f),				//カメラの位置(z軸手前)
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),				//注視点(原点)
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)				//上方向(Y軸)
	);
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,										//左
		static_cast<float>(SCREEN_WIDTH),			//右
		static_cast<float>(SCREEN_HEIGHT),						//下
		0.0f,										//上
		0.0f,										//近クリップ
		1.0f										//遠クリップ
	);

	//転置してXMFLOAT4X4(GPU読み取り用)
	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));

	//スプライト描画用に設定
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	//描画位置
	DirectX::XMFLOAT2 pos = { 0.0f,0.0f };
	DirectX::XMFLOAT2 size[] = {
		{210.0f,40.0f},{200.0f,30.0f}		//フレームを大きめに設定
	};

	Texture* pTex[] = { m_pScoreTex };

	for (int i = 0; i < 2; i++)
	{
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pos.x - size[i].x * 0.5f, pos.y, 0.0f);
		DirectX::XMMATRIX S;
		if (i == 0)
			S = DirectX::XMMatrixScaling(1.5f, 1.0f, 1.0f);	//1.5倍で上下反転
		DirectX::XMMATRIX mWorld = S * T;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));
		Sprite::SetWorld(world);						//スプライトのワールド行列を設定
		Sprite::SetSize(size[i]);						//スプライトのサイズを設定
		Sprite::SetOffset({0.0f,0.0f });	//スプライトの原点を変更
		Sprite::SetTexture(m_pScoreTex);					//テクスチャの設定
		Sprite::Draw();
	}
}
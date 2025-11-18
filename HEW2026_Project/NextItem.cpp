#include "NextItem.h"
#include <algorithm>
#include <random>
#include "Defines.h"

NextItem::NextItem()
	:m_pFrameTex(nullptr), m_pItemIconTex{ nullptr }
{
	m_pFrameTex = new Texture();
	if (FAILED(m_pFrameTex->Create("Assets/Texture/Frame.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	m_pItemIconTex[0] = new Texture();
	if (FAILED(m_pItemIconTex[0]->Create("Assets/Texture/Item.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	m_pItemIconTex[1] = new Texture();
	if (FAILED(m_pItemIconTex[1]->Create("Assets/Texture/Item_0.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	m_pItemIconTex[2] = new Texture();
	if (FAILED(m_pItemIconTex[2]->Create("Assets/Texture/Item_1.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	m_pItemIconTex[3] = new Texture();
	if (FAILED(m_pItemIconTex[3]->Create("Assets/Texture/Item_2.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	m_pItemIconTex[4] = new Texture();
	if (FAILED(m_pItemIconTex[4]->Create("Assets/Texture/Item_3.png")))
		MessageBox(NULL, "Texture load failed.\nFrame.png", "Error", MB_OK);
	
	AddLootTable();
}

NextItem::~NextItem()
{
	SAFE_DELETE(m_pFrameTex);
	SAFE_DELETE_ARRAY(*m_pItemIconTex);
}

void NextItem::Draw()
{
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMVECTOR eye, fcs, up;
	eye = DirectX::XMVectorSet(0.f, 0.f, -5.f, 0.f);
	fcs = DirectX::XMVectorSet(0.f, .5f, 0.f, 0.f);
	up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMMATRIX mView =
		DirectX::XMMatrixLookAtLH(eye, fcs, up);
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f, 0.001f, 1000.f );

	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));

	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	// フレームとゲージの描画
	DirectX::XMFLOAT2 pos = { 1200.f, 270.f };
	DirectX::XMFLOAT2 size[] = {
		{80.f, 80.f}, {80.0f, 80.0f}		// フレームを大きめに設定
	};
	Texture* pTex[] = { m_pFrameTex, m_pItemIconTex[0]};
	for (int i = 0; i < 2; ++i) {
		//--- 中心に合わせる
		DirectX::XMMATRIX T =	DirectX::XMMatrixTranslation(pos.x - size[i].x * 0.5f, pos.y, 0.0f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.f, 1.0f);
		DirectX::XMMATRIX mWorld = S * T;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));

		Sprite::SetWorld(world);												// スプライトのワールド行列を設定
		Sprite::SetSize(size[i]);												// スプライトのサイズを設定
		Sprite::SetOffset({ size[i].x * 0.5f, 0.0f });	// スプライトの原点を変更
		Sprite::SetTexture(pTex[i]);										// テクスチャを設定
		Sprite::Draw();
	}
}

void NextItem::AddLootTable()
{
	std::vector<Item> arr{ Item(0),Item(1),Item(2),Item(3),Item(4)};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(arr.begin(), arr.end(), gen);
	m_Queue.insert(m_Queue.end(), arr.begin(), arr.end());
}

void NextItem::Next()
{
	//次送り
}
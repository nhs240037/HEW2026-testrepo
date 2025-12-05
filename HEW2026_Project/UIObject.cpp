/*********************************************************************
 * \file   UIObject.cpp
 * \brief  UIオブジェクトの基底クラス Base class of UI Object.
 *
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-18
 *********************************************************************/
//=====| Includes |=====//
#include "UIObject.h"
#include "Defines.h"
#include <cmath>
#include <algorithm>

namespace std
{
template <typename T>
T clamp(const T& v, const T& lo, const T& hi)
{
	return (v < lo) ? lo : (v > hi) ? hi : v;
}
}

UIObject::UIObject() : UIObject("Placeholder.png") {};
UIObject::UIObject(std::string RelativeTexturePathFromTextureFolder) : UIObject(RelativeTexturePathFromTextureFolder, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f) {};
UIObject::UIObject(float PositionX, float PositionY) : UIObject("Placeholder.png", PositionX, PositionY) {};
UIObject::UIObject(float PositionX, float PositionY, float Width, float Height) : UIObject("Placeholder.png", PositionX, PositionY, Width, Height) {};
UIObject::UIObject(float PositionX, float PositionY, float Width, float Height, float RotationX, float RotationY, float RotationZ) : UIObject("Placeholder.png", PositionX, PositionY, Width, Height, RotationX, RotationY, RotationZ) {};
UIObject::UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY) : UIObject(RelativeTexturePathFromTextureFolder, PositionX, PositionY, 100.f, 100.f) {};
UIObject::UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY, float Width, float Height) : UIObject(RelativeTexturePathFromTextureFolder, PositionX, PositionY, Width, Height, 0.f, 0.f, 0.f) {};

UIObject::UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY, float Width, float Height, float RotationX, float RotationY, float RotationZ): m_pTexture(nullptr)
{
	m_fPosition = {PositionX, PositionY};
	m_fSize = {Width, Height};
	m_fRotation = {RotationX, RotationY, RotationZ};
	m_fUVPositon = { 0.0f, 0.0f };
	m_fUVScale = { 1.0f, 1.0f };
	m_fColor[0] = {1.0f, 1.0f, 1.0f, 1.0f};
	m_fColor[1] = {1.0f, 1.0f, 1.0f, 1.0f};
	m_fColor[2] = {1.0f, 1.0f, 1.0f, 1.0f};
	m_fColor[3] = {1.0f, 1.0f, 1.0f, 1.0f};

	std::string TexturePath = "Assets/Texture/" + RelativeTexturePathFromTextureFolder;
	std::string FailedMsg = "Texture load failed.\n"+TexturePath;

	m_pTexture = new Texture();
	if(FAILED(m_pTexture->Create(TexturePath.c_str())))
		MessageBox(NULL, FailedMsg.c_str(), "Error", MB_OK);
};

UIObject::~UIObject()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void UIObject::SetTexture(std::string RelativeTexturePathFromTextureFolder)
{
	std::string TexturePath = "Assets/Texture/" + RelativeTexturePathFromTextureFolder;
	std::string FailedMsg = "Texture load failed.\n" + TexturePath;

	SAFE_DELETE(m_pTexture);

	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(TexturePath.c_str())))
		MessageBox(NULL, FailedMsg.c_str(), "Error", MB_OK);
}

void UIObject::Draw()
{
	RenderTarget *pRTV = GetDefaultRTV();
	DepthStencil *pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMVECTOR eye, fcs, up;
	eye = DirectX::XMVectorSet(0.f, 0.f, -5.f, 0.f);
	fcs = DirectX::XMVectorSet(0.f, .5f, 0.f, 0.f);
	up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(eye, fcs, up);
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f, 0.001f, 1000.f);

	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));

	Sprite::SetView(view);
	Sprite::SetProjection(proj);
	
	//--- 中心に合わせる
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.f, 1.0f);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_fRotation.x));
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_fRotation.y));
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_fRotation.z));
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_fPosition.x - m_fSize.x * 0.5f, m_fPosition.y, 0.0f);
	DirectX::XMMATRIX mWorld = S * Rx * Ry * Rz * T;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));

	Sprite::SetWorld(world);												// スプライトのワールド行列を設定
	Sprite::SetSize(m_fSize);												// スプライトのサイズを設定
	Sprite::SetOffset({ m_fSize.x * 0.5f, 0.0f });	// スプライトの原点を変更
	Sprite::SetTexture(m_pTexture);										// テクスチャを設定
	Sprite::SetUVPos(m_fUVPositon);
	Sprite::SetUVScale(m_fUVScale);
	for (int i = 0; i < 4; ++i)
		Sprite::SetColor(m_fColor[i], i);
	Sprite::Draw();

	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);
}

//=====| Setter |=====//

void UIObject::SetPosition(float X, float Y)
{
	m_fPosition = {X, Y};
}

void UIObject::SetSize(float W, float H)
{
	m_fSize = {W, H};
}

void UIObject::SetRotation(float X, float Y, float Z)
{
	m_fRotation = {X, Y, Z};
}

void UIObject::SetUVPosition(float X, float Y)
{
		m_fUVPositon = { X, Y };
}

void UIObject::SetUVScale(float X, float Y)
{
		m_fUVScale = { X, Y };
}

void UIObject::SetColor(float R, float G, float B, float A)
{
	for (int i = 0; i < 4; i++)
	{
		m_fColor[i] = {R,G,B,A};
	}
}

/**
* \brief 始点、終点、角度を指定してグラデーションを描画.
* 
* \param fromR 始点R値(0.0f ~ 1.0f)
* \param fromG 始点G値(0.0f ~ 1.0f)
* \param fromB 始点B値(0.0f ~ 1.0f)
* \param fromA 始点A値(0.0f ~ 1.0f)
* \param toR 終点R値(0.0f ~ 1.0f)
* \param toG 終点G値(0.0f ~ 1.0f)
* \param toB 終点B値(0.0f ~ 1.0f)
* \param toA 終点A値(0.0f ~ 1.0f)
* \param degree 角度(0 ~ 360: 0 = 上から下)
*/
void UIObject::GenerateGradient(float fromR, float fromG, float fromB, float fromA, float toR, float toG, float toB, float toA, int degree)
{
	GenerateGradient({ fromR, fromG, fromB, fromA }, { toR, toG, toB, toA }, degree);
}

//=====| Getter |=====//

DirectX::XMFLOAT2 UIObject::GetPosition(void)
{
	return m_fPosition;
}

DirectX::XMFLOAT2 UIObject::GetSize(void)
{
	return m_fSize;
}

DirectX::XMFLOAT3 UIObject::GetRotation(void)
{
	return m_fRotation;
}

DirectX::XMFLOAT2 UIObject::GetUVPosition(void)
{
		return m_fUVPositon;
}

DirectX::XMFLOAT2 UIObject::GetUVScale(void)
{
		return m_fUVScale;
}

DirectX::XMFLOAT4 UIObject::GetColor(int index)
{
		return m_fColor[index];
}

//=====| Setter as XMFLOATx |=====//

void UIObject::SetPosition(DirectX::XMFLOAT2 pos)
{
	m_fPosition = pos;
}

void UIObject::SetSize(DirectX::XMFLOAT2 size)
{
	m_fSize = size;
}



/**
* \brief 始点、終点、角度を指定してグラデーションを描画.
* 
* \param colorFrom 始点(各値 0.0f ~ 1.0f)
* \param colorTo 終点(各値 0.0f ~ 1.0f)
* \param degree 角度(0 ~ 360: 0 = 上から下)
*/
void UIObject::GenerateGradient(DirectX::XMFLOAT4 colorFrom, DirectX::XMFLOAT4 colorTo, int degree)
{
  float angleRad = DirectX::XMConvertToRadians(degree);
	
	DirectX::XMFLOAT2 dir(std::cos(angleRad), std::sin(angleRad));

	DirectX::XMFLOAT2 v[4] = {
			DirectX::XMFLOAT2(-0.5f,  0.5f), 
			DirectX::XMFLOAT2(-0.5f, -0.5f), 
			DirectX::XMFLOAT2(0.5f,  0.5f), 
			DirectX::XMFLOAT2(0.5f, -0.5f), 
	};

	//--- 射影値を計算
	float t[4];
	float tMin = FLT_MAX;
	float tMax = -FLT_MAX;
	for (int i = 0; i < 4; ++i)
	{
		t[i] = v[i].x * dir.x + v[i].y * dir.y;
		tMin = min(tMin, t[i]);
		tMax = max(tMax, t[i]);
	}

	float invLen = (tMax != tMin) ? 1.0f / (tMax - tMin) : 0.0f;

	//--- 正規化して色を補間し、頂点カラーに反映
	for (int i = 0; i < 4; ++i)
	{
		float u = (t[i] - tMin) * invLen;
		u = std::clamp(u, 0.0f, 1.0f);

		DirectX::XMFLOAT4 col;
		col.x = colorFrom.x + (colorTo.x - colorFrom.x) * u;
		col.y = colorFrom.y + (colorTo.y - colorFrom.y) * u;
		col.z = colorFrom.z + (colorTo.z - colorFrom.z) * u;
		col.w = colorFrom.w + (colorTo.w - colorFrom.w) * u;

		m_fColor[i] = col;
	}
}


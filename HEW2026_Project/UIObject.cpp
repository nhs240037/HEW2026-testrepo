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

	std::string TexturePath = "Assets/Texture/" + RelativeTexturePathFromTextureFolder;
	std::string FailedMsg = "Texture load failed.\n"+TexturePath;

	m_pTexture = new Texture();
	if(FAILED(m_pTexture->Create(TexturePath.c_str())))
		MessageBox(NULL, FailedMsg.c_str(), "Error", MB_OK);
};

UIObject::~UIObject()
{
	SAFE_DELETE(m_pTexture);
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
		Sprite::Draw();

	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);
}

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


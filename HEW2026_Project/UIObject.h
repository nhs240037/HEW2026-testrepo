/*********************************************************************
 * \file   UIObject.h
 * \brief  UIオブジェクトの基底クラス Base class of UI Object.
 *
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-18
*********************************************************************/

#pragma once
#include <string>
#include <DirectXMath.h>
#include "Sprite.h"
#include "Texture.h";


class UIObject
{
public:
	UIObject();
	UIObject(std::string RelativeTexturePathFromTextureFolder);
	UIObject(float PositionX, float PositionY);
	UIObject(float PositionX, float PositionY, float Width, float Height);
	UIObject(float PositionX, float PositionY, float Width, float Height, float RotationX, float RotationY, float RotationZ);
	UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY);
	UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY, float Width, float Height);
	UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY, float Width, float Height, float RotationX, float RotationY, float RotationZ);
	~UIObject();
	virtual void Update() {};
	void Draw();

	void SetPosition(float X, float Y);
	void SetSize(float W, float H);
	void SetRotation(float X, float Y, float Z);
	void SetTexture(std::string RelativeTexturePathFromTextureFolder);
	void SetUVPosition(float X, float Y);
	void SetUVScale(float X, float Y);
	void SetColor(float R, float G, float B, float A);

	DirectX::XMFLOAT2 GetPosition(void);
	DirectX::XMFLOAT2 GetSize(void);
	DirectX::XMFLOAT3 GetRotation(void);
	DirectX::XMFLOAT2 GetUVPosition(void);
	DirectX::XMFLOAT2 GetUVScale(void);
	DirectX::XMFLOAT4 GetColor(void);

	void SetPosition(DirectX::XMFLOAT2 pos);
	void SetSize(DirectX::XMFLOAT2 size);
	void SetRotation(DirectX::XMFLOAT3 rotation);
	void SetUVPosition(DirectX::XMFLOAT2 uvPos);
	void SetUVScale(DirectX::XMFLOAT2 uvScale);
	void SetColor(DirectX::XMFLOAT4 color);

private:
	Texture* m_pTexture;
	DirectX::XMFLOAT2 m_fPosition;
	DirectX::XMFLOAT2 m_fSize;
	DirectX::XMFLOAT3 m_fRotation;
	DirectX::XMFLOAT2 m_fUVPositon;
	DirectX::XMFLOAT2 m_fUVScale;
	DirectX::XMFLOAT4 m_fColor;
};
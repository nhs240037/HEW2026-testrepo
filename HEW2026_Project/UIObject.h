/*********************************************************************
 * \file   UIObject.h
 * \brief  UIオブジェクトの基底クラス Base class of UI Object.
 *
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-18
 *********************************************************************/
#pragma once
#include <string>

class UIObject
{
public:
	UIObject();
	UIObject(std::string RelativeTexturePathFromTextureFolder);
	UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY);
	UIObject(std::string RelativeTexturePathFromTextureFolder, float PositionX, float PositionY, float Width, float Height);
	~UIObject();
	virtual void Update() {};
	virtual void Draw() {};
};
#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include <fstream>   // ファイル操作用
#include <sstream>   // 文字列ストリーム用
#include <string>
#include <vector>



HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();

#endif // __MAIN_H__
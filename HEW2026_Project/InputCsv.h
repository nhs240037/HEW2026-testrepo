#pragma once

#include <iostream>
#include <fstream>   // ファイル操作用
#include <sstream>   // 文字列ストリーム用
#include <string>
#include <vector>

struct CsvData
{
	int width;
	int height;
	float move_speed;
};


void InitInputCsv(CsvData*);
void UninitInputCsv();
CsvData GetCsvData();

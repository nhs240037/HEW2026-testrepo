#include "CsvData.h"
#include <fstream>
#include <sstream>
#include <string>


void CsvData::Init()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            this->hamb[i].type[j] = -1;
        }
    }
    this->hamb[0].type[0] = NULL;

    const char* filename = "Assets/Csv/debug.csv";
    const char* filename_humb = "Assets/Csv/hamburger_menu.csv";

    std::ifstream inputFile(filename);
    std::ifstream inputFile_humb(filename_humb);

    if (!inputFile.is_open()) 
    {
        return;
    }
    if (!inputFile_humb.is_open()) 
    {
        return;
    }

    std::string line;
    std::string line_h;

    // ヘッダ行をスキップ (オプション)
    //std::getline(inputFile, line);
    // 今回はスキップ
    while (std::getline(inputFile_humb, line_h))
    {
        std::stringstream ss_h(line_h);
        std::string cell_h[5];
        int count = 0;
        static int s_count_h;
        if (!std::getline(ss_h, cell_h[count], ',')) count++;
        if (!std::getline(ss_h, cell_h[count], ',')) count++;
        if (!std::getline(ss_h, cell_h[count], ',')) count++;
        if (!std::getline(ss_h, cell_h[count], ',')) count++;
        if (!std::getline(ss_h, cell_h[count], ',')) count++;
        continue;
        try
        {
            static int s_count;
            int kari;
            switch (s_count)
            {
            case CsvData::Hamburger_Tag1:
                kari = std::stod(cell_h[0]);
                this->hamb[s_count_h].type[0] = kari;
                break;
            case CsvData::Hamburger_Tag2:
                kari = std::stod(cell_h[1]);
                this->hamb[s_count_h].type[1] = kari;
                break;
            case CsvData::Hamburger_Tag3:
                kari = std::stod(cell_h[2]);
                this->hamb[s_count_h].type[2] = kari;
                break;
            case CsvData::Hamburger_Tag4:
                kari = std::stod(cell_h[3]);
                this->hamb[s_count_h].type[3] = kari;
                break;
            case CsvData::Hamburger_Tag5:
                kari = std::stod(cell_h[4]);
                this->hamb[s_count_h].type[4] = kari;
                break;
            }
        }
        catch(const std::invalid_argument& e)
        {

        }
        s_count_h++;
    }
    // データ行の処理
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line);
        std::string cell;

        // 1列目 (ID) をスキップ
        if (!std::getline(ss, cell, ',')) continue;

        // 2列目 (Name: 文字列として取得)
        std::string name;
        if (!std::getline(ss, name, ',')) continue;

        // 3列目 (Price: 数値として取得)
        std::string price_str;
        if (!std::getline(ss, price_str, ',')) continue;

        try 
        {
            // 文字列を float 型の数値に変換
            float price = std::stod(price_str);

            // 代入する数値を変更するためのカウントする変数
            static int count;

            // 処理・出力
            

            // 代入処理
            switch (count)
            {
            case DownloadState::Field_WIDTH:
                this->field.width = int(price);
                this->width = (int)price;
                break;
            case DownloadState::Field_HEIGHT:
                this->field.height = int(price);
                this->height = (int)price;
                break;
            case DownloadState::Field_COLUMN:
                this->field.column = int(price);
                break;
            case DownloadState::Field_ROW:
                this->field.row = int(price);
                break;
            case DownloadState::Player_POS_X:
                this->player.pos.x = price;
                break;
            case DownloadState::Player_POS_Y:
                this->player.pos.y = price;
                break;
            case DownloadState::Player_SIZE_X:
                this->player.size.x = price;
                break;
            case DownloadState::Player_SIZE_Y:
                this->player.size.y = price;
                break;
            case DownloadState::Player_SPEED:
                this->player.speed = (price) * 0.01f;
                this->speed = (price) * 0.01f;
                break;
            case DownloadState::Block_Pos_X:
                this->block.blo.pos.x = price;
                break;
            case DownloadState::Block_Pos_Y:
                this->block.blo.pos.y = price;
                break;
            case DownloadState::Block_Size_X:
                this->block.blo.size.x = price;
                break;
            case DownloadState::Block_Size_Y:
                this->block.blo.size.y = price;
                break;
            case DownloadState::Block_Drop_Speed:
                this->block.blo.posY = price;
                break;
            case DownloadState::Block_Respawn_Height:
                this->block.height = price;
            }
            count++;
        }
        catch (const std::invalid_argument& e) {
            // 変換対象の文字列が数値として無効だった場合
            //std::cerr << "警告: Priceのデータが数値ではありません (\"" << price_str << "\")" << std::endl;

        }
        catch (const std::out_of_range& e) {
            // 変換後の数値が double 型の範囲を超えていた場合
            //std::cerr << "警告: Priceの値が大きすぎます (\"" << price_str << "\")" << std::endl;

        }
    }

    inputFile.close();
}

#pragma once

#define MAX_HAMBRUGER       (10)
#define MAX_HAMBRUGER_TYPE  (5)

class CsvData {
private:
    CsvData() = default;
    ~CsvData() = default;
public:
    struct f2
    {
        float x;
        float y;
    };
private:

    struct Player
    {
        f2 pos;
        f2 size;
        float speed;
    };
    struct Field
    {
        float width;    // フィールドの横幅
        float height;   // フィールドの高さ
        int column;     // フィールドの横のグリッド数
        int row;        // フィールドの縦のグリッド数
    };

    struct Object
    {
        f2 pos;
        f2 size;
        float posY;
    };
    struct Block
    {
        Object blo;
        float height;
    };
    struct Hamburger
    {
        int type[MAX_HAMBRUGER_TYPE];
    };

    enum DownloadState
    {
        Field_WIDTH,
        Field_HEIGHT,
        Field_COLUMN,
        Field_ROW,
        Player_POS_X,
        Player_POS_Y,
        Player_SIZE_X,
        Player_SIZE_Y,
        Player_SPEED,
        Block_Pos_X,
        Block_Pos_Y,
        Block_Size_X,
        Block_Size_Y,
        Block_Drop_Speed,
        Block_Respawn_Height,
        Hamburger_Tag1,
        Hamburger_Tag2,
        Hamburger_Tag3,
        Hamburger_Tag4,
        Hamburger_Tag5
    };

    Player player;
    Field field;
    Block block;
    Hamburger hamb[MAX_HAMBRUGER];

    int width;
    int height;
    float speed;
public:
    CsvData(const CsvData&) = delete;
    CsvData& operator=(const CsvData&) = delete;

    void Init();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetSpeed() const { return speed; }

    Player GetPlayerState() { return player; }
    Field GetFieldState() { return field; }
    Block GetBlockState() { return block; }
    Hamburger* GetHambuger() { return hamb; }

    static CsvData& get_instance()
    {
        static CsvData instance;
        return instance;
    }
};

// SoundManager.h / .cpp 合体サンプル
#include <xaudio2.h>
#include <fstream>
#include <vector>
#include <string>
#include <atomic>
#include <iostream>

#pragma comment(lib, "xaudio2.lib")

// インスタンスを取得するマクロ : sound でアクセス
#define SE_INS SoundManager& sound = SoundManager::GetInstance();
#define SE_INS_So SoundManager& sound = SoundManager::GetInstance();sound

// ================================
// WAV 読み込み（PCM専用の超簡易版）
// ================================
struct WavData {
    WAVEFORMATEX wfx{};
    std::vector<BYTE> buffer;
};

static WavData LoadWav(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(std::string("WAV が開けない: ") + filename);
    }

    auto read4 = [&](char out[4]) {
        file.read(out, 4);
        if (!file) throw std::runtime_error("WAV 読み込み失敗");
        };

    char riff[4]; read4(riff);         // "RIFF"
    file.ignore(4);                    // file size
    char wave[4]; read4(wave);         // "WAVE"

    // "fmt " チャンクまで進む
    char chunkId[4];
    uint32_t chunkSize = 0;

    WAVEFORMATEX wfx{};
    bool fmtFound = false;
    bool dataFound = false;
    std::vector<BYTE> data;

    while (!dataFound && file) {
        read4(chunkId);
        file.read(reinterpret_cast<char*>(&chunkSize), 4);
        if (!file) break;

        if (std::strncmp(chunkId, "fmt ", 4) == 0) {
            // フォーマット情報
            if (chunkSize < sizeof(WAVEFORMATEX)) {
                std::vector<char> tmp(chunkSize);
                file.read(tmp.data(), chunkSize);
                std::memcpy(&wfx, tmp.data(), chunkSize);
            }
            else {
                file.read(reinterpret_cast<char*>(&wfx), sizeof(WAVEFORMATEX));
                if (chunkSize > sizeof(WAVEFORMATEX)) {
                    file.ignore(chunkSize - sizeof(WAVEFORMATEX));
                }
            }
            fmtFound = true;
        }
        else if (std::strncmp(chunkId, "data", 4) == 0) {
            // 音声データ本体
            data.resize(chunkSize);
            file.read(reinterpret_cast<char*>(data.data()), chunkSize);
            dataFound = true;
        }
        else {
            // その他チャンクはスキップ
            file.ignore(chunkSize);
        }
    }

    if (!fmtFound || !dataFound) {
        throw std::runtime_error("WAV フォーマット異常: " + std::string(filename));
    }

    WavData result;
    result.wfx = wfx;
    result.buffer = std::move(data);
    return result;
}

// ================================
// XAudio2 Voice 用コールバック
// ================================
struct VoiceCallback : public IXAudio2VoiceCallback
{
    std::atomic<bool> finished = false;

    // コンストラクタ
    VoiceCallback() = default;

    // コピー禁止
    VoiceCallback(const VoiceCallback&) = delete;
    VoiceCallback& operator=(const VoiceCallback&) = delete;

    // ムーブ許可
    VoiceCallback(VoiceCallback&&) = default;
    VoiceCallback& operator=(VoiceCallback&&) = default;

    void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override {
        finished = true;
    }

    void STDMETHODCALLTYPE OnStreamEnd() override {}
    void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {}
    void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) override {}
    void STDMETHODCALLTYPE OnBufferStart(void*) override {}
    void STDMETHODCALLTYPE OnLoopEnd(void*) override {}
    void STDMETHODCALLTYPE OnVoiceError(void*, HRESULT) override {}
};


// ================================
// サウンドマネージャ（シングルトン）
// ================================
class SoundManager
{
public:
    static SoundManager& GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

    bool Init();
    void Uninit();

    /// <summary>
    /// SEを再生する関数
    /// </summary>
    /// <param name="no">番目の音を再生 </param>
    /// 0:金額増加 
    /// 1:積載音
    /// 2:注文表示
    /// 3:注文品完成
    /// 4:歩行音
    /// 5:落下音
    void PlaySE(int no);
    void PlayBGM(int no, bool loopFlag);
    void StopBGM();

private:
    SoundManager() = default;
    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    struct SEVoice {
        IXAudio2SourceVoice* voice = nullptr;
        VoiceCallback callback;
        bool inUse = false;

        SEVoice() = default;

        // コピーは禁止（念のため明示）
        SEVoice(const SEVoice&) = delete;
        SEVoice& operator=(const SEVoice&) = delete;

        // ムーブも特に使わないので定義不要（あってもいい）
    };


    IXAudio2* m_xAudio2 = nullptr;
    IXAudio2MasteringVoice* m_masterVoice = nullptr;

    std::vector<WavData> m_seData;
    std::vector<WavData> m_bgmData;

    static const int MAX_SE_VOICE = 16;
    SEVoice m_seVoices[MAX_SE_VOICE];   // ★ vector → 固定配列にする

    IXAudio2SourceVoice* m_bgmVoice = nullptr;
    VoiceCallback m_bgmCallback;
    bool m_bgmLoop = false;
    int  m_currentBGM = -1;

private:
    void CleanupSEVoices();
};

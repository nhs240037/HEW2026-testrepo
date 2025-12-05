#include "Sound.h"
// ================================
// 実装
// ================================

bool SoundManager::Init()
{
    HRESULT hr = XAudio2Create(&m_xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        std::cerr << "XAudio2Create 失敗\n";
        return false;
    }

    hr = m_xAudio2->CreateMasteringVoice(&m_masterVoice);
    if (FAILED(hr)) {
        std::cerr << "CreateMasteringVoice 失敗\n";
        m_xAudio2->Release();
        m_xAudio2 = nullptr;
        return false;
    }

    // ===== ここをあなたの指定パスに変更済み =====
    const char* SE_FILES[] = {
        "Assets/Sound/money_up.wav",        // 0
        "Assets/Sound/ride.wav",            // 1
        "Assets/Sound/order_show.wav",      // 2
        "Assets/Sound/order_success.wav",   // 3
        "Assets/Sound/walk.wav",            // 4
        "Assets/Sound/drop.wav",             // 5
        "Assets/Sound/TimeLimit.wav"        // 6
        //"Assets/Sound/TimeLimit.wav"
    };

    // BGM は今は無し。あとで追加するならここにパスを足す。
    // 例:
    // const char* BGM_FILES[] = {
    //     "Assets/Sound/bgm_title.wav",
    //     "Assets/Sound/bgm_stage1.wav",
    // };
    const char* BGM_FILES[] = {
        // 空->だとバグったので魔王魂の曲を入れるだけ入れる
        "C:\\Users\\abyss\\Documents\\GitHub\\HEW2026\\HEW2026_Project\\Assets\\Sound\\maou_bgm_cyber13.wav"
    };
    const size_t SE_COUNT = sizeof(SE_FILES) / sizeof(SE_FILES[0]);
    const size_t BGM_COUNT = sizeof(BGM_FILES) / sizeof(BGM_FILES[0]);
    // =============================================

    m_masterVoice->SetVolume(1.0f);

    // auto test = LoadWav("C:\\Users\\abyss\\Documents\\GitHub\\HEW2026\\HEW2026_Project\\Assets\\Sound\\TimeLimit.wav");

    try {
        // SE 読み込み
        m_seData.clear();
        m_seData.reserve(SE_COUNT);
        for (size_t i = 0; i < SE_COUNT; ++i) {
            m_seData.emplace_back(LoadWav(SE_FILES[i]));
        }
        

        // BGM 読み込み（今は 0 件なので何もしない）
        //m_bgmData.clear();
        //m_bgmData.reserve(BGM_COUNT);
        //for (size_t i = 0; i < BGM_COUNT; ++i) {
        //    m_bgmData.emplace_back(LoadWav(BGM_FILES[i]));
        //}
    }
    catch (const std::exception& e) {
        std::cerr << "WAV 読み込み失敗: " << e.what() << "\n";
        Uninit();
        return false;
    }

    // SE ボイスを事前に複数作っておく（全 SE 共通フォーマット前提）
    for (int i = 0; i < MAX_SE_VOICE; ++i) {
        HRESULT r = m_xAudio2->CreateSourceVoice(
            &m_seVoices[i].voice,
            &m_seData[0].wfx,
            0,
            XAUDIO2_DEFAULT_FREQ_RATIO,
            &m_seVoices[i].callback
        );
        if (FAILED(r)) {
            std::cerr << "SE SourceVoice 作成失敗\n";
            Uninit();
            return false;
        }
    }

    // BGM 用ボイス（BGM データがあるときだけ作る）
    if (!m_bgmData.empty() && false) {
        HRESULT r = m_xAudio2->CreateSourceVoice(
            &m_bgmVoice,
            &m_bgmData[0].wfx,
            0,
            XAUDIO2_DEFAULT_FREQ_RATIO,
            &m_bgmCallback
        );
        if (FAILED(r)) {
            std::cerr << "BGM SourceVoice 作成失敗\n";
            Uninit();
            return false;
        }
    }


    return true;
}

void SoundManager::Uninit()
{
    StopBGM();

    // SE Voice 破棄
    for (int i = 0; i < MAX_SE_VOICE; ++i) {
        auto& v = m_seVoices[i];
        if (v.voice) {
            v.voice->DestroyVoice();
            v.voice = nullptr;
            v.inUse = false;
            v.callback.finished = false;
        }
    }
    //m_seVoices.clear();

    // BGM Voice 破棄
    if (m_bgmVoice) {
        m_bgmVoice->DestroyVoice();
        m_bgmVoice = nullptr;
    }

    if (m_masterVoice) {
        m_masterVoice->DestroyVoice();
        m_masterVoice = nullptr;
    }

    if (m_xAudio2) {
        m_xAudio2->Release();
        m_xAudio2 = nullptr;
    }

    m_seData.clear();
    m_bgmData.clear();
}

// 再生終了した SE ボイスを再利用可能状態に戻す
void SoundManager::CleanupSEVoices()
{
    for (auto& v : m_seVoices) {
        if (v.inUse && v.callback.finished) {
            v.voice->Stop();
            v.voice->FlushSourceBuffers();
            v.inUse = false;
            v.callback.finished = false;
        }
    }
}

// 効果音再生
void SoundManager::PlaySE(int no)
{
    if (no < 0 || no >= static_cast<int>(m_seData.size())) return;
    if (!m_xAudio2) return;

    // 終了済みのボイスを解放
    CleanupSEVoices();

    // 空きボイスを探す
    SEVoice* slot = nullptr;
    for (auto& v : m_seVoices) {
        if (!v.inUse) {
            slot = &v;
            break;
        }
    }
    // 空きがなければ諦める（もしくは一番古いのを潰すなど拡張可）
    if (!slot) {
        // std::cerr << "SE ボイス不足\n";
        return;
    }

    auto& data = m_seData[no];

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = static_cast<UINT32>(data.buffer.size());
    buf.pAudioData = data.buffer.data();
    buf.Flags = XAUDIO2_END_OF_STREAM;
    buf.LoopCount = 0; // SE は基本ループしない

    slot->callback.finished = false;
    slot->voice->Stop();
    slot->voice->FlushSourceBuffers();
    slot->voice->SubmitSourceBuffer(&buf);
    slot->voice->Start();
    slot->inUse = true;
}

// BGM 再生
void SoundManager::PlayBGM(int no, bool loopFlag)
{
    if (no < 0 || no >= static_cast<int>(m_bgmData.size())) return;
    if (!m_bgmVoice) return; // 今は BGM 無しなのでここで必ず return される

    StopBGM();

    auto& data = m_bgmData[no];

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = static_cast<UINT32>(data.buffer.size());
    buf.pAudioData = data.buffer.data();
    buf.Flags = XAUDIO2_END_OF_STREAM;
    buf.LoopCount = loopFlag ? XAUDIO2_LOOP_INFINITE : 0;

    m_bgmCallback.finished = false;
    m_bgmLoop = loopFlag;
    m_currentBGM = no;

    m_bgmVoice->SubmitSourceBuffer(&buf);
    m_bgmVoice->Start();
}

// BGM 停止
void SoundManager::StopBGM()
{
    if (!m_bgmVoice) return;

    m_bgmVoice->Stop();
    m_bgmVoice->FlushSourceBuffers();
    m_bgmCallback.finished = false;
    m_currentBGM = -1;
    m_bgmLoop = false;
}

// ================================
// 使い方イメージ
// ================================
/*
int main()
{
    auto& snd = SoundManager::GetInstance();
    if (!snd.Init()) {
        return 1;
    }

    // 効果音テスト
    snd.PlaySE(0); // 金額増加
    snd.PlaySE(4); // 歩行音 など

    // BGM はまだファイル指定してないので何も起こらない
    snd.PlayBGM(0, true);

    for (;;) {
        // ゲームループ
        Sleep(16);
    }

    snd.Uninit();
    return 0;
}
*/

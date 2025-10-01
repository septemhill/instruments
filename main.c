#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // For pow()

// --- 輔助結構和定義 ---

// --- 88 鍵鋼琴頻率 ---
// 陣列索引與鋼琴鍵/音名的完整對應關係：
// --- Octave 0 ---
// 索引 0  : 鍵 1  (A0)
// 索引 1  : 鍵 2  (A#0/Bb0)
// 索引 2  : 鍵 3  (B0)
// --- Octave 1 ---
// 索引 3  : 鍵 4  (C1)
// 索引 4  : 鍵 5  (C#1/Db1)
// 索引 5  : 鍵 6  (D1)
// 索引 6  : 鍵 7  (D#1/Eb1)
// 索引 7  : 鍵 8  (E1)
// 索引 8  : 鍵 9  (F1)
// 索引 9  : 鍵 10 (F#1/Gb1)
// 索引 10 : 鍵 11 (G1)
// 索引 11 : 鍵 12 (G#1/Ab1)
// 索引 12 : 鍵 13 (A1)
// 索引 13 : 鍵 14 (A#1/Bb1)
// 索引 14 : 鍵 15 (B1)
// --- Octave 2 ---
// 索引 15 : 鍵 16 (C2)
// 索引 16 : 鍵 17 (C#2/Db2)
// 索引 17 : 鍵 18 (D2)
// 索引 18 : 鍵 19 (D#2/Eb2)
// 索引 19 : 鍵 20 (E2)
// 索引 20 : 鍵 21 (F2)
// 索引 21 : 鍵 22 (F#2/Gb2)
// 索引 22 : 鍵 23 (G2)
// 索引 23 : 鍵 24 (G#2/Ab2)
// 索引 24 : 鍵 25 (A2)
// 索引 25 : 鍵 26 (A#2/Bb2)
// 索引 26 : 鍵 27 (B2)
// --- Octave 3 ---
// 索引 27 : 鍵 28 (C3)
// 索引 28 : 鍵 29 (C#3/Db3)
// 索引 29 : 鍵 30 (D3)
// 索引 30 : 鍵 31 (D#3/Eb3)
// 索引 31 : 鍵 32 (E3)
// 索引 32 : 鍵 33 (F3)
// 索引 33 : 鍵 34 (F#3/Gb3)
// 索引 34 : 鍵 35 (G3)
// 索引 35 : 鍵 36 (G#3/Ab3)
// 索引 36 : 鍵 37 (A3)
// 索引 37 : 鍵 38 (A#3/Bb3)
// 索引 38 : 鍵 39 (B3)
// --- Octave 4 (中央八度) ---
// 索引 39 : 鍵 40 (C4 - 中央 C)
// 索引 40 : 鍵 41 (C#4/Db4)
// 索引 41 : 鍵 42 (D4)
// 索引 42 : 鍵 43 (D#4/Eb4)
// 索引 43 : 鍵 44 (E4)
// 索引 44 : 鍵 45 (F4)
// 索引 45 : 鍵 46 (F#4/Gb4)
// 索引 46 : 鍵 47 (G4)
// 索引 47 : 鍵 48 (G#4/Ab4)
// 索引 48 : 鍵 49 (A4 - 標準音高 440 Hz)
// 索引 49 : 鍵 50 (A#4/Bb4)
// 索引 50 : 鍵 51 (B4)
// --- Octave 5 ---
// 索引 51 : 鍵 52 (C5)
// 索引 52 : 鍵 53 (C#5/Db5)
// 索引 53 : 鍵 54 (D5)
// 索引 54 : 鍵 55 (D#5/Eb5)
// 索引 55 : 鍵 56 (E5)
// 索引 56 : 鍵 57 (F5)
// 索引 57 : 鍵 58 (F#5/Gb5)
// 索引 58 : 鍵 59 (G5)
// 索引 59 : 鍵 60 (G#5/Ab5)
// 索引 60 : 鍵 61 (A5)
// 索引 61 : 鍵 62 (A#5/Bb5)
// 索引 62 : 鍵 63 (B5)
// --- Octave 6 ---
// 索引 63 : 鍵 64 (C6)
// 索引 64 : 鍵 65 (C#6/Db6)
// 索引 65 : 鍵 66 (D6)
// 索引 66 : 鍵 67 (D#6/Eb6)
// 索引 67 : 鍵 68 (E6)
// 索引 68 : 鍵 69 (F6)
// 索引 69 : 鍵 70 (F#6/Gb6)
// 索引 70 : 鍵 71 (G6)
// 索引 71 : 鍵 72 (G#6/Ab6)
// 索引 72 : 鍵 73 (A6)
// 索引 73 : 鍵 74 (A#6/Bb6)
// 索引 74 : 鍵 75 (B6)
// --- Octave 7 ---
// 索引 75 : 鍵 76 (C7)
// 索引 76 : 鍵 77 (C#7/Db7)
// 索引 77 : 鍵 78 (D7)
// 索引 78 : 鍵 79 (D#7/Eb7)
// 索引 79 : 鍵 80 (E7)
// 索引 80 : 鍵 81 (F7)
// 索引 81 : 鍵 82 (F#7/Gb7)
// 索引 82 : 鍵 83 (G7)
// 索引 83 : 鍵 84 (G#7/Ab7)
// 索引 84 : 鍵 85 (A7)
// 索引 85 : 鍵 86 (A#7/Bb7)
// 索引 86 : 鍵 87 (B7)
// --- Octave 8 ---
// 索引 87 : 鍵 88 (C8)
#define NUM_PIANO_KEYS 88
double piano_key_frequencies[NUM_PIANO_KEYS];

/**
 * @brief 計算鋼琴 88 個鍵的頻率
 * 
 * 鋼琴的標準調音以 A4 (第 49 個鍵) 為 440 Hz。
 * 每個音的頻率是前一個音的 2^(1/12) 倍。
 * 公式: f(n) = 2^((n - 49) / 12) * 440，其中 n 是從 1 到 88 的琴鍵編號。
 */
void generate_piano_frequencies() {
    double a4_freq = 440.0;
    int a4_key_number = 49; // A4 是第 49 個鍵
    for (int i = 0; i < NUM_PIANO_KEYS; i++) {
        int key_number = i + 1;
        piano_key_frequencies[i] = a4_freq * pow(2.0, (double)(key_number - a4_key_number) / 12.0);
    }
}

// --- 音名索引 Enum (對應 piano_key_frequencies 陣列) ---
// 使用 Enum 讓音符定義更具型別安全性與可讀性。
// 's' 代表升記號 (sharp)，例如 Cs4 代表 C#4。
typedef enum {
    // Octave 0
    A0 = 0, As0, B0,
    // Octave 1
    C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A1, As1, B1,
    // Octave 2
    C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A2, As2, B2,
    // Octave 3
    C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A3, As3, B3,
    // Octave 4 (中央八度)
    C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A4, As4, B4,
    // Octave 5
    C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A5, As5, B5,
    // Octave 6
    C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A6, As6, B6,
    // Octave 7
    C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A7, As7, B7,
    // Octave 8
    C8
} PianoKey;


// C 大調中的和弦定義 (三和弦)
// 每個和弦由三個音高組成 (根音, 三度音, 五度音)
struct Chord {
    const char *key;
    PianoKey indices[3]; // 存放 PianoKey enum，對應 piano_key_frequencies 的索引
};

// 使用 QWERTYUI 鍵來觸發和弦
struct Chord chords[] = {
    // Q: C Major (I)
    {"Q", {C4, E4, G4}},
    // W: D Minor (ii)
    {"W", {D4, F4, A4}},
    // E: E Minor (iii)
    {"E", {E4, G4, B4}},
    // R: F Major (IV)
    {"R", {F4, A4, C5}},
    // T: G Major (V)
    {"T", {G4, B4, D4}}, // 使用 D4 讓和弦轉位更緊湊
    // Y: A Minor (vi)
    {"Y", {A4, C5, E5}},
    // U: B Diminished (vii°)
    {"U", {B4, D4, F4}}
};
const int NUM_CHORDS = 7;

// --- 多聲部樂譜結構 ---

// 音樂事件結構
typedef struct {
    int value;      // PianoKey enum 值 (用於旋律) 或 chords 陣列索引 (用於和弦)
    double duration; // 事件持續時間 (秒)
} MusicEvent;

// 聲部類型
typedef enum {
    TRACK_MELODY,
    TRACK_CHORD
} TrackType;

// 聲部結構
typedef struct {
    const char *name;   // 聲部名稱，方便日誌輸出
    TrackType type;     // 聲部類型
    MusicEvent *events;
    int count;
} Track;

// --- 《小星星》樂譜定義 (含旋律與和弦) ---

const double QUARTER_NOTE = 0.4; // 四分音符時長
const double HALF_NOTE = 0.8;    // 二分音符時長
const double WHOLE_NOTE = 1.6;   // 全音符時長

// 聲部 1: 旋律 (Melody)
MusicEvent melody_events[] = {
    // C C G G A A G-
    {C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE},
    {A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE},
    // F F E E D D C-
    {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE},
    {D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE},
    // G G F F E E D-
    {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE},
    {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE},
    // G G F F E E D-
    {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE},
    {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE},
    // C C G G A A G-
    {C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE},
    {A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE},
    // F F E E D D C-
    {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE},
    {D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE},
};

// 聲部 2: 和弦 (Chords)
// 和弦索引對應 `chords` 陣列: 0=C, 3=F, 4=G
MusicEvent chord_events[] = {
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
};


// --- 清理函數 ---

void restore_terminal(void) {
}

void cleanup(CSOUND* csound) {
    csoundStop(csound);
    csoundDestroy(csound);
    // restore_terminal() 不再需要
}

// --- 主要程式碼 ---

int main() {
    // 1. 建立 Csound instance
    // 首先產生鋼琴頻率
    generate_piano_frequencies();

    CSOUND *csound = csoundCreate(NULL);
    if (csound == NULL) {
        fprintf(stderr, "Error: Failed to create Csound instance.\n");
        return 1;
    }


    // 2. 設定輸出
    csoundSetOption(csound, "-odac");

    // 3. Orchestra (保持不變)
    const char *orc =
        "sr = 44100\n"
        "ksmps = 32\n"
        "nchnls = 2\n"
        "0dbfs = 1\n"
        "\n"
        "instr 1\n"
        "    i_freq = p4\n"
        "    i_amp = p5\n"
        "    i_dur = p3\n"
        "\n"
        "    ; 主音\n"
        "    a_env1 linsegr 0, 0.01, i_amp, i_dur, 0\n"
        "    a_sig1 oscili a_env1, i_freq\n"
        "\n"
        "    ; 第二泛音\n"
        "    a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0\n"
        "    a_sig2 oscili a_env2, i_freq*2\n"
        "\n"
        "    ; 第三泛音 (輕微失諧)\n"
        "    a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0\n"
        "    a_sig3 oscili a_env3, i_freq*3.01\n"
        "\n"
        "    ; 混合所有波形 (0.5 防止削波)\n"
        "    a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5\n"
        "    outs a_mix, a_mix\n"
        "endin\n";

    if (csoundCompileOrc(csound, orc) != 0) {
        fprintf(stderr, "Error: Orchestra compilation failed.\n");
        cleanup(csound);
        return 1;
    }

    // 4. Score (動態生成)
    printf("Generating score from sequence...\n");
    
    // 將所有聲部組織到一個陣列中
    Track all_tracks[] = {
        {"Melody", TRACK_MELODY, melody_events, sizeof(melody_events) / sizeof(MusicEvent)},
        {"Chords", TRACK_CHORD,  chord_events,  sizeof(chord_events) / sizeof(MusicEvent)}
        // 未來可以輕鬆加入更多聲部, 例如:
        // {"Bassline", TRACK_CHORD, bass_events, sizeof(bass_events) / sizeof(MusicEvent)}
    };
    int num_tracks = sizeof(all_tracks) / sizeof(Track);

    // 使用一個迴圈處理所有聲部
    for (int t = 0; t < num_tracks; t++) {
        Track current_track = all_tracks[t];
        double current_time = 0.0;
        printf("\nProcessing Track: %s\n", current_track.name);

        for (int i = 0; i < current_track.count; i++) {
            MusicEvent event = current_track.events[i];
            char score_event[128];

            switch (current_track.type) {
                case TRACK_MELODY: {
                    double freq = piano_key_frequencies[event.value];
                    double amp = 0.5; // 旋律音量
                    sprintf(score_event, "i1 %f %f %f %f", current_time, event.duration, freq, amp);
                    csoundInputMessage(csound, score_event);
                    printf("  Note: time=%.2f, dur=%.2f, freq=%.2f\n", current_time, event.duration, freq);
                    break;
                }
                case TRACK_CHORD: {
                    struct Chord c = chords[event.value];
                    double amp = 0.3; // 和弦音量
                    printf("  Chord %s: time=%.2f, dur=%.2f\n", c.key, current_time, event.duration);
                    for (int j = 0; j < 3; j++) {
                        PianoKey index = c.indices[j];
                        double freq = piano_key_frequencies[index];
                        sprintf(score_event, "i1 %f %f %f %f", current_time, event.duration, freq, amp);
                        csoundInputMessage(csound, score_event);
                    }
                    break;
                }
            }
            current_time += event.duration;
        }
    }

    // 5. 啟動 Csound 引擎並執行
    printf("\nStarting Csound playback...\n");
    if (csoundStart(csound) == 0) {
        // 迴圈直到 Csound 完成所有排程的事件
        while (csoundPerformKsmps(csound) == 0);
    }

    // 6. 清理資源
    printf("\nPlayback finished. Cleaning up Csound resources.\n");
    cleanup(csound);

    return 0;
}
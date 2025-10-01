#include <csound.h>
#include <stdio.h>
#include <stdlib.h>

// --- 輔助結構和定義 ---

// C4 大調音階的頻率 (C4 - C5)
// C=0, D=1, E=2, F=3, G=4, A=5, B=6, C5=7
const double full_frequencies[] = {
    261.63, // C4 (Do) - 0
    293.66, // D4 (Re) - 1
    329.63, // E4 (Mi) - 2
    349.23, // F4 (Fa) - 3
    392.00, // G4 (So) - 4
    440.00, // A4 (La) - 5
    493.88, // B4 (Si) - 6
    523.25  // C5 (Do) - 7
};
const int NUM_FREQUENCIES = 8; // 陣列大小為 8

// C 大調中的和弦定義 (三和弦)
// 每個和弦由三個音高組成 (音階索引: 根音, 三度音, 五度音)
struct Chord {
    const char *key;
    int indices[3]; // 存放 full_frequencies 的索引
};

// 使用 QWERTYUI 鍵來觸發和弦
struct Chord chords[] = {
    // Q: C Major (I) - C(0) + E(2) + G(4)
    {"Q", {0, 2, 4}},
    // W: D Minor (ii) - D(1) + F(3) + A(5)
    {"W", {1, 3, 5}},
    // E: E Minor (iii) - E(2) + G(4) + B(6)
    {"E", {2, 4, 6}},
    // R: F Major (IV) - F(3) + A(5) + C5(7)
    {"R", {3, 5, 7}},
    // T: G Major (V) - G(4) + B(6) + D4 (Octave jump to D4 to keep it low)
    // 雖然理論上是 D5 (索引 9)，但我們使用 D4 (索引 1) 讓和弦更緊湊
    {"T", {4, 6, 1}},
    // Y: A Minor (vi) - A(5) + C5(7) + E5(Index 2 up one octave for better voicing)
    {"Y", {5, 7, 2}},
    // U: B Diminished (vii°) - B(6) + D(1) + F(3) (三個音都來自 C4-D4 範圍)
    {"U", {6, 1, 3}}
};
const int NUM_CHORDS = 7;

// --- 自動播放序列定義 ---

// 事件類型
typedef enum {
    EVENT_NOTE,
    EVENT_CHORD,
    EVENT_REST
} EventType;

// 音樂事件結構
typedef struct {
    EventType type;
    int value;      // 對於 NOTE: full_frequencies 的索引; 對於 CHORD: chords 的索引
    double duration; // 事件持續時間 (秒)
} MusicEvent;

// 定義一個簡單的音樂序列 (C -> G -> Am -> F 和弦進行)
MusicEvent sequence[] = {
    {EVENT_CHORD, 0, 2.0}, // C Major (chords[0]), 持續 2 秒
    {EVENT_CHORD, 4, 2.0}, // G Major (chords[4]), 持續 2 秒
    {EVENT_CHORD, 5, 2.0}, // A Minor (chords[5]), 持續 2 秒
    {EVENT_CHORD, 3, 2.0}, // F Major (chords[3]), 持續 2 秒
    {EVENT_REST,  0, 1.0}, // 休止 1 秒
    {EVENT_NOTE,  0, 0.5}, // C4
    {EVENT_NOTE,  1, 0.5}, // D4
    {EVENT_NOTE,  2, 1.0}, // E4
};
const int SEQUENCE_LENGTH = sizeof(sequence) / sizeof(MusicEvent);


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
    double currentTime = 0.0; // 記錄目前在樂譜中的時間點

    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        MusicEvent event = sequence[i];
        char score_event[128];

        switch (event.type) {
            case EVENT_NOTE: {
                double freq = full_frequencies[event.value];
                double amp = 0.5;
                // i1 [start_time] [duration] [frequency] [amplitude]
                sprintf(score_event, "i1 %f %f %f %f", currentTime, event.duration, freq, amp);
                csoundInputMessage(csound, score_event);
                printf("  Note: time=%.2f, dur=%.2f, freq=%.2f\n", currentTime, event.duration, freq);
                break;
            }
            case EVENT_CHORD: {
                struct Chord c = chords[event.value];
                double amp = 0.35; // 和弦中每個音的音量較低以防破音
                printf("  Chord %s: time=%.2f, dur=%.2f\n", c.key, currentTime, event.duration);
                for (int j = 0; j < 3; j++) {
                    int index = c.indices[j];
                    if (index >= 0 && index < NUM_FREQUENCIES) {
                        double freq = full_frequencies[index];
                        sprintf(score_event, "i1 %f %f %f %f", currentTime, event.duration, freq, amp);
                        csoundInputMessage(csound, score_event);
                    }
                }
                break;
            }
            case EVENT_REST: {
                printf("  Rest: time=%.2f, dur=%.2f\n", currentTime, event.duration);
                // 休止符只需要增加時間，不需要發送事件
                break;
            }
        }
        // 更新下一個事件的開始時間
        currentTime += event.duration;
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
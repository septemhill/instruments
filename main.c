#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include "instrument_piano.h"

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
                    const struct Chord* c = get_piano_chord(event.value);
                    if (c == NULL) break; // 如果和弦無效，則跳過

                    double amp = 0.3; // 和弦音量
                    printf("  Chord %s: time=%.2f, dur=%.2f\n", c->key, current_time, event.duration);
                    for (int j = 0; j < 3; j++) {
                        PianoKey key = c->indices[j];
                        double freq = get_piano_frequency(key);
                        if (freq > 0) { // 確保頻率有效
                            sprintf(score_event, "i1 %f %f %f %f", current_time, event.duration, freq, amp);
                            csoundInputMessage(csound, score_event);
                        }
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
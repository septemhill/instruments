#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // for toupper()

// For non-blocking keyboard input on macOS/Linux
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static struct termios old_tio, new_tio;

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

// --- 終端機設定與清理函數 (保持不變) ---

void setup_terminal(void) {
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO); // 關閉標準模式和回顯
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK); // 設置為非阻塞
}

void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

void cleanup(CSOUND* csound) {
    csoundStop(csound);
    csoundDestroy(csound);
    restore_terminal();
}

// --- 主要程式碼 ---

int main() {
    // 1. 建立 Csound instance
    CSOUND *csound = csoundCreate(NULL);
    if (csound == NULL) {
        fprintf(stderr, "Error: Failed to create Csound instance.\n");
        return 1;
    }

    // 設定終端機以便即時讀取鍵盤輸入
    setup_terminal();
    printf("Press keys 1-7 for single notes, Q-U for chords. Press 'q' to quit.\n");

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

    // 4. Score (保持不變 - 讓 Csound 持續運行)
    const char *sco =
        "f0 3600\n";

    if (csoundReadScore(csound, sco) != 0) {
        fprintf(stderr, "Error: Score reading failed.\n");
        cleanup(csound);
        return 1;
    }

    // 5. 啟動 Csound 引擎並執行
    if (csoundStart(csound) == 0) {
        while (csoundPerformKsmps(csound) == 0) {
            // 非阻塞地讀取一個字元
            char c;
            if (read(STDIN_FILENO, &c, 1) > 0) {
                
                // 處理單音 (1-7)
                if (c >= '1' && c <= '7') {
                    int note_index = c - '1';
                    // 使用 full_frequencies 陣列的前七個音符
                    double freq = full_frequencies[note_index]; 
                    char event[128];
                    // i1 0 1.5 freq 0.5 (instr 1, start 0, duration 1.5, freq, amplitude 0.5)
                    sprintf(event, "i1 0 1.5 %f 0.5", freq); 
                    csoundInputMessage(csound, event);
                }
                // 處理和弦 (Q, W, E, R, T, Y, U)
                else if (c == 'Q' || c == 'W' || c == 'E' || c == 'R' || c == 'T' || c == 'Y' || c == 'U' ||
                         c == 'q' || c == 'w' || c == 'e' || c == 'r' || c == 't' || c == 'y' || c == 'u') {
                    
                    char upper_c = toupper(c);
                    
                    for (int i = 0; i < NUM_CHORDS; i++) {
                        if (chords[i].key[0] == upper_c) {
                            double dur = 2.0;  // 和弦持續時間
                            // 和弦中每個音符的音量要調低，以避免三個音疊加導致音訊訊號超載 (Clipping)
                            double amp = 0.35; 

                            for (int j = 0; j < 3; j++) {
                                int index = chords[i].indices[j];
                                
                                // 檢查索引是否有效
                                if (index < NUM_FREQUENCIES && index >= 0) {
                                    double freq = full_frequencies[index];
                                    char event[128];
                                    // 每個音符發送一個獨立的 i-event
                                    sprintf(event, "i1 0 %f %f %f", dur, freq, amp);
                                    csoundInputMessage(csound, event);
                                }
                            }
                            break; // 處理完畢，跳出 for 迴圈
                        }
                    }
                }
                // 處理退出 ('q')
                else if (c == 'q') {
                    break;
                }
            }
        }
    }

    // 6. 清理資源
    printf("Score finished. Cleaning up Csound resources.\n");
    cleanup(csound);

    return 0;
}
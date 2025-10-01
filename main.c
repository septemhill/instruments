#include <csound.h>
#include <stdio.h>

// 輔助函數，用於釋放 Csound 資源
void cleanup(CSOUND* csound) {
    // 確保停止任何正在運行的音訊
    csoundStop(csound);
    // 銷毀 Csound 實例
    csoundDestroy(csound);
}

int main() {
    // 1. 建立 Csound instance
    CSOUND *csound = csoundCreate(NULL);
    if (csound == NULL) {
        fprintf(stderr, "Error: Failed to create Csound instance.\n");
        return 1;
    }

    // 2. 設定輸出到即時音訊裝置 (聲卡)
    // 註：-odac 參數告訴 Csound 將輸出傳送到您的音訊裝置
    csoundSetOption(csound, "-odac");

    // 3. Orchestra (樂器定義) - 使用數字編號 1
    // 使用加法合成（oscili）模擬鋼琴音色，並使用 linsegr 模擬衰減。
    const char *orc = 
        "sr = 44100\n"
        "ksmps = 32\n"
        "nchnls = 2\n"
        "0dbfs = 1\n"
        "\n"
        "instr 1 ; <--- 修復: 使用數字編號 1\n"
        "    i_freq = p4\n"
        "    i_amp = p5\n"
        "    i_dur = p3\n"
        "\n"
        "    ; 主音 (Fundamental) - 衰減最慢\n"
        "    ; linsegr 0, Attack(0.01), Peak(i_amp), Decay(i_dur), End(0)\n"
        "    a_env1 linsegr 0, 0.01, i_amp, i_dur, 0\n"
        "    a_sig1 oscili a_env1, i_freq\n"
        "\n"
        "    ; 第二泛音 (2nd Harmonic) - 衰減稍快，振幅較小\n"
        "    a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0\n"
        "    a_sig2 oscili a_env2, i_freq*2\n"
        "\n"
        "    ; 第三泛音 (3rd Harmonic) - 衰減更快，振幅更小，並輕微失諧 (3.01)\n"
        "    a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0\n"
        "    a_sig3 oscili a_env3, i_freq*3.01\n"
        "\n"
        "    ; 混合所有波形\n"
        "    a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5 ; 乘以 0.5 防止削波 (Clipping)\n"
        "    outs a_mix, a_mix ; 立體聲輸出\n"
        "endin\n";

    if (csoundCompileOrc(csound, orc) != 0) {
        fprintf(stderr, "Error: Orchestra compilation failed.\n");
        cleanup(csound);
        return 1;
    }

    // 4. Score (樂譜) - 播放一個 C4 和弦
    // 修復: 將 i "Piano" 改為 i 1
    const char *sco =
        "; instr | start | duration | frequency | amplitude\n"
        "i 1 0.0 2 261.63 0.5 ; C4\n"
        "i 1 0.0 2 329.63 0.5 ; E4\n"
        "i 1 0.0 2 392.00 0.5 ; G4\n"
        "e"; // 樂譜結束

    if (csoundReadScore(csound, sco) != 0) {
        fprintf(stderr, "Error: Score reading failed.\n");
        cleanup(csound);
        return 1;
    }

    // 5. 啟動 Csound 引擎並執行
    // csoundStart() 會先執行初始化
    if (csoundStart(csound) == 0) {
        // csoundPerformKsmps() 持續執行，直到樂譜結束或發生錯誤
        while (csoundPerformKsmps(csound) == 0) {
            // 在這裡可以加入即時控制邏輯
        }
    }

    // 6. 清理資源
    printf("Score finished. Cleaning up Csound resources.\n");
    cleanup(csound);

    return 0;
}
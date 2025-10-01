#ifndef INSTRUMENT_PIANO_H
#define INSTRUMENT_PIANO_H

#include <math.h>

// --- 鋼琴定義 ---
#define NUM_PIANO_KEYS 88

// --- 音名索引 Enum (對應 piano_key_frequencies 陣列) ---
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

// --- 和弦定義 ---
struct Chord {
    const char *key;
    PianoKey indices[3];
};

// --- 公開變數 ---

// 鋼琴 88 個鍵的頻率陣列
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
extern double piano_key_frequencies[NUM_PIANO_KEYS];

// C 大調中的和弦定義
extern struct Chord chords[];
extern const int NUM_CHORDS;

// --- 公開函式 ---

/**
 * @brief 計算並填充 piano_key_frequencies 陣列
 */
void generate_piano_frequencies();

/**
 * @brief 根據索引取得和弦定義
 * @param index 和弦在 chords 陣列中的索引
 * @return 指向 Chord 結構的指標，如果索引無效則返回 NULL
 */
const struct Chord* get_piano_chord(int index);

/**
 * @brief 根據索引取得鋼琴音高頻率
 * @param key PianoKey 的 enum 值
 * @return 對應的頻率 (Hz)，如果索引無效則返回 0.0
 */
double get_piano_frequency(PianoKey key);


#endif // INSTRUMENT_PIANO_H
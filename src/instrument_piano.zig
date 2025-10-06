const std = @import("std");

const Key = enum(u8) {
    // Octave 0
    A0 = 0,
    As0,
    B0,
    // Octave 1
    C1,
    Cs1,
    D1,
    Ds1,
    E1,
    F1,
    Fs1,
    G1,
    Gs1,
    A1,
    As1,
    B1,
    // Octave 2
    C2,
    Cs2,
    D2,
    Ds2,
    E2,
    F2,
    Fs2,
    G2,
    Gs2,
    A2,
    As2,
    B2,
    // Octave 3
    C3,
    Cs3,
    D3,
    Ds3,
    E3,
    F3,
    Fs3,
    G3,
    Gs3,
    A3,
    As3,
    B3,
    // Octave 4
    C4,
    Cs4,
    D4,
    Ds4,
    E4,
    F4,
    Fs4,
    G4,
    Gs4,
    A4,
    As4,
    B4,
    // Octave 5
    C5,
    Cs5,
    D5,
    Ds5,
    E5,
    F5,
    Fs5,
    G5,
    Gs5,
    A5,
    As5,
    B5,
    // Octave 6
    C6,
    Cs6,
    D6,
    Ds6,
    E6,
    F6,
    Fs6,
    G6,
    Gs6,
    A6,
    As6,
    B6,
    // Octave 7
    C7,
    Cs7,
    D7,
    Ds7,
    E7,
    F7,
    Fs7,
    G7,
    Gs7,
    A7,
    As7,
    B7,
    // Octave 8
    C8,
};

fn pianoFrequencies(comptime len: usize) [len]f64 {
    // 1. 宣告一個未初始化的固定長度陣列
    var piano_key_frequencies: [len]f64 = undefined;

    // 2. 定義常數
    const A4_FREQ: f64 = 440.0;
    const A4_KEY_NUMBER: i32 = 49;

    // 2. 使用 inline for 執行編譯期迴圈
    //    'inline for' 會在編譯時展開，等同於手動寫出每個索引的計算。
    //    i 是 usize 型別，代表陣列索引 (0 到 87)。
    inline for (0..len) |i| {
        // 陣列索引 i 轉換為鍵號 (key_number)，從 1 到 88
        const key_number: i32 = @as(i32, i) + 1;

        // C 語言公式：
        // a4_freq * pow(2.0, (double)(key_number - a4_key_number) / 12.0)

        // 計算指數部分 (n/12)，使用 f64
        const n: f64 = @as(f64, key_number - A4_KEY_NUMBER);
        const exponent = n / 12.0;

        // 計算 2^(n/12)
        // 注意：Zig 的 comptime 允許在編譯期使用標準數學函數，
        // 但這些函數必須在 comptime 語境中能被求值。
        // const freq_multiplier = std.math.pow(f64, 2.0, exponent);
        const freq_multiplier = @exp2(exponent);

        // 計算最終頻率並存入陣列
        piano_key_frequencies[i] = A4_FREQ * freq_multiplier;
    }

    return piano_key_frequencies;
}

const frequencies = pianoFrequencies(@typeInfo(Key).@"enum".fields.len);

pub fn getFrequency(key: Key) f64 {
    return frequencies[@intFromEnum(key)];
}

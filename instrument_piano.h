#ifndef INSTRUMENT_PIANO_H
#define INSTRUMENT_PIANO_H

#include <math.h>

// --- Piano Definitions ---
#define NUM_PIANO_KEYS 88

// --- Note Name Index Enum (corresponds to piano_key_frequencies array) ---
typedef enum {
    // Octave 0
    A0 = 0, As0, B0,
    // Octave 1
    C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A1, As1, B1,
    // Octave 2
    C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A2, As2, B2,
    // Octave 3
    C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A3, As3, B3,
    // Octave 4 (Middle Octave)
    C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A4, As4, B4,
    // Octave 5
    C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A5, As5, B5,
    // Octave 6
    C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A6, As6, B6,
    // Octave 7
    C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A7, As7, B7,
    // Octave 8
    C8
} PianoKey; // Total 88 keys, from index 0 (A0) to 87 (C8)

#define NO_NOTE -1 // A sentinel value to indicate an unused note slot in a chord.

// --- Chord Type Enum (corresponds to chords array index) ---
typedef enum {
    CHORD_C     = 0,  // I
    CHORD_Dm    = 1,  // ii
    CHORD_Em    = 2,  // iii
    CHORD_F     = 3,  // IV
    CHORD_G     = 4,  // V
    CHORD_Am    = 5,  // vi
    CHORD_Bdim  = 6,  // vii°
    CHORD_Cmaj7 = 7,  // Imaj7
    CHORD_Dm7   = 8,  // iim7
    CHORD_Em7   = 9,  // iiim7
    CHORD_Fmaj7 = 10, // IVmaj7
    CHORD_G7    = 11, // V7
    CHORD_Am7   = 12, // vim7
    CHORD_Bm7b5 = 13  // viim7(b5)
} ChordType;

// --- Chord Definition ---
struct Chord {
    const char *name;      // The standard name of the chord (e.g., "C", "Dm7").
    PianoKey indices[4];  // The piano keys that make up the chord (up to 4 notes).
};

// --- Public Variables ---

// Frequency array for the 88 piano keys
// Complete mapping of array index to piano key/note name:
// --- Octave 0 ---
// Index 0  : Key 1  (A0)
// Index 1  : Key 2  (A#0/Bb0)
// Index 2  : Key 3  (B0)
// --- Octave 1 ---
// Index 3  : Key 4  (C1)
// Index 4  : Key 5  (C#1/Db1)
// Index 5  : Key 6  (D1)
// Index 6  : Key 7  (D#1/Eb1)
// Index 7  : Key 8  (E1)
// Index 8  : Key 9  (F1)
// Index 9  : Key 10 (F#1/Gb1)
// Index 10 : Key 11 (G1)
// Index 11 : Key 12 (G#1/Ab1)
// Index 12 : Key 13 (A1)
// Index 13 : Key 14 (A#1/Bb1)
// Index 14 : Key 15 (B1)
// --- Octave 2 ---
// Index 15 : Key 16 (C2)
// Index 16 : Key 17 (C#2/Db2)
// Index 17 : Key 18 (D2)
// Index 18 : Key 19 (D#2/Eb2)
// Index 19 : Key 20 (E2)
// Index 20 : Key 21 (F2)
// Index 21 : Key 22 (F#2/Gb2)
// Index 22 : Key 23 (G2)
// Index 23 : Key 24 (G#2/Ab2)
// Index 24 : Key 25 (A2)
// Index 25 : Key 26 (A#2/Bb2)
// Index 26 : Key 27 (B2)
// --- Octave 3 ---
// Index 27 : Key 28 (C3)
// Index 28 : Key 29 (C#3/Db3)
// Index 29 : Key 30 (D3)
// Index 30 : Key 31 (D#3/Eb3)
// Index 31 : Key 32 (E3)
// Index 32 : Key 33 (F3)
// Index 33 : Key 34 (F#3/Gb3)
// Index 34 : Key 35 (G3)
// Index 35 : Key 36 (G#3/Ab3)
// Index 36 : Key 37 (A3)
// Index 37 : Key 38 (A#3/Bb3)
// Index 38 : Key 39 (B3)
// --- Octave 4 (Middle Octave) ---
// Index 39 : Key 40 (C4 - Middle C)
// Index 40 : Key 41 (C#4/Db4)
// Index 41 : Key 42 (D4)
// Index 42 : Key 43 (D#4/Eb4)
// Index 43 : Key 44 (E4)
// Index 44 : Key 45 (F4)
// Index 45 : Key 46 (F#4/Gb4)
// Index 46 : Key 47 (G4)
// Index 47 : Key 48 (G#4/Ab4)
// Index 48 : Key 49 (A4 - Standard pitch 440 Hz)
// Index 49 : Key 50 (A#4/Bb4)
// Index 50 : Key 51 (B4)
// --- Octave 5 ---
// Index 51 : Key 52 (C5)
// Index 52 : Key 53 (C#5/Db5)
// Index 53 : Key 54 (D5)
// Index 54 : Key 55 (D#5/Eb5)
// Index 55 : Key 56 (E5)
// Index 56 : Key 57 (F5)
// Index 57 : Key 58 (F#5/Gb5)
// Index 58 : Key 59 (G5)
// Index 59 : Key 60 (G#5/Ab5)
// Index 60 : Key 61 (A5)
// Index 61 : Key 62 (A#5/Bb5)
// Index 62 : Key 63 (B5)
// --- Octave 6 ---
// Index 63 : Key 64 (C6)
// Index 64 : Key 65 (C#6/Db6)
// Index 65 : Key 66 (D6)
// Index 66 : Key 67 (D#6/Eb6)
// Index 67 : Key 68 (E6)
// Index 68 : Key 69 (F6)
// Index 69 : Key 70 (F#6/Gb6)
// Index 70 : Key 71 (G6)
// Index 71 : Key 72 (G#6/Ab6)
// Index 72 : Key 73 (A6)
// Index 73 : Key 74 (A#6/Bb6)
// Index 74 : Key 75 (B6)
// --- Octave 7 ---
// Index 75 : Key 76 (C7)
// Index 76 : Key 77 (C#7/Db7)
// Index 77 : Key 78 (D7)
// Index 78 : Key 79 (D#7/Eb7)
// Index 79 : Key 80 (E7)
// Index 80 : Key 81 (F7)
// Index 81 : Key 82 (F#7/Gb7)
// Index 82 : Key 83 (G7)
// Index 83 : Key 84 (G#7/Ab7)
// Index 84 : Key 85 (A7)
// Index 85 : Key 86 (A#7/Bb7)
// Index 86 : Key 87 (B7)
// --- Octave 8 ---
// Index 87 : Key 88 (C8)
extern double piano_key_frequencies[NUM_PIANO_KEYS];

// Chord definitions in C Major
extern struct Chord chords[];
extern const int NUM_CHORDS;

// --- Public Functions ---

/**
 * @brief Calculates and populates the piano_key_frequencies array.
 */
void generate_piano_frequencies();

/**
 * @brief Gets a chord definition by its index.
 * @param index The index of the chord in the chords array.
 * @return A pointer to the Chord struct, or NULL if the index is invalid.
 */
const struct Chord* get_piano_chord(int index);

/**
 * @brief Gets the frequency of a piano key by its index.
 * @param key The enum value from PianoKey.
 * @return The corresponding frequency in Hz, or 0.0 if the key is invalid.
 */
double get_piano_frequency(PianoKey key);


#endif // INSTRUMENT_PIANO_H
#include <stdio.h> // For NULL
#include "instrument_piano.h"

// --- Variable Definitions ---

double piano_key_frequencies[NUM_PIANO_KEYS];

// --- Chord Definitions (Diatonic to C Major) ---
// Triads and Seventh chords for each degree of the C Major scale.
struct Chord chords[] = {
    // --- Triads ---
    {"C",    {C4, E4, G4, NO_NOTE}}, // I:   C Major
    {"Dm",   {D4, F4, A4, NO_NOTE}}, // ii:  D Minor
    {"Em",   {E4, G4, B4, NO_NOTE}}, // iii: E Minor
    {"F",    {F4, A4, C5, NO_NOTE}}, // IV:  F Major
    {"G",    {G4, B4, D4, NO_NOTE}}, // V:   G Major
    {"Am",   {A4, C5, E5, NO_NOTE}}, // vi:  A Minor
    {"Bdim", {B4, D4, F4, NO_NOTE}}, // vii°:B Diminished

    // --- Full Seventh Chords ---
    {"Cmaj7", {C4, E4, G4, B4}}, // Imaj7: C Major 7th
    {"Dm7",   {D4, F4, A4, C5}}, // iim7:  D Minor 7th
    {"Em7",   {E4, G4, B4, D5}}, // iiim7: E Minor 7th
    {"Fmaj7", {F4, A4, C5, E5}}, // IVmaj7:F Major 7th
    {"G7",    {G4, B4, D4, F5}}, // V7:    G Dominant 7th
    {"Am7",   {A4, C5, E5, G5}}, // vim7:  A Minor 7th
    {"Bm7b5", {B4, D4, F4, A4}}, // viim7(b5): B Half-diminished 7th
};
const int NUM_CHORDS = sizeof(chords) / sizeof(struct Chord);

// --- Function Implementations ---

// --- 88-Key Piano Frequencies ---
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
void generate_piano_frequencies() {
    double a4_freq = 440.0;
    int a4_key_number = 49; // A4 is the 49th key
    for (int i = 0; i < NUM_PIANO_KEYS; i++) {
        int key_number = i + 1;
        piano_key_frequencies[i] = a4_freq * pow(2.0, (double)(key_number - a4_key_number) / 12.0);
    }
}

const struct Chord* get_piano_chord(int index) {
    if (index >= 0 && index < NUM_CHORDS) {
        return &chords[index];
    }
    return NULL;
}

double get_piano_frequency(PianoKey key) {
    if (key >= 0 && key < NUM_PIANO_KEYS) {
        return piano_key_frequencies[key];
    }
    return 0.0; // Return invalid frequency
}
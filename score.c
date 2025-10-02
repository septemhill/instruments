#include "score.h"

// --- "Twinkle, Twinkle, Little Star" Score Definition ---

// Note durations are now defined in "beats" relative to a quarter note.
const double QUARTER_NOTE = 1.0; // A quarter note is 1 beat.
const double HALF_NOTE    = 2.0; // A half note is 2 beats.
const double WHOLE_NOTE   = 4.0; // A whole note is 4 beats.

// --- Melody Track Data ---
MusicEvent melody_m1[] = {{C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}};
MusicEvent melody_m2[] = {{A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE}};
MusicEvent melody_m3[] = {{F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}};
MusicEvent melody_m4[] = {{D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE}};
MusicEvent melody_m5[] = {{G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}};
MusicEvent melody_m6[] = {{E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE}};

Measure melody_measures[] = {
    {melody_m1, sizeof(melody_m1)/sizeof(MusicEvent), 4, 4, 100.0}, // 4/4 Time, Start at 100 BPM
    {melody_m2, sizeof(melody_m2)/sizeof(MusicEvent), 4, 4, 0},
    {melody_m3, sizeof(melody_m3)/sizeof(MusicEvent), 4, 4, 0},
    {melody_m4, sizeof(melody_m4)/sizeof(MusicEvent), 4, 4, 160.0}, // Speed up to 160 BPM!
    {melody_m5, sizeof(melody_m5)/sizeof(MusicEvent), 4, 4, 0},
    {melody_m6, sizeof(melody_m6)/sizeof(MusicEvent), 4, 4, 0},
    // Repeat
    {melody_m1, sizeof(melody_m1)/sizeof(MusicEvent), 4, 4, 100.0}, // Back to 100 BPM
    {melody_m2, sizeof(melody_m2)/sizeof(MusicEvent), 4, 4, 0},
    {melody_m3, sizeof(melody_m3)/sizeof(MusicEvent), 4, 4, 0},
    {melody_m4, sizeof(melody_m4)/sizeof(MusicEvent), 4, 4, 0},
};
const int MELODY_MEASURE_COUNT = sizeof(melody_measures)/sizeof(Measure);

// --- Chord Track Data ---
// Chord indices correspond to the `chords` array in instrument_piano.c
// Triads: 0=C, 1=Dm, 2=Em, 3=F, 4=G, 5=Am, 6=Bdim
// 7ths:   7=Cmaj7, 8=Dm7, 9=Em7, 10=Fmaj7, 11=G7, 12=Am7, 13=Bm7b5

// --- Part 1: Basic Triads ---
MusicEvent chord_m1[] = {{CHORD_C, WHOLE_NOTE}}; // C Major
MusicEvent chord_m2[] = {{CHORD_G, WHOLE_NOTE}}; // G Major
MusicEvent chord_m3[] = {{CHORD_C, WHOLE_NOTE}}; // C Major
MusicEvent chord_m4[] = {{CHORD_F, WHOLE_NOTE}}; // F Major

// --- Part 2: Seventh Chords ---
MusicEvent chord_m_ii7[] = {{CHORD_Dm7,   WHOLE_NOTE}}; // Dm7 (ii)
MusicEvent chord_m_V7[]  = {{CHORD_G7,    WHOLE_NOTE}}; // G7  (V)
MusicEvent chord_m_I7[]  = {{CHORD_Cmaj7, WHOLE_NOTE}}; // Cmaj7 (I)

Measure chord_measures[] = {
    {chord_m1, 1, 4, 4, 100.0}, // 4/4 Time, Start at 100 BPM
    {chord_m2, 1, 4, 4, 0},
    {chord_m3, 1, 4, 4, 0},
    {chord_m4, 1, 4, 4, 160.0}, // Speed up to 160 BPM!
    {chord_m1, 1, 4, 4, 0},
    {chord_m2, 1, 4, 4, 0},
    // Repeat
    {chord_m1, 1, 4, 4, 100.0}, // Back to 100 BPM
    {chord_m2, 1, 4, 4, 0},
    {chord_m3, 1, 4, 4, 0},
    {chord_m4, 1, 4, 4, 0},
    // --- Part 2: Seventh Chord Progression (ii-V-I) ---
    {chord_m_ii7, 1, 4, 4, 100.0}, // Dm7 (ii)
    {chord_m_V7,  1, 4, 4, 0},     // G7  (V)
    {chord_m_I7,  1, 4, 4, 0},     // Cmaj7 (I)
    {chord_m_I7,  1, 4, 4, 0},     // Cmaj7 (I)
};
const int CHORD_MEASURE_COUNT = sizeof(chord_measures)/sizeof(Measure);

// --- Bassline Track Data ---
MusicEvent bass_m1[] = {{C3, WHOLE_NOTE}}; // C
MusicEvent bass_m2[] = {{G2, WHOLE_NOTE}}; // G
MusicEvent bass_m3[] = {{C3, WHOLE_NOTE}}; // C
MusicEvent bass_m4[] = {{F2, WHOLE_NOTE}}; // F

Measure bass_measures[] = {
    {bass_m1, 1, 4, 4, 100.0}, {bass_m2, 1, 4, 4, 0}, {bass_m3, 1, 4, 4, 0}, {bass_m4, 1, 4, 4, 160.0},
    {bass_m1, 1, 4, 4, 0},     {bass_m2, 1, 4, 4, 0}, {bass_m1, 1, 4, 4, 100.0}, {bass_m2, 1, 4, 4, 0},
    {bass_m3, 1, 4, 4, 0},     {bass_m4, 1, 4, 4, 0},
};
const int BASS_MEASURE_COUNT = sizeof(bass_measures)/sizeof(Measure);
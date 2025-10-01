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
// Chord indices correspond to the `chords` array: 0='Q'(C), 3='R'(F), 4='T'(G)
MusicEvent chord_m1[] = {{0, WHOLE_NOTE}}; // C Major
MusicEvent chord_m2[] = {{4, WHOLE_NOTE}}; // G Major
MusicEvent chord_m3[] = {{0, WHOLE_NOTE}}; // C Major
MusicEvent chord_m4[] = {{3, WHOLE_NOTE}}; // F Major

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
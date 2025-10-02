#include "score.h"

// --- "Twinkle, Twinkle, Little Star" Score Definition ---

// Note durations are now defined in "beats" relative to a quarter note.
const double QUARTER_NOTE = 1.0;  // A quarter note is 1 beat.
const double HALF_NOTE = 2.0;  // A half note is 2 beats.
const double WHOLE_NOTE = 4.0;  // A whole note is 4 beats.
const double EIGHTH_NOTE = 0.5;  // An eighth note is 1/2 beat.
const double SIXTEENTH_NOTE = 0.25; // A sixteenth note is 1/4 beat.
const double THIRTY_SECOND_NOTE = 0.125; // A 32nd note is 1/8 beat.
const double SIXTY_FOURTH_NOTE = 0.0625; // A 64th note is 1/16 beat.
const double ONE_TWENTY_EIGHTH_NOTE = 0.03125; // A 128th note is 1/32 beat.

// Dotted notes (duration is 1.5 times the original note's value)
const double DOTTED_HALF_NOTE = HALF_NOTE * 1.5;      // 3.0 beats
const double DOTTED_QUARTER_NOTE = QUARTER_NOTE * 1.5;   // 1.5 beats
const double DOTTED_EIGHTH_NOTE = EIGHTH_NOTE * 1.5;    // 0.75 beats
const double DOTTED_SIXTEENTH_NOTE = SIXTEENTH_NOTE * 1.5; // 0.375 beats

// --- Melody Track Data ---
MusicEvent melody_m1[] = { {C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE} };
MusicEvent melody_m2[] = { {A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE} };
MusicEvent melody_m3[] = { {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE} };
MusicEvent melody_m4[] = { {D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE} };
MusicEvent melody_m5[] = { {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE} };
MusicEvent melody_m6[] = { {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE} };

Measure melody_measures[] = {
    {melody_m1, sizeof(melody_m1) / sizeof(MusicEvent), 4, 4, 100.0}, // 4/4 Time, Start at 100 BPM
    {melody_m2, sizeof(melody_m2) / sizeof(MusicEvent), 4, 4, 0},
    {melody_m3, sizeof(melody_m3) / sizeof(MusicEvent), 4, 4, 0},
    {melody_m4, sizeof(melody_m4) / sizeof(MusicEvent), 4, 4, 160.0}, // Speed up to 160 BPM!
    {melody_m5, sizeof(melody_m5) / sizeof(MusicEvent), 4, 4, 0},
    {melody_m6, sizeof(melody_m6) / sizeof(MusicEvent), 4, 4, 0},
    // Repeat
    {melody_m1, sizeof(melody_m1) / sizeof(MusicEvent), 4, 4, 100.0}, // Back to 100 BPM
    {melody_m2, sizeof(melody_m2) / sizeof(MusicEvent), 4, 4, 0},
    {melody_m3, sizeof(melody_m3) / sizeof(MusicEvent), 4, 4, 0},
    {melody_m4, sizeof(melody_m4) / sizeof(MusicEvent), 4, 4, 0},
};
const int MELODY_MEASURE_COUNT = sizeof(melody_measures) / sizeof(Measure);

// --- Chord Track Data ---
// Chord indices correspond to the `chords` array in instrument_piano.c
// Triads: 0=C, 1=Dm, 2=Em, 3=F, 4=G, 5=Am, 6=Bdim
// 7ths:   7=Cmaj7, 8=Dm7, 9=Em7, 10=Fmaj7, 11=G7, 12=Am7, 13=Bm7b5

// --- Part 1: Basic Triads ---
MusicEvent chord_m1[] = { {CHORD_C, WHOLE_NOTE} }; // C Major
MusicEvent chord_m2[] = { {CHORD_G, WHOLE_NOTE} }; // G Major
MusicEvent chord_m3[] = { {CHORD_C, WHOLE_NOTE} }; // C Major
MusicEvent chord_m4[] = { {CHORD_F, WHOLE_NOTE} }; // F Major

// --- Part 2: Seventh Chords ---
MusicEvent chord_m_ii7[] = { {CHORD_Dm7,   WHOLE_NOTE} }; // Dm7 (ii)
MusicEvent chord_m_V7[] = { {CHORD_G7,    WHOLE_NOTE} }; // G7  (V)
MusicEvent chord_m_I7[] = { {CHORD_Cmaj7, WHOLE_NOTE} }; // Cmaj7 (I)

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
const int CHORD_MEASURE_COUNT = sizeof(chord_measures) / sizeof(Measure);

// --- Bassline Track Data ---
MusicEvent bass_m1[] = { {C3, WHOLE_NOTE} }; // C
MusicEvent bass_m2[] = { {G2, WHOLE_NOTE} }; // G
MusicEvent bass_m3[] = { {C3, WHOLE_NOTE} }; // C
MusicEvent bass_m4[] = { {F2, WHOLE_NOTE} }; // F

Measure bass_measures[] = {
    {bass_m1, 1, 4, 4, 100.0}, {bass_m2, 1, 4, 4, 0}, {bass_m3, 1, 4, 4, 0}, {bass_m4, 1, 4, 4, 160.0},
    {bass_m1, 1, 4, 4, 0},     {bass_m2, 1, 4, 4, 0}, {bass_m1, 1, 4, 4, 100.0}, {bass_m2, 1, 4, 4, 0},
    {bass_m3, 1, 4, 4, 0},     {bass_m4, 1, 4, 4, 0},
};
const int BASS_MEASURE_COUNT = sizeof(bass_measures) / sizeof(Measure);

// --- Test/Example Score Data ---

// A scale run of all piano keys
MusicEvent allKeys[] = {
    {C1, QUARTER_NOTE}, {Cs1, QUARTER_NOTE}, {D1, QUARTER_NOTE}, {Ds1, QUARTER_NOTE},
    {E1, QUARTER_NOTE}, {F1, QUARTER_NOTE}, {Fs1, QUARTER_NOTE}, {G1, QUARTER_NOTE},
    {Gs1, QUARTER_NOTE}, {A1, QUARTER_NOTE}, {As1, QUARTER_NOTE}, {B1, QUARTER_NOTE},
    {C2, QUARTER_NOTE}, {Cs2, QUARTER_NOTE}, {D2, QUARTER_NOTE}, {Ds2, QUARTER_NOTE},
    {E2, QUARTER_NOTE}, {F2, QUARTER_NOTE}, {Fs2, QUARTER_NOTE}, {G2, QUARTER_NOTE},
    {Gs2, QUARTER_NOTE}, {A2, QUARTER_NOTE}, {As2, QUARTER_NOTE}, {B2, QUARTER_NOTE},
    {C3, QUARTER_NOTE}, {Cs3, QUARTER_NOTE}, {D3, QUARTER_NOTE}, {Ds3, QUARTER_NOTE},
    {E3, QUARTER_NOTE}, {F3, QUARTER_NOTE}, {Fs3, QUARTER_NOTE}, {G3, QUARTER_NOTE},
    {Gs3, QUARTER_NOTE}, {A3, QUARTER_NOTE}, {As3, QUARTER_NOTE}, {B3, QUARTER_NOTE},
    {C4, QUARTER_NOTE}, {Cs4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {Ds4, QUARTER_NOTE},
    {E4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {Fs4, QUARTER_NOTE}, {G4, QUARTER_NOTE},
    {Gs4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {As4, QUARTER_NOTE}, {B4, QUARTER_NOTE},
    {C5, QUARTER_NOTE}, {Cs5, QUARTER_NOTE}, {D5, QUARTER_NOTE}, {Ds5, QUARTER_NOTE},
    {E5, QUARTER_NOTE}, {F5, QUARTER_NOTE}, {Fs5, QUARTER_NOTE}, {G5, QUARTER_NOTE},
    {Gs5, QUARTER_NOTE}, {A5, QUARTER_NOTE}, {As5, QUARTER_NOTE}, {B5, QUARTER_NOTE},
    {C6, QUARTER_NOTE}, {Cs6, QUARTER_NOTE}, {D6, QUARTER_NOTE}, {Ds6, QUARTER_NOTE},
    {E6, QUARTER_NOTE}, {F6, QUARTER_NOTE}, {Fs6, QUARTER_NOTE}, {G6, QUARTER_NOTE},
    {Gs6, QUARTER_NOTE}, {A6, QUARTER_NOTE}, {As6, QUARTER_NOTE}, {B6, QUARTER_NOTE},
    {C7, QUARTER_NOTE}, {Cs7, QUARTER_NOTE}, {D7, QUARTER_NOTE}, {Ds7, QUARTER_NOTE},
    {E7, QUARTER_NOTE}, {F7, QUARTER_NOTE}, {Fs7, QUARTER_NOTE}, {G7, QUARTER_NOTE},
    {Gs7, QUARTER_NOTE}, {A7, QUARTER_NOTE}, {As7, QUARTER_NOTE}, {B7, QUARTER_NOTE},
    {REST, QUARTER_NOTE}, {REST, QUARTER_NOTE},
};
const int ALL_KEYS_EVENT_COUNT = sizeof(allKeys) / sizeof(MusicEvent);

// Melody from "North"
MusicEvent north_events[] = {
    {B3, QUARTER_NOTE},
    {REST, EIGHTH_NOTE},

    {C4, THIRTY_SECOND_NOTE},
    {B3, THIRTY_SECOND_NOTE},
    {As3, EIGHTH_NOTE},
    {B3, EIGHTH_NOTE},

    {G4, EIGHTH_NOTE},
    {B3, EIGHTH_NOTE},
    {G4, DOTTED_HALF_NOTE},

    {B3, QUARTER_NOTE},
    {REST, EIGHTH_NOTE},

    {C4, THIRTY_SECOND_NOTE},
    {B3, THIRTY_SECOND_NOTE},
    {As3, EIGHTH_NOTE},
    {B3, EIGHTH_NOTE},

    {A4, EIGHTH_NOTE},
    {C4, EIGHTH_NOTE},
    {A4, DOTTED_HALF_NOTE},

    {B3, QUARTER_NOTE},
    {REST, EIGHTH_NOTE},

    {C4, THIRTY_SECOND_NOTE},
    {B3, THIRTY_SECOND_NOTE},
    {As3, EIGHTH_NOTE},
    {B3, EIGHTH_NOTE},

    {B4, EIGHTH_NOTE},
    {D4, EIGHTH_NOTE},
    {B4, DOTTED_QUARTER_NOTE},
    {D4, EIGHTH_NOTE},

    {B4, EIGHTH_NOTE},
    {D4, EIGHTH_NOTE},
    {B4, DOTTED_QUARTER_NOTE},
    {D4, EIGHTH_NOTE},

    {B4, EIGHTH_NOTE},
    {A4, EIGHTH_NOTE},
    {A4, EIGHTH_NOTE},
    {G4, EIGHTH_NOTE},
    {G4, EIGHTH_NOTE},
    {F4, EIGHTH_NOTE},
    {F4, EIGHTH_NOTE},
    {E4, EIGHTH_NOTE},
    {E4, HALF_NOTE},

    {REST, QUARTER_NOTE},
    {REST, QUARTER_NOTE},
};
const int NORTH_EVENT_COUNT = sizeof(north_events) / sizeof(MusicEvent);

Measure north_measures[] = {
    {north_events, sizeof(north_events) / sizeof(MusicEvent), 4, 4, 98.0},
};
const int NORTH_MEASURE_COUNT = sizeof(north_measures) / sizeof(Measure);
#ifndef SCORE_H
#define SCORE_H

#include "instrument_piano.h"

// --- Score Data Structures ---

/**
 * @brief Represents a single musical event, like a note or a chord.
 *
 * This is the most basic building block of the score.
 */
typedef struct {
    int value;       /**< For melody tracks, a PianoKey enum value. For chord tracks, an index into the chords array. */
    double duration; /**< The duration of the event in beats, where 1.0 represents a quarter note. */
} MusicEvent;

#define REST -1 /**< A sentinel value for MusicEvent.value to indicate a rest (silence). */

/**
 * @brief Represents a single measure of music.
 *
 * A measure contains a sequence of musical events and defines the time signature
 * and tempo for that segment of music.
 */
typedef struct {
    MusicEvent* events;          /**< A pointer to an array of MusicEvent structures. */
    int event_count;             /**< The number of events in this measure. */
    int beats_per_measure;       /**< The numerator of the time signature (e.g., 4 for 4/4 time). */
    int beat_unit;               /**< The denominator of the time signature (e.g., 4 for a quarter-note beat). */
    double bpm;                  /**< The tempo (Beats Per Minute) for this measure. If 0, the tempo from the previous measure is used. */
} Measure;

// --- Note Duration Constants (in beats) ---
extern const double QUARTER_NOTE;   // 1.0 beats
extern const double HALF_NOTE;      // 2.0 beats
extern const double WHOLE_NOTE;     // 4.0 beats
extern const double EIGHTH_NOTE;    // 0.5 beats
extern const double SIXTEENTH_NOTE; // 0.25 beats
extern const double THIRTY_SECOND_NOTE; // 0.125 beats
extern const double SIXTY_FOURTH_NOTE; // 0.0625 beats
extern const double ONE_TWENTY_EIGHTH_NOTE; // 0.03125 beats

// Dotted notes
extern const double DOTTED_HALF_NOTE;      // 3.0 beats
extern const double DOTTED_QUARTER_NOTE;   // 1.5 beats
extern const double DOTTED_EIGHTH_NOTE;    // 0.75 beats
extern const double DOTTED_SIXTEENTH_NOTE; // 0.375 beats

// --- Public Score Data ---

extern Measure melody_measures[];
extern const int MELODY_MEASURE_COUNT;

extern Measure chord_measures[];
extern const int CHORD_MEASURE_COUNT;

extern Measure bass_measures[];
extern const int BASS_MEASURE_COUNT;

// --- Test/Example Score Data ---
extern MusicEvent allKeys[];
extern const int ALL_KEYS_EVENT_COUNT;

extern MusicEvent north_events[];
extern const int NORTH_EVENT_COUNT;
extern Measure north_measures[];
extern const int NORTH_MEASURE_COUNT;

#endif // SCORE_H
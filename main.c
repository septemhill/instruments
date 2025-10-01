#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrument_piano.h"
#include "instruments.h"

// --- Polyphonic Score Structure ---

// Music event structure
typedef struct {
    int value;       // PianoKey enum value (for melody) or chords array index (for chords)
    double duration; // Event duration in beats (e.g., 1.0 for a quarter note)
} MusicEvent;

// Measure structure
typedef struct {
    MusicEvent *events;
    int event_count;
    int beats_per_measure;       // Time signature numerator (e.g., 4 for 4/4 time)
    int beat_unit;               // Time signature denominator (e.g., 4 for a quarter-note beat)
    double bpm;                  // Optional: BPM for this measure. If 0, use previous BPM.
} Measure;

// Real-time player state for each track
typedef struct {
    int current_measure;
    int current_event_in_measure;
    double next_event_time;
} TrackState;

// Track type
typedef enum {
    TRACK_MELODY,
    TRACK_CHORD
} TrackType;

// Track structure
typedef struct {
    const char *name;    // Track name for logging
    TrackType type;      // Track type
    int instrument;      // Csound instrument number to use
    Measure *measures;
    int measure_count;
} Track;

// --- "Twinkle, Twinkle, Little Star" Score Definition (with melody and chords) ---
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

// --- Bassline Track Data ---
// A simple bassline playing the root note of each chord in a lower octave.
MusicEvent bass_m1[] = {{C3, WHOLE_NOTE}}; // C
MusicEvent bass_m2[] = {{G2, WHOLE_NOTE}}; // G
MusicEvent bass_m3[] = {{C3, WHOLE_NOTE}}; // C
MusicEvent bass_m4[] = {{F2, WHOLE_NOTE}}; // F

Measure bass_measures[] = {
    {bass_m1, 1, 4, 4, 100.0}, // 4/4 Time, Matches tempo changes
    {bass_m2, 1, 4, 4, 0},
    {bass_m3, 1, 4, 4, 0},
    {bass_m4, 1, 4, 4, 160.0},
    {bass_m1, 1, 4, 4, 0},
    {bass_m2, 1, 4, 4, 0},
    {bass_m1, 1, 4, 4, 100.0},
    {bass_m2, 1, 4, 4, 0},
    {bass_m3, 1, 4, 4, 0},
    {bass_m4, 1, 4, 4, 0},
};

// --- Cleanup Functions ---

/**
 * @brief Validates the score to ensure measures have the correct number of beats.
 * @param tracks Array of tracks to validate.
 * @param num_tracks Number of tracks in the array.
 */
void validate_score(Track* tracks, int num_tracks) {
    printf("Validating score...\n");
    for (int t = 0; t < num_tracks; t++) {
        for (int m = 0; m < tracks[t].measure_count; m++) {
            Measure* measure = &tracks[t].measures[m];
            double total_duration_in_quarter_notes = 0;
            for (int e = 0; e < measure->event_count; e++) {
                total_duration_in_quarter_notes += measure->events[e].duration;
            }

            // Calculate the expected duration of the measure in quarter notes.
            // e.g., for 3/8 time, this is 3 * (4.0 / 8) = 1.5 quarter notes.
            // e.g., for 4/4 time, this is 4 * (4.0 / 4) = 4.0 quarter notes.
            double expected_duration = (double)measure->beats_per_measure * (4.0 / (double)measure->beat_unit);

            // Compare with a small tolerance for floating point inaccuracies.
            if (fabs(total_duration_in_quarter_notes - expected_duration) > 1e-6) {
                printf("  [WARNING] Track '%s', Measure %d: For %d/%d time, expected total duration of %.2f quarter notes, but found %.2f.\n",
                       tracks[t].name, m + 1, measure->beats_per_measure, measure->beat_unit, expected_duration, total_duration_in_quarter_notes);
            }
        }
    }
}

void restore_terminal(void) {
    // Placeholder for potential future terminal state restoration
}

void cleanup(CSOUND* csound) {
    csoundStop(csound);
    csoundDestroy(csound);
}

// --- Main Program ---

int main(int argc, char **argv) {
    // 1. Initialization
    generate_piano_frequencies();

    CSOUND *csound = csoundCreate(NULL);
    atexit(restore_terminal);
    if (csound == NULL) {
        fprintf(stderr, "Error: Failed to create Csound instance.\n");
        return 1;
    }

    // 2. Compile Orchestra
    // Set output to a WAV file instead of real-time audio.
    csoundSetOption(csound, "-odac");
    char *orc = get_orchestra_string();
    if (orc == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for orchestra string.\n");
        cleanup(csound);
        return 1;
    }
    
    if (csoundCompileOrc(csound, orc) != 0) {
        fprintf(stderr, "Error: Orchestra compilation failed.\n");
        free(orc);
        cleanup(csound);
        return 1;
    }

    // 3. Setup Tracks
    Track all_tracks[] = {
        {"Piano Melody",  TRACK_MELODY, 1, melody_measures, sizeof(melody_measures)/sizeof(Measure)},
        {"Violin Chords", TRACK_CHORD,  2, chord_measures,  sizeof(chord_measures)/sizeof(Measure)},
        {"Viola Chords",  TRACK_CHORD,  3, chord_measures,  sizeof(chord_measures)/sizeof(Measure)},
        {"Piano Bass",    TRACK_MELODY, 1, bass_measures,   sizeof(bass_measures)/sizeof(Measure)} // New Bassline Track
    };
    int num_tracks = sizeof(all_tracks) / sizeof(Track);

    // Validate score before playing
    validate_score(all_tracks, num_tracks);

    // 4. Setup Real-time Player State
    TrackState *track_states = (TrackState*)calloc(num_tracks, sizeof(TrackState));
    if (track_states == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for track states.\n");
        free(orc);
        cleanup(csound);
        return 1;
    }
    double current_bpm = 120.0; // Default starting BPM
    double max_end_time = 0.0;  // Track the time the very last note will end.

    // 5. Real-time Performance Loop
    printf("\nStarting Csound playback...\n");
    int performance_running = 1;
    if (csoundStart(csound) == 0) {
        // The loop continues as long as there are events to schedule OR 
        // the score time has not yet reached the end of the last note.
        while ((performance_running || csoundGetScoreTime(csound) < max_end_time) && csoundPerformKsmps(csound) == 0) {
            double current_time_sec = csoundGetScoreTime(csound);

            if (performance_running) {
                performance_running = 0; // Assume finished unless a track is still active
            }
            for (int t = 0; t < num_tracks; t++) {
                Track *track = &all_tracks[t];
                TrackState *ts = &track_states[t];

                if (ts->current_measure >= track->measure_count) {
                    continue; // This track is finished
                }
                if (performance_running == 0) {
                    performance_running = 1; // At least one track is still active
                }

                if (current_time_sec >= ts->next_event_time) {
                    Measure *measure = &track->measures[ts->current_measure];

                    // Check for BPM change at the start of a measure (only for the first track to avoid conflicts)
                    if (t == 0 && ts->current_event_in_measure == 0 && measure->bpm > 0 && current_bpm != measure->bpm) {
                        current_bpm = measure->bpm;
                        printf("\n--- Tempo Change! New BPM: %.1f ---\n", current_bpm);
                    }

                    MusicEvent event = measure->events[ts->current_event_in_measure];
                    
                    // Calculate duration in seconds based on CURRENT BPM
                    double quarter_note_sec = 60.0 / current_bpm;
                    double duration_in_sec = event.duration * quarter_note_sec;

                    // Send score event to Csound
                    char score_event[128];
                    if (track->type == TRACK_MELODY) {
                        double freq = get_piano_frequency(event.value);
                        sprintf(score_event, "i%d %f %f %f %f", track->instrument, 0.0, duration_in_sec, freq, 0.5);
                        csoundInputMessage(csound, score_event);
                    } else if (track->type == TRACK_CHORD) {
                        const struct Chord* c = get_piano_chord(event.value);
                        if (c) {
                            for (int j = 0; j < 3; j++) {
                                double freq = get_piano_frequency(c->indices[j]);
                                sprintf(score_event, "i%d %f %f %f %f", track->instrument, 0.0, duration_in_sec, freq, 0.25);
                                csoundInputMessage(csound, score_event);
                            }
                        }
                    }

                    // Schedule the next event for this track
                    ts->next_event_time += duration_in_sec;
                    // Update the maximum end time for the entire piece
                    if (ts->next_event_time > max_end_time) {
                        max_end_time = ts->next_event_time;
                    }

                    ts->current_event_in_measure++;
                    if (ts->current_event_in_measure >= measure->event_count) {
                        ts->current_event_in_measure = 0;
                        ts->current_measure++;
                    }
                }
            }
        }
    }

    // 6. Clean up resources
    printf("\nPlayback finished. Cleaning up Csound resources.\n");
    free(orc);
    free(track_states);
    cleanup(csound);

    return 0;
}
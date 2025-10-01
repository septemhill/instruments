#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrument_piano.h"
#include "instruments.h"
#include "score.h"

// --- Player Engine Structures ---

/**
 * @brief Holds the real-time playback state for a single track.
 *
 * This structure tracks the current position within the score for a track,
 * allowing the player to know which event to play next and when.
 */
typedef struct {
    int current_measure;          /**< Index of the current measure being played. */
    int current_event_in_measure; /**< Index of the current event within the measure. */
    double next_event_time;       /**< The absolute time in seconds when the next event should be triggered. */
} TrackState;

/**
 * @brief Defines the type of a track, which determines how its events are interpreted.
 */
typedef enum {
    TRACK_MELODY, /**< A monophonic melody line where each event is a single note. */
    TRACK_CHORD   /**< A polyphonic chord line where each event represents a full chord. */
} TrackType;

/**
 * @brief Represents a complete musical track, including its score and metadata.
 *
 * A track is a sequence of measures played by a specific instrument.
 */
typedef struct {
    const char *name;    /**< The name of the track, used for logging and identification. */
    TrackType type;      /**< The type of the track (e.g., melody or chord). */
    int instrument;      /**< The Csound instrument number (from the orchestra) to use for this track. */
    Measure *measures;   /**< A pointer to an array of Measure structures that make up the track's score. */
    int measure_count;   /**< The total number of measures in the track. */
} Track;

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
        {"Piano Melody",  TRACK_MELODY, 1, melody_measures, MELODY_MEASURE_COUNT},
        {"Violin Chords", TRACK_CHORD,  2, chord_measures,  CHORD_MEASURE_COUNT},
        {"Viola Chords",  TRACK_CHORD,  3, chord_measures,  CHORD_MEASURE_COUNT},
        {"Piano Bass",    TRACK_MELODY, 1, bass_measures,   BASS_MEASURE_COUNT}
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
#include <csound.h>
#include <stdio.h>
#include <stdlib.h>
#include "instrument_piano.h"
#include "instruments.h"

// --- Polyphonic Score Structure ---

// Music event structure
typedef struct {
    int value;      // PianoKey enum value (for melody) or chords array index (for chords)
    double duration; // Event duration in seconds
} MusicEvent;

// Track type
typedef enum {
    TRACK_MELODY,
    TRACK_CHORD
} TrackType;

// Track structure
typedef struct {
    const char *name;    // Track name for logging
    TrackType type;      // Track type
    int instrument;      // Csound instrument number
    MusicEvent *events;
    int count;
} Track;

// --- "Twinkle, Twinkle, Little Star" Score Definition (with melody and chords) ---

const double QUARTER_NOTE = 0.4; // Quarter note duration
const double HALF_NOTE = 0.8;    // Half note duration
const double WHOLE_NOTE = 1.6;   // Whole note duration

// Track 1: Melody - Piano
MusicEvent melody_events[] = {
    // C C G G A A G
    {C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE},
    {A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE},
    // F F E E D D C
    {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE},
    {D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE},
    // G G F F E E D
    {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE},
    {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE},
    // G G F F E E D
    {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE},
    {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {D4, HALF_NOTE},
    // C C G G A A G
    {C4, QUARTER_NOTE}, {C4, QUARTER_NOTE}, {G4, QUARTER_NOTE}, {G4, QUARTER_NOTE},
    {A4, QUARTER_NOTE}, {A4, QUARTER_NOTE}, {G4, HALF_NOTE},
    // F F E E D D C
    {F4, QUARTER_NOTE}, {F4, QUARTER_NOTE}, {E4, QUARTER_NOTE}, {E4, QUARTER_NOTE},
    {D4, QUARTER_NOTE}, {D4, QUARTER_NOTE}, {C4, HALF_NOTE},
};

// Track 2: Chords - Piano
// Chord indices correspond to the `chords` array: 0=C, 3=F, 4=G
MusicEvent chord_events[] = {
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
};

// Track 3: Violin Chords (same as piano chords)
MusicEvent violin_chord_events[] = {
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
};

// Track 4: Viola Chords (same as piano chords, but pitch can be adjusted)
MusicEvent viola_chord_events[] = {
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
    {3, WHOLE_NOTE}, // F Major
    {0, WHOLE_NOTE}, // C Major
    {4, WHOLE_NOTE}, // G Major
    {0, WHOLE_NOTE}, // C Major
};


// --- Cleanup Functions ---

void restore_terminal(void) {
}

void cleanup(CSOUND* csound) {
    csoundStop(csound);
    csoundDestroy(csound);
}

// --- Main Program ---

int main() {
    // 1. Create Csound instance
    // First, generate piano frequencies
    generate_piano_frequencies();

    CSOUND *csound = csoundCreate(NULL);
    if (csound == NULL) {
        fprintf(stderr, "Error: Failed to create Csound instance.\n");
        return 1;
    }

    // 2. Set output
    csoundSetOption(csound, "-odac");

    // 3. Orchestra (dynamically assembled from the instruments module)
    char *orc = get_orchestra_string();
    if (orc == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for orchestra string.\n");
        cleanup(csound);
        return 1;
    }
    
    if (csoundCompileOrc(csound, orc) != 0) {
        fprintf(stderr, "Error: Orchestra compilation failed.\n");
        free(orc); // Free memory
        cleanup(csound);
        return 1;
    }

    // 4. Score (dynamic generation)
    printf("Generating score from sequence...\n");
    
    // Organize all tracks into an array
    Track all_tracks[] = {
        {"Piano Melody", TRACK_MELODY, 2, melody_events, sizeof(melody_events) / sizeof(MusicEvent)},
        {"Piano Chords", TRACK_CHORD,  1, chord_events,  sizeof(chord_events) / sizeof(MusicEvent)},
        {"Violin Chords", TRACK_CHORD, 2, violin_chord_events, sizeof(violin_chord_events) / sizeof(MusicEvent)},
        {"Viola Chords", TRACK_CHORD,  3, viola_chord_events, sizeof(viola_chord_events) / sizeof(MusicEvent)}
    };
    int num_tracks = sizeof(all_tracks) / sizeof(Track);

    // Process all tracks using a single loop
    for (int t = 0; t < num_tracks; t++) {
        Track current_track = all_tracks[t];
        double current_time = 0.0;
        printf("\nProcessing Track: %s (Instrument %d)\n", current_track.name, current_track.instrument);

        for (int i = 0; i < current_track.count; i++) {
            MusicEvent event = current_track.events[i];
            char score_event[128];

            switch (current_track.type) {
                case TRACK_MELODY: {
                    double freq = piano_key_frequencies[event.value];
                    double amp = 0.5; // Melody volume
                    sprintf(score_event, "i%d %f %f %f %f", 
                            current_track.instrument, current_time, event.duration, freq, amp);
                    csoundInputMessage(csound, score_event);
                    printf("  Note: time=%.2f, dur=%.2f, freq=%.2f\n", current_time, event.duration, freq);
                    break;
                }
                case TRACK_CHORD: {
                    const struct Chord* c = get_piano_chord(event.value);
                    if (c == NULL) break;

                    double amp = 0.25; // Chord volume (lowered to avoid saturation)
                    printf("  Chord %s: time=%.2f, dur=%.2f\n", c->key, current_time, event.duration);
                    for (int j = 0; j < 3; j++) {
                        PianoKey key = c->indices[j];
                        double freq = get_piano_frequency(key);
                        if (freq > 0) {
                            sprintf(score_event, "i%d %f %f %f %f", 
                                    current_track.instrument, current_time, event.duration, freq, amp);
                            csoundInputMessage(csound, score_event);
                        }
                    }
                    break;
                }
            }
            current_time += event.duration;
        }
    }

    // 5. Start the Csound engine and perform
    printf("\nStarting Csound playback...\n");
    if (csoundStart(csound) == 0) {
        while (csoundPerformKsmps(csound) == 0);
    }

    // 6. Clean up resources
    printf("\nPlayback finished. Cleaning up Csound resources.\n");
    free(orc); // Free memory
    cleanup(csound);

    return 0;
}
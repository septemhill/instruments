# Csound Polyphonic Music Player

This is a command-line music player built with C and the Csound API. It programmatically generates a polyphonic score from structured data and synthesizes it into audio. The current example plays a variation of "Twinkle, Twinkle, Little Star" with a melody, chord accompaniment, and a bassline, demonstrating dynamic tempo changes.

## ✨ Features

- **Polyphonic Playback**: Can play multiple independent tracks simultaneously (e.g., melody, chords, bass).
- **Multi-instrument Timbre**: Assigns different Csound instruments to different tracks (currently includes Piano, Violin, and Viola).
- **Dynamic Tempo & Time Signatures**: Supports tempo changes mid-piece and can handle various time signatures on a per-measure basis.
- **Score Validation**: Includes a utility to automatically check if the notes in each measure correctly add up to the time signature's duration.
- **Modular Design**:
  - `main.c`: The main player engine, manages playback flow and scheduling.
  - `score.c` / `score.h`: Defines the musical score data (notes, rhythms, measures).
  - `instruments.c`: Defines the Csound instrument timbres (the `.orc` code).
  - `instrument_piano.c`: Defines musical constants like piano key frequencies and chord structures.
- **Easy to Extend**: The structured design makes it simple to add new instrument timbres, modify the score, or add entirely new musical tracks.

## 📋 Requirements

Before building and running this project, please ensure you have the following software installed:

1.  **C Compiler**: `clang` or `gcc`.
2.  **Make**: For automated builds.
3.  **Csound**: The audio synthesis engine library.

    On macOS, it is recommended to install via Homebrew:
    ```bash
    brew install csound
    ```

> **Note**: The Csound path in `Makefile` is set for the default Homebrew installation location. If you are using a different operating system or installation path, please modify the `CSOUND_HOME` variable in `Makefile`.

## 🚀 Getting Started

### 1. Build the Project

Open a terminal, navigate to the project root directory, and run the `make` command. The `Makefile` will automatically compile all necessary files.

```bash
make
```

After a successful build, an executable file named `csound_example` will be generated in the directory.

### 2. Run the Project

Execute the newly generated file to hear the music.

```bash
./csound_example
```

The program will output the processing steps for each track and then start playback.

### 3. Clean Up

To delete the compiled object files and the executable, you can run:

```bash
make clean
```

## 🛠️ How to Customize and Extend

### Add a New Instrument Timbre

1.  **Open `instruments.c`**.
2.  Following the format of `piano_instr` and `violin_instr`, add a new `static const char*` string to define your new instrument (e.g., `instr 4`).
3.  In the `get_orchestra_string()` function, add your new instrument string to the `instruments` array.

### Modify the Music

All musical score data is located in `score.c`.

1.  **Open `score.c`**.
2.  To change the melody, modify the `MusicEvent` arrays like `melody_m1`, `melody_m2`, etc. Each event consists of a note (`PianoKey` enum) and a duration (e.g., `QUARTER_NOTE`).
3.  To change the tempo, modify the `bpm` field in the `Measure` definitions (e.g., `melody_measures`). A `bpm` of `0` means it will continue using the previous tempo.

### Add a New Track

1.  **Define the score in `score.c`**: Create new `MusicEvent` arrays for your measures and a `Measure` array to structure them, similar to `bass_measures`.
2.  **Declare the score in `score.h`**: Add `extern` declarations for your new `Measure` array and its count so other files can access it.
    ```c
    // In score.h
    extern Measure my_new_track_measures[];
    extern const int MY_NEW_TRACK_MEASURE_COUNT;
    ```
3.  **Add the track in `main.c`**: Add a new entry to the `all_tracks` array, linking your new score data to an instrument.

    ```c
    // In main.c
    Track all_tracks[] = {
        // ... other tracks
        {"My New Track", TRACK_MELODY, 4, my_new_track_measures, MY_NEW_TRACK_MEASURE_COUNT} // Assuming usage of instr 4
    };
    ```

    The program's main loop will automatically process this new track.

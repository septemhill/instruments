# Csound Polyphonic Music Player

This is a command-line music player built with C and the Csound API. It programmatically generates a polyphonic score and synthesizes it into audio in real-time. The current example plays "Twinkle, Twinkle, Little Star" with a melody and chord accompaniment.

## ✨ Features

- **Polyphonic Support**: Can play multiple independent tracks simultaneously (e.g., melody, chords, bass).
- **Multi-instrument Timbre**: Assign different instrument timbres to different tracks via Csound's Orchestra definition (currently includes Piano, Violin, and Viola).
- **Modular Design**:
  - `main.c`: Manages the musical structure arrangement and playback flow.
  - `instrument_piano.c`: Defines instrument data (like piano pitches and chord structures).
  - `instruments.c`: Defines the Csound instrument timbres (ORC code).
- **Easy to Extend**: Easily add new instrument timbres or define new musical tracks.

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

1.  **Open `main.c`**.
2.  You can modify the `melody_events` array to change the main melody, or the `chord_events` array to change the chord progression.

### Add a New Track

1.  **Define a new score in `main.c`**: Create a new `MusicEvent` array, for example, `bass_events`.
2.  **Add the new track to the `all_tracks` array**:

    ```c
    Track all_tracks[] = {
        {"Piano Melody",  TRACK_MELODY, 1, melody_events, ...},
        {"Piano Chords",  TRACK_CHORD,  1, chord_events, ...},
        // ... other tracks
        {"My New Bassline", TRACK_MELODY, 4, bass_events, sizeof(bass_events) / sizeof(MusicEvent)} // Assuming usage of instr 4
    };
    ```

    The program's main loop will automatically process this new track.

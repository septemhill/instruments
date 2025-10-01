#include "instruments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Individual Instrument Definitions ---
// By defining each instrument separately, it's easy to add, remove, or modify them.

static const char* orc_header =
    "sr = 44100\n"
    "ksmps = 32\n"
    "nchnls = 2\n"
    "0dbfs = 1\n\n";

static const char* piano_instr =
    "; Instrument 1: Piano\n"
    "instr 1\n"
    "    i_freq = p4\n"
    "    i_amp = p5\n"
    "    i_dur = p3\n"
    "    a_env1 linsegr 0, 0.01, i_amp, i_dur, 0\n"
    "    a_sig1 oscili a_env1, i_freq\n"
    "    a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0\n"
    "    a_sig2 oscili a_env2, i_freq*2\n"
    "    a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0\n"
    "    a_sig3 oscili a_env3, i_freq*3.01\n"
    "    a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5\n"
    "    outs a_mix, a_mix\n"
    "endin\n\n";

static const char* violin_instr =
    "; Instrument 2: Violin\n"
    "instr 2\n"
    "    i_freq = p4\n"
    "    i_amp = p5\n"
    "    i_dur = p3\n"
    "    a_env linsegr 0, 0.08, i_amp, i_dur-0.1, i_amp*0.7, 0.02, 0\n"
    "    a_sig1 oscili a_env, i_freq\n"
    "    a_sig2 oscili a_env*0.7, i_freq*3\n"
    "    a_sig3 oscili a_env*0.4, i_freq*5\n"
    "    a_sig4 oscili a_env*0.2, i_freq*7\n"
    "    k_vib oscili 5, 5.5\n"
    "    a_vibrato oscili a_env*0.8, i_freq+k_vib\n"
    "    a_mix = (a_sig1 + a_sig2 + a_sig3 + a_sig4 + a_vibrato) * 0.3\n"
    "    outs a_mix, a_mix\n"
    "endin\n\n";

static const char* viola_instr =
    "; Instrument 3: Viola\n"
    "instr 3\n"
    "    i_freq = p4\n"
    "    i_amp = p5\n"
    "    i_dur = p3\n"
    "    a_env linsegr 0, 0.1, i_amp, i_dur-0.12, i_amp*0.75, 0.02, 0\n"
    "    a_sig1 oscili a_env, i_freq\n"
    "    a_sig2 oscili a_env*0.8, i_freq*2\n"
    "    a_sig3 oscili a_env*0.5, i_freq*3\n"
    "    a_sig4 oscili a_env*0.3, i_freq*4\n"
    "    k_vib oscili 4, 4.8\n"
    "    a_vibrato oscili a_env*0.7, i_freq+k_vib\n"
    "    a_mix = (a_sig1 + a_sig2 + a_sig3 + a_sig4 + a_vibrato) * 0.35\n"
    "    outs a_mix, a_mix\n"
    "endin\n\n";

char* get_orchestra_string() {
    // Array of all instrument strings
    const char* instruments[] = { piano_instr, violin_instr, viola_instr };
    int num_instruments = sizeof(instruments) / sizeof(instruments[0]);

    // Calculate total length needed
    size_t total_len = strlen(orc_header) + 1; // +1 for null terminator
    for (int i = 0; i < num_instruments; i++) {
        total_len += strlen(instruments[i]);
    }

    // Allocate memory
    char* orc_string = (char*)malloc(total_len);
    if (orc_string == NULL) return NULL;

    // Build the string
    strcpy(orc_string, orc_header);
    for (int i = 0; i < num_instruments; i++) {
        strcat(orc_string, instruments[i]);
    }

    return orc_string;
}
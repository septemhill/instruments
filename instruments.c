#include "instruments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Individual Instrument Definitions ---
// By defining each instrument separately, it's easy to add, remove, or modify them.
//
// The LINE macro appends a newline character to a string literal.
// C's compile-time string literal concatenation then joins these lines
// into a single static string, which is highly efficient and safe.
#define LINE(s) s "\n"

// A macro to wrap the instrument definition boilerplate.
// It takes the instrument name and the body of the instrument code.
#define DEFINE_INSTRUMENT(name, body) \
    LINE("; Instrument: " #name)       \
    LINE("instr " #name) body LINE("endin") LINE("")

static const char* get_orc_header()
{
    return LINE("sr = 44100")
        LINE("ksmps = 32")
        LINE("nchnls = 2")
        LINE("0dbfs = 1")
        LINE(""); // Extra newline for separation
}

static const char* get_piano_instr()
{
    return DEFINE_INSTRUMENT(1,
        LINE("    i_freq = p4")
        LINE("    i_amp = p5")
        LINE("    i_dur = p3")
        LINE("    a_env1 linsegr 0, 0.01, i_amp, i_dur, 0")
        LINE("    a_sig1 oscili a_env1, i_freq")
        LINE("    a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0")
        LINE("    a_sig2 oscili a_env2, i_freq*2")
        LINE("    a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0")
        LINE("    a_sig3 oscili a_env3, i_freq*3.01")
        LINE("    a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5")
        LINE("    outs a_mix, a_mix"));
}

static const char* get_violin_instr()
{
    return DEFINE_INSTRUMENT(2,
        LINE("    i_freq = p4")
        LINE("    i_amp = p5")
        LINE("    i_dur = p3")
        LINE("    a_env linsegr 0, 0.08, i_amp, i_dur-0.1, i_amp*0.7, 0.02, 0")
        LINE("    a_sig1 oscili a_env, i_freq")
        LINE("    a_sig2 oscili a_env*0.7, i_freq*3")
        LINE("    a_sig3 oscili a_env*0.4, i_freq*5")
        LINE("    a_sig4 oscili a_env*0.2, i_freq*7")
        LINE("    k_vib oscili 5, 5.5")
        LINE("    a_vibrato oscili a_env*0.8, i_freq+k_vib")
        LINE("    a_mix = (a_sig1 + a_sig2 + a_sig3 + a_sig4 + a_vibrato) * 0.3")
        LINE("    outs a_mix, a_mix"));
}

static const char* get_viola_instr()
{
    return DEFINE_INSTRUMENT(3,
        LINE("    i_freq = p4")
        LINE("    i_amp = p5")
        LINE("    i_dur = p3")
        LINE("    a_env linsegr 0, 0.1, i_amp, i_dur-0.12, i_amp*0.75, 0.02, 0")
        LINE("    a_sig1 oscili a_env, i_freq")
        LINE("    a_sig2 oscili a_env*0.8, i_freq*2")
        LINE("    a_sig3 oscili a_env*0.5, i_freq*3")
        LINE("    a_sig4 oscili a_env*0.3, i_freq*4")
        LINE("    k_vib oscili 4, 4.8")
        LINE("    a_vibrato oscili a_env*0.7, i_freq+k_vib")
        LINE("    a_mix = (a_sig1 + a_sig2 + a_sig3 + a_sig4 + a_vibrato) * 0.35")
        LINE("    outs a_mix, a_mix"));
}

char* get_orchestra_string() {
    // Array of all instrument strings
    const char* instruments[] = {
        get_piano_instr(),
        get_violin_instr(),
        get_viola_instr() };
    int num_instruments = sizeof(instruments) / sizeof(instruments[0]);

    // Calculate total length needed
    size_t total_len = strlen(get_orc_header()) + 1; // +1 for null terminator
    for (int i = 0; i < num_instruments; i++) {
        total_len += strlen(instruments[i]);
    }

    // Allocate memory
    char* orc_string = (char*)malloc(total_len);
    if (orc_string == NULL) {
        perror("Failed to allocate memory for orchestra string");
        return NULL;
    }

    // Build the string
    strcpy(orc_string, get_orc_header());
    for (int i = 0; i < num_instruments; i++) {
        strcat(orc_string, instruments[i]);
    }

    return orc_string;
}
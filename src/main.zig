const std = @import("std");
const cs = @import("csound");
const piano = @import("instrument_piano.zig");
const builtin = @import("builtin");
const csound = @cImport({
    @cInclude("csound.h");
});

const orc = cs.Orchestra.init(&.{
    "sr = 44100",
    "ksmps = 32",
    "nchnls = 2",
    "0dbfs = 1",
    "",
}, &.{
    cs.Instrument.init("Piano", &.{
        "i_freq = p4",
        "i_amp = p5",
        "i_dur = p3",
        "a_env1 linsegr 0, 0.01, i_amp, i_dur, 0",
        "a_sig1 oscili a_env1, i_freq",
        "a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0",
        "a_sig2 oscili a_env2, i_freq*2",
        "a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0",
        "a_sig3 oscili a_env3, i_freq*3.01",
        "a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5",
        "outs a_mix, a_mix",
    }),
    cs.Instrument.init("Guitar", &.{
        "i_freq = p4",
        "i_amp = p5",
        "i_dur = p3",
        "a_env1 linsegr 0, 0.01, i_amp, i_dur, 0",
        "a_sig1 oscili a_env1, i_freq",
        "a_env2 linsegr 0, 0.01, i_amp*0.6, i_dur*0.7, 0",
        "a_sig2 oscili a_env2, i_freq*2",
        "a_env3 linsegr 0, 0.01, i_amp*0.3, i_dur*0.4, 0",
        "a_sig3 oscili a_env3, i_freq*3.01",
        "a_mix = (a_sig1 + a_sig2 + a_sig3) * 0.5",
        "outs a_mix, a_mix",
    }),
});

const Sample = enum(u32) {
    A,
    B,
    C,
};

pub fn main() !void {
    const oc: orc = .init;
    const orc_content = comptime oc.buildOrchestra();

    const csd = csound.csoundCreate(null) orelse unreachable;
    defer _ = csound.csoundCleanup(csd);

    _ = csound.csoundSetOption(csd, "-odac");
    _ = csound.csoundCompileOrc(csd, orc_content[0.. :0]);

    if (csound.csoundStart(csd) != 0) unreachable;

    const v1_sample = "i1 0.0 2 440.0 0.5\n";
    csound.csoundInputMessage(csd, v1_sample[0.. :0]);

    while (csound.csoundPerformKsmps(csd) == 0) {
        // break;
    }

    defer _ = csound.csoundStop(csd);
}

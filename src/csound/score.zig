const std = @import("std");

pub const Note = struct {
    pitch: u8,
    duration: f32,
    velocity: ?u8, // reserved;
};

pub const Measure = struct {
    notes: []const Note,
    beats_per_measure: u8,
    beat_unit: u8,
    bpm: f32,
};

const std = @import("std");
const Instrument = @import("Instrument.zig");
const score: type = @import("score.zig");

pub fn init(comptime headers: []const []const u8, comptime instruments: []const Instrument) type {
    const instrEnum = blk: {
        var fields: [instruments.len]std.builtin.Type.EnumField = undefined;

        inline for (instruments, 0..) |instrument, i| {
            fields[i] = .{
                .name = instrument.name[0.. :0],
                .value = i,
            };
        }

        break :blk @Type(.{ .@"enum" = .{
            .tag_type = u32,
            .fields = &fields,
            .decls = &.{},
            .is_exhaustive = true,
        } });
    };

    return struct {
        headers: []const []const u8,
        instruments: []const Instrument,
        // tracks: []const Track,

        const Self = @This();

        pub const InstrumentEnum = instrEnum;

        pub const Track = struct {
            name: []const u8,
            instrument: InstrumentEnum,
            measures: []const score.Measure,
            state: TrackState,
        };

        pub const TrackState = struct {
            current_measure: u32,
            current_event_in_measure: u32,
            next_event_time: f64,
        };

        pub const init: Self = .{
            .headers = headers,
            .instruments = instruments,
        };

        pub fn buildOrchestra(self: Self) []const u8 {
            return blk: {
                comptime var result: []const u8 = "";

                inline for (self.headers) |header| {
                    result = result ++ header ++ "\n";
                }

                inline for (self.instruments) |instrument| {
                    result = result ++ instrument.buildInstr() ++ "\n";
                }

                break :blk result;
            };
        }
    };
}

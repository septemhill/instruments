const Self = @This();

name: []const u8,
body: []const []const u8,

pub fn init(name: []const u8, body: []const []const u8) @This() {
    return .{
        .name = name,
        .body = body,
    };
}

pub fn buildInstr(self: Self) []const u8 {
    return blk: {
        comptime var result: []const u8 = "";

        result = result ++ "instr " ++ self.name ++ "\n";

        inline for (self.body) |line| {
            result = result ++ line ++ "\n";
        }

        result = result ++ "endin\n";

        break :blk result;
    };
}

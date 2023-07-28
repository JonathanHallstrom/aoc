const std = @import("std");

const complex = @Vector(2, i64);

fn part_one(allocator: std.mem.Allocator, input: std.ArrayList(complex)) !i64 {
    var seen = std.AutoArrayHashMap(complex, void).init(allocator);
    defer seen.deinit();

    var pos = complex{ 0, 0 };
    try seen.put(pos, {});

    for (input.items) |movement| {
        pos += movement;
        try seen.put(pos, {});
    }

    return @intCast(seen.count());
}

fn part_two(allocator: std.mem.Allocator, input: std.ArrayList(complex)) !i64 {
    var seen = std.AutoArrayHashMap(complex, void).init(allocator);
    defer seen.deinit();

    var santa_pos = complex{ 0, 0 };
    var robo_santa_pos = complex{ 0, 0 };

    try seen.put(santa_pos, {});
    try seen.put(robo_santa_pos, {});

    // v----------------------------------------------------v
    for (0..input.items.len / 2) |i| {
        const santa_update = input.items[2 * i];
        const robo_santa_update = input.items[2 * i + 1];
    // ^----------------------------------------------------^
    // this part seems like it could be improved, using something similar to https://doc.rust-lang.org/std/primitive.slice.html#method.chunks

        santa_pos += santa_update;
        robo_santa_pos += robo_santa_update;

        try seen.put(santa_pos, {});
        try seen.put(robo_santa_pos, {});
    }

    return @intCast(seen.count());
}

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();
    const stdin_file = std.io.getStdIn().reader();
    var stdin = std.io.bufferedReader(stdin_file);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const input = try stdin.reader().readAllAlloc(allocator, 16384);
    defer allocator.free(input);

    var moves = try std.ArrayList(complex).initCapacity(allocator, input.len);
    defer moves.deinit();
    for (input) |char| {
        moves.appendAssumeCapacity(switch (char) {
            '^' => complex{ 1, 0 },
            'v' => complex{ -1, 0 },
            '<' => complex{ 0, -1 },
            '>' => complex{ 0, 1 },
            else => unreachable,
        });
    }

    try stdout.print("Part one: {!}\n", .{part_one(allocator, moves)});
    try stdout.print("Part two: {!}\n", .{part_two(allocator, moves)});

    try bw.flush(); // don't forget to flush!
}

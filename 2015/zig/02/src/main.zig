const std = @import("std");

fn part_one(input: std.ArrayList([3]i64)) !i64 {
    var result: i64 = 0;

    for (input.items) |side_lengths| {
        for (0..3) |i| {
            for (i + 1..3) |j| {
                result += side_lengths[i] * side_lengths[j] * 2;
            }
        }

        result += side_lengths[0] * side_lengths[1];
    }

    return result;
}

fn part_two(input: std.ArrayList([3]i64)) !i64 {
    var result: i64 = 0;

    for (input.items) |side_lengths| {
        var product: i64 = 1;
        for (side_lengths) |side_length| {
            product *= side_length;
        }
        result += product;

        result += 2 * (side_lengths[0] + side_lengths[1]);
    }

    return result;
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

    var presents = std.ArrayList([3]i64).init(allocator);
    defer presents.deinit();

    var lines = std.mem.split(u8, input, "\n");
    while (lines.next()) |line| {
        if (std.mem.eql(u8, line, ""))
            continue;
        var side_lengths: [3]i64 = undefined;
        var ints = std.mem.split(u8, line, "x");
        for (0..3) |i| {
            side_lengths[i] = try std.fmt.parseInt(i64, ints.next().?, 10);
        }
        std.mem.sort(i64, &side_lengths, {}, std.sort.asc(i64));
        try presents.append(side_lengths);
    }

    try stdout.print("Part one: {!}\n", .{part_one(presents)});
    try stdout.print("Part two: {!}\n", .{part_two(presents)});

    try bw.flush(); // don't forget to flush!
}

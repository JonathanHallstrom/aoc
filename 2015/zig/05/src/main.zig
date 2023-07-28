const std = @import("std");

fn contains(comptime T: type, arr: []const T, val: T) bool {
    return std.mem.indexOf(u8, arr, &[_]T{val}) != null;
}

fn contains_slice(comptime T: type, arr: []const T, val: []const T) bool {
    return std.mem.indexOf(u8, arr, val) != null;
}

fn is_nice_part_one(str: []const u8) bool {
    var vowel_count: i64 = 0;
    for (str) |chr| {
        vowel_count += @intFromBool(contains(u8, "aeiou", chr));
    }

    var contains_repeat = false;
    for (0..str.len - 1) |i| {
        contains_repeat = contains_repeat or str[i] == str[i + 1];
    }

    const doesnt_contain_forbidden_strings = !contains_slice(u8, str, "ab") and !contains_slice(u8, str, "cd") and !contains_slice(u8, str, "pq") and !contains_slice(u8, str, "xy");
    return vowel_count >= 3 and doesnt_contain_forbidden_strings and contains_repeat;
}

fn part_one(input: []u8) !usize {
    var lines = std.mem.tokenize(u8, input, "\n");
    var result: usize = 0;
    while (lines.next()) |line| {
        result += @intFromBool(is_nice_part_one(line));
    }
    return result;
}

fn is_nice_part_two(str: []const u8) bool {
    var first_pos: [256][256]usize = undefined;
    for (0..256) |i| {
        for (0..256) |j| {
            first_pos[i][j] = std.math.maxInt(usize);
        }
    }

    var found_repeating_str = false;

    for (1..str.len) |i| {
        const fp = &first_pos[str[i - 1]][str[i]];
        if (fp.* == std.math.maxInt(usize)) {
            fp.* = i;
        } else if (fp.* + 1 < i) {
            found_repeating_str = true;
            break;
        }
    }

    var found_surrounded_str = false;

    for (2..str.len) |i| {
        if (str[i - 2] == str[i]) {
            found_surrounded_str = true;
        }
    }

    return found_repeating_str and found_surrounded_str;
}

fn part_two(input: []u8) !usize {
    var lines = std.mem.tokenize(u8, input, "\n");
    var result: usize = 0;
    while (lines.next()) |line| {
        result += @intFromBool(is_nice_part_two(line));
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

    const input = try stdin.reader().readAllAlloc(allocator, 1 << 20);
    defer allocator.free(input);

    try stdout.print("Part one: {!}\n", .{part_one(input)});
    try stdout.print("Part two: {!}\n", .{part_two(input)});

    try bw.flush(); // don't forget to flush!
}

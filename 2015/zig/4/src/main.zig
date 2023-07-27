const std = @import("std");

const md5 = std.crypto.hash.Md5;

fn part_one(input: []u8) !usize {
    var buf1: [16]u8 = undefined;
    var buf2: [128]u8 = undefined;
    var buf3: [128]u8 = undefined;
    for (0..1_000_000_000) |i| {
        // buffers have a large amount of extra space
        const string_repr = std.fmt.bufPrint(&buf2, "{}", .{i}) catch unreachable;
        const msg = std.fmt.bufPrint(&buf3, "{s}{s}", .{ input, string_repr }) catch unreachable;

        md5.hash(msg, &buf1, .{});
        if (std.mem.eql(u8, std.fmt.bytesToHex(buf1, .lower)[0..5], "00000")) {
            return i;
        }
    }
    return 0;
}

fn part_two(input: []u8) !usize {
    var buf1: [16]u8 = undefined;
    var buf2: [128]u8 = undefined;
    var buf3: [128]u8 = undefined;
    for (0..1_000_000_000) |i| {
        // buffers have a large amount of extra space
        const string_repr = std.fmt.bufPrint(&buf2, "{}", .{i}) catch unreachable;
        const msg = std.fmt.bufPrint(&buf3, "{s}{s}", .{ input, string_repr }) catch unreachable;

        md5.hash(msg, &buf1, .{});
        if (std.mem.eql(u8, std.fmt.bytesToHex(buf1, .lower)[0..6], "000000")) {
            return i;
        }
    }
    return 0;
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

    try stdout.print("Part one: {!}\n", .{part_one(input)});
    try stdout.print("Part two: {!}\n", .{part_two(input)});

    try bw.flush(); // don't forget to flush!
}

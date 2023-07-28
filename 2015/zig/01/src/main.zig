const std = @import("std");

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

    var part_one: i64 = 0;
    var part_two: i64 = -1;

    for (input, 0..) |character, idx| {
        part_one += @intFromBool(character == '(');
        part_one -= @intFromBool(character == ')');
        if (part_one == -1 and part_two == -1)
            part_two = @intCast(idx + 1);
    }

    try stdout.print("Part one: {}\n", .{part_one});
    try stdout.print("Part two: {}\n", .{part_two});

    try bw.flush(); // don't forget to flush!
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}

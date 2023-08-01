const std = @import("std");

fn contains(comptime T: type, arr: []const T, val: T) bool {
    return std.mem.indexOf(u8, arr, &[_]T{val}) != null;
}

fn contains_slice(comptime T: type, arr: []const T, val: []const T) bool {
    return std.mem.indexOf(u8, arr, val) != null;
}

fn starts_with(comptime T: type, arr: []const T, val: []const T) bool {
    const idx = std.mem.indexOf(u8, arr, val);
    return idx != null and idx.? == 0;
}

fn parse_numbers(comptime T: type, allocator: std.mem.Allocator, buf: []const u8) !std.ArrayList(T) {
    comptime std.debug.assert(@typeInfo(T) == .Int);
    var result = std.ArrayList(T).init(allocator);
    var curr: ?T = null;
    for (buf) |c| {
        switch (c) {
            '0'...'9' => {
                curr = (if (curr != null) curr.? else 0) * 10 + c - '0';
            },
            else => {
                if (curr != null)
                    try result.append(curr.?);
                curr = null;
            },
        }
    }
    if (curr != null)
        try result.append(curr.?);
    return result;
}

const point = @Vector(2, usize);

const operation = union(enum) {
    toggle: [2]point,
    on: [2]point,
    off: [2]point,
};

fn part_one(input: []operation) !usize {
    var board: [1000][1000]bool = undefined;
    for (0..1000) |i| {
        for (0..1000) |j| {
            board[i][j] = false;
        }
    }
    for (input) |op| {
        switch (op) {
            .on => |bounds| {
                // dont like these loops, wish there was some way to make them inclusive
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] = true;
                    }
                }
            },
            .off => |bounds| {
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] = false;
                    }
                }
            },
            .toggle => |bounds| {
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] = !board[i][j];
                    }
                }
            },
        }
    }
    var result: usize = 0;
    for (board) |row| {
        for (row) |elem| {
            result += @intFromBool(elem);
        }
    }
    return result;
}

fn part_two(input: []operation) !usize {
    var board: [1000][1000]usize = undefined;
    for (0..1000) |i| {
        for (0..1000) |j| {
            board[i][j] = 0;
        }
    }
    for (input) |op| {
        switch (op) {
            .on => |bounds| {
                // dont like these loops, wish there was some way to make them inclusive
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] += 1;
                    }
                }
            },
            .off => |bounds| {
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] = @max(board[i][j], 1) - 1;
                    }
                }
            },
            .toggle => |bounds| {
                for (bounds[0][0]..1 + bounds[1][0]) |i| {
                    for (bounds[0][1]..1 + bounds[1][1]) |j| {
                        board[i][j] += 2;
                    }
                }
            },
        }
    }
    var result: usize = 0;
    for (board) |row| {
        for (row) |elem| {
            result += elem;
        }
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

    var operations = std.ArrayList(operation).init(allocator);
    defer operations.deinit();

    var lines = std.mem.tokenize(u8, input, "\n");

    while (lines.next()) |line| {
        var op: operation = undefined;
        const nums = try parse_numbers(usize, allocator, line);
        defer nums.deinit();
        std.debug.assert(nums.items.len == 4);
        const arr = [2]point{ point{ nums.items[0], nums.items[1] }, point{ nums.items[2], nums.items[3] } };
        if (starts_with(u8, line, "turn on")) {
            op = operation{ .on = arr };
        } else if (starts_with(u8, line, "turn off")) {
            op = operation{ .off = arr };
        } else if (starts_with(u8, line, "toggle")) {
            op = operation{ .toggle = arr };
        } else {
            unreachable;
        }

        try operations.append(op);
    }

    try stdout.print("Part one: {!}\n", .{part_one(operations.items)});
    try stdout.print("Part two: {!}\n", .{part_two(operations.items)});

    try bw.flush(); // don't forget to flush!
}

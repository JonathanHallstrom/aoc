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

fn parse_numbers_comptime_length(comptime T: type, comptime num_ints: usize, buf: []const u8) ![num_ints]T {
    comptime std.debug.assert(@typeInfo(T) == .Int);
    var result: [num_ints]T = undefined;
    var result_size: usize = 0;
    var curr: ?T = null;
    for (buf) |c| {
        switch (c) {
            '0'...'9' => {
                curr = (if (curr != null) curr.? else 0) * 10 + c - '0';
            },
            else => {
                if (curr != null) {
                    result[result_size] = curr.?;
                    result_size += 1;
                }
                curr = null;
            },
        }
    }
    if (curr != null) {
        result[result_size] = curr.?;
    }
    return result;
}

const logical_op = enum {
    NOT,
    LSHIFT,
    RSHIFT,
    AND,
    OR,
    ASSIGNMENT,
    UNINITIALIZED,
};

const operand = union(enum) {
    constant: usize,
    variable: usize,
};

const operation = struct { tp: logical_op = .UNINITIALIZED, first_operand: operand = .{ .constant = 1000000000 }, second_operand: ?operand = null, result_operand: operand = .{ .constant = 1000000000 } };

fn parse(s: []const u8) operand {
    return .{ .constant = std.fmt.parseInt(usize, s, 10) catch {
        var result: usize = 0;
        for (s) |c| {
            result = result * 26 + c - 'a' + 1;
        }
        return .{ .variable = result };
    } };
}

var cache: [26 * 26 + 1]?u64 = undefined;

fn compute(x: usize, source: [26 * 26 + 1]operation) usize {
    if (cache[x] == null) {
        const first = switch (source[x].first_operand) {
            .constant => |c| c,
            .variable => |v| compute(v, source),
        };
        const second = if (source[x].second_operand != null) switch (source[x].second_operand.?) {
            .constant => |c| c,
            .variable => |v| compute(v, source),
        } else 0;

        std.debug.assert(source[x].tp != .UNINITIALIZED);

        cache[x] = switch (source[x].tp) {
            .ASSIGNMENT => first,
            .NOT => ~first,
            .AND => first & second,
            .OR => first | second,
            .LSHIFT => first << @intCast(second),
            .RSHIFT => first >> @intCast(second),
            .UNINITIALIZED => unreachable,
        } & 0xFFFF;
    }
    return cache[x].?;
}

fn part_one(input: []operation) !usize {
    var source: [26 * 26 + 1]operation = undefined;
    for (0..26 * 26 + 1) |i| {
        source[i] = .{};
    }
    for (input) |op| {
        source[op.result_operand.variable] = op;
    }
    for (0..cache.len) |i| {
        cache[i] = null;
    }
    return compute(1, source);
}

fn part_two(input: []operation) !usize {
    var source: [26 * 26 + 1]operation = undefined;
    for (0..26 * 26 + 1) |i| {
        source[i] = .{};
    }
    for (input) |op| {
        source[op.result_operand.variable] = op;
    }
    for (0..cache.len) |i| {
        cache[i] = null;
    }
    const a_outp = compute(1, source);

    for (0..cache.len) |i| {
        cache[i] = null;
    }

    cache[2] = a_outp;
    return compute(1, source);
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
        var op: operation = .{};
        var token_iter = std.mem.tokenize(u8, line, " ");

        inline for ([_]logical_op{ .NOT, .LSHIFT, .RSHIFT, .AND, .OR, .ASSIGNMENT }) |tp| {
            if (contains_slice(u8, line, @tagName(tp))) {
                op.tp = tp;
                break;
            }
        }
        if (op.tp == .UNINITIALIZED)
            op.tp = .ASSIGNMENT;

        if (op.tp == .NOT) {
            _ = token_iter.next();
        }
        op.first_operand = parse(token_iter.next().?);
        if (op.tp != .NOT and op.tp != .ASSIGNMENT) {
            _ = token_iter.next();
            op.second_operand = parse(token_iter.next().?);
        }
        _ = token_iter.next();
        op.result_operand = parse(token_iter.next().?);

        try operations.append(op);
    }

    try stdout.print("Part one: {!}\n", .{part_one(operations.items)});
    try stdout.print("Part two: {!}\n", .{part_two(operations.items)});

    try bw.flush(); // don't forget to flush!
}

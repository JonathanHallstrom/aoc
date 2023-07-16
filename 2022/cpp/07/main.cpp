#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

struct directory {
    std::map<std::string, std::unique_ptr<directory>> subdirectories;
    std::map<std::string, int> files;
    directory *parent = nullptr;
};

int get_size(directory const *dir) {
    int result = 0;
    for (auto const &[name, dir]: dir->subdirectories)
        result += get_size(dir.get());
    for (auto const &[name, size]: dir->files)
        result += size;
    return result;
}

int part_one(directory const *root) {
    int result = 0;

    if (int curr_directory_size = get_size(root); curr_directory_size < 100'000)
        result += curr_directory_size;

    for (auto const &[name, dir]: root->subdirectories)
        result += part_one(dir.get());

    return result;
}

int part_two_helper(directory *root, int amount_to_free) {
    int result = 1e9;

    if (int curr_directory_size = get_size(root); curr_directory_size >= amount_to_free)
        result = curr_directory_size;

    for (auto const &[name, dir]: root->subdirectories)
        result = std::min(part_two_helper(dir.get(), amount_to_free), result);

    return result;
}

int part_two(directory *root, int total_space, int space_needed_for_update) {
    const int used_space = get_size(root);
    const int remaining_space = total_space - used_space;
    const int amount_to_free = space_needed_for_update - remaining_space;
    if (amount_to_free > 0) {
        return part_two_helper(root, amount_to_free);
    } else {
        return 0;
    }
}

void print_tree(directory const *root, std::string padding = " - ") {
    for (auto const &[name, dir]: root->subdirectories) {
        std::cout << padding << name << " (dir, size=" << get_size(dir.get()) << ")\n";
        print_tree(dir.get(), "  " + padding);
    }
    for (auto const &[name, size]: root->files) {
        std::cout << padding << name << " (file, size=" << size << ")\n";
    }
}

int main() {
    // for pretty printing the tree
    directory above_root;
    above_root.subdirectories["/"] = std::make_unique<directory>(directory{.parent = &above_root});

    directory *const root = above_root.subdirectories["/"].get();
    directory *curr = root;

    std::string input;
    while (std::getline(std::cin, input)) {
        std::istringstream iss(input);
        std::string type_or_size;
        iss >> type_or_size;
        if (type_or_size == "$") {
            std::string command_type;
            iss >> command_type;
            if (command_type == "cd") {
                std::string new_directory;
                iss >> new_directory;
                if (new_directory == "/") {
                    curr = root;
                } else if (new_directory == "..") {
                    curr = curr->parent;
                } else {
                    curr = curr->subdirectories.at(new_directory).get();
                }
            }
        } else if (type_or_size == "dir") {
            std::string name;
            iss >> name;
            curr->subdirectories.insert_or_assign(std::move(name), std::make_unique<directory>(directory{.parent = curr}));
        } else {
            std::string name;
            iss >> name;
            int size = std::stoi(type_or_size);
            curr->files.insert_or_assign(std::move(name), size);
        }
    }

    print_tree(&above_root);

    std::cout << "Part one: " << part_one(root) << '\n';
    std::cout << "Part two: " << part_two(root, 70'000'000, 30'000'000) << '\n';
}
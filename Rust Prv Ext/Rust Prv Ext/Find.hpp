#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <unordered_map>

std::string Path = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Rust\\Bundles\\items";

std::vector<uint64_t> FindPng(const std::string& ItemName) {
    std::string FilePath = Path + "\\" + ItemName;
    std::ifstream file(FilePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return {};
    }

    std::streamsize size = file.tellg(); // Get file size
    file.seekg(0, std::ios::beg);        // Move file pointer back to beginning

    std::size_t num_uint64 = (size + sizeof(uint64_t) - 1) / sizeof(uint64_t);

    std::vector<uint64_t> data(num_uint64);

    file.read(reinterpret_cast<char*>(data.data()), size);

    if (file.bad()) {
        return {};
    }

    file.close();

    return data;
}
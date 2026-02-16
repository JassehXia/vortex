#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// ... (previous headers)
#include "../include/InvertedIndex.hpp"

// Namespace alias for convenience
namespace fs = std::filesystem;

void index_directory(const std::string &directory_path) {
  std::cout << "Indexing directory: " << directory_path << "\n";

  if (!fs::exists(directory_path)) {
    std::cerr << "Error: Directory does not exist.\n";
    return;
  }

  InvertedIndex index;
  int file_id_counter = 0;
  std::vector<std::string> file_paths; // Map ID -> Path

  auto start_time = std::chrono::high_resolution_clock::now();

  // C++17 filesystem iterator
  for (const auto &entry : fs::recursive_directory_iterator(directory_path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      std::string path_str = entry.path().string();

      // Read file content
      std::ifstream file(path_str);
      std::stringstream buffer;
      buffer << file.rdbuf();

      index.add_document(file_id_counter, buffer.str());
      file_paths.push_back(path_str);

      file_id_counter++;
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  std::cout << "Indexed " << file_id_counter << " files in " << duration.count()
            << "ms.\n";

  // Save the index
  std::string index_file = "vortex.index";
  std::cout << "Saving index to " << index_file << "...\n";
  index.save(index_file);

  // Load it back to verify
  std::cout << "Verifying load...\n";
  InvertedIndex loaded_index;
  loaded_index.load(index_file);

  // Quick test search on LOADED index
  std::string query = "search";
  std::cout << "\nSearching for '" << query << "' in loaded index:\n";
  auto results = loaded_index.search(query);
  for (const auto &res : results) {
    std::cout << " - File: " << file_paths[res.file_id]
              << " (Freq: " << res.frequency << ")\n";
  }
}

int main(int argc, char *argv[]) {
  // ...
  if (argc < 2) {
    std::cerr << "Usage: vortex_indexer <directory_path>\n";
    return 1;
  }

  std::string path = argv[1];
  index_directory(path);

  return 0;
}

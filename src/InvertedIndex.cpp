#include "../include/InvertedIndex.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

void InvertedIndex::add_document(int file_id, const std::string &content) {
  std::unordered_map<std::string, int> word_counts;
  std::stringstream ss(content);
  std::string word;

  // Simple tokenizer: split by whitespace
  while (ss >> word) {
    // Normalize: remove punctuation and convert to lowercase
    // This is a naive implementation; we'll optimize this later with SIMD?
    word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    if (!word.empty()) {
      word_counts[word]++;
    }
  }

  // Thread-safe insertion
  // We lock the mutex because multiple threads might try to modify 'index' at
  // once
  std::lock_guard<std::mutex> lock(index_mutex);

  for (const auto &[term, count] : word_counts) {
    index[term].push_back({file_id, count});
  }
}

std::vector<DocFrequency> InvertedIndex::search(const std::string &term) const {
  // Thread-safe read (though standard map reads are usually safe if no writes
  // happen)
  std::lock_guard<std::mutex> lock(index_mutex);

  // Normalize input term to match index
  std::string search_term = term;
  std::transform(search_term.begin(), search_term.end(), search_term.begin(),
                 ::tolower);

  if (auto it = index.find(search_term); it != index.end()) {
    return it->second;
  }

  return {}; // Return empty vector if not found
}

void InvertedIndex::save(const std::string &filename) const {
  std::lock_guard<std::mutex> lock(index_mutex);
  std::ofstream out_file(filename, std::ios::binary);

  if (!out_file) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return;
  }

  // 1. Write the size of the map (how many unique terms?)
  size_t map_size = index.size();
  out_file.write(reinterpret_cast<const char *>(&map_size), sizeof(map_size));

  for (const auto &[term, docs] : index) {
    // 2. Write the string length, then the string characters
    size_t term_len = term.size();
    out_file.write(reinterpret_cast<const char *>(&term_len), sizeof(term_len));
    out_file.write(term.c_str(), term_len);

    // 3. Write the vector size, then the vector data block
    size_t vec_size = docs.size();
    out_file.write(reinterpret_cast<const char *>(&vec_size), sizeof(vec_size));

    // Direct memory dump of the vector (FAST!)
    out_file.write(reinterpret_cast<const char *>(docs.data()),
                   vec_size * sizeof(DocFrequency));
  }
}

void InvertedIndex::load(const std::string &filename) {
  std::lock_guard<std::mutex> lock(index_mutex);
  index.clear();

  std::ifstream in_file(filename, std::ios::binary);
  if (!in_file) {
    std::cerr << "Error opening file for reading: " << filename << std::endl;
    return;
  }

  // 1. Read how many terms we have
  size_t map_size;
  in_file.read(reinterpret_cast<char *>(&map_size), sizeof(map_size));

  for (size_t i = 0; i < map_size; ++i) {
    // 2. Read string length, then string
    size_t term_len;
    in_file.read(reinterpret_cast<char *>(&term_len), sizeof(term_len));

    std::string term(term_len, '\0'); // Pre-allocate string buffer
    in_file.read(&term[0], term_len);

    // 3. Read vector size, then vector data
    size_t vec_size;
    in_file.read(reinterpret_cast<char *>(&vec_size), sizeof(vec_size));

    std::vector<DocFrequency> docs(vec_size);
    in_file.read(reinterpret_cast<char *>(docs.data()),
                 vec_size * sizeof(DocFrequency));

    index[term] = std::move(docs);
  }
}

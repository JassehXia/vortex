#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

// A simple structure to hold the frequency of a term in a specific document
struct DocFrequency {
  int file_id;
  int frequency;
};

class InvertedIndex {
public:
  InvertedIndex() = default;

  // Adds a document's content to the index
  // file_id: Unique identifier for the file (we'll map this to the path later)
  // content: The full text content of the file
  void add_document(int file_id, const std::string &content);

  // search for a term and return a list of documents containing it
  // term: The word to search for
  std::vector<DocFrequency> search(const std::string &term) const;

  // Save the index to a binary file
  void save(const std::string &filename) const;

  // Load the index from a binary file
  void load(const std::string &filename);

private:
  // The core data structure: A map from "term" -> List of {FileID, Frequency}
  // usage of unordered_map gives us O(1) average time complexity for lookups
  std::unordered_map<std::string, std::vector<DocFrequency>> index;

  // Mutex to protect the index during parallel insertions (for later stages)
  mutable std::mutex index_mutex;
};

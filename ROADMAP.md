# Vortex Project Roadmap & Implementation Plan

## 1. Project Overview
Vortex is a high-performance, local Search Engine and retrieval-augmented generation (RAG) platform designed to provide instantaneous, private access to vast personal data libraries.

**Core Value Proposition:**
- **Speed:** Sub-10ms retrieval times using C++ indexing.
- **Privacy:** 100% local processing with local LLMs (Ollama).
- **Architecture:** Hybrid C++ (indexing) + Python (LLM/API) approach.

---

## 2. Implementation Roadmap (Phased Rollout)

The project is divided into 5 distinct MVP stages to ensure incremental delivery of value.

### Phase 1: MVP 1 (The Engine Room)
**Goal:** Build the standalone C++ CLI tool that indexes a folder of `.txt` files and outputs a searchable binary file.
- [X] Set up C++20 project structure with CMake.
- [X] Implement file system scanner to read `.txt` files recursively.
- [X] Develop the Inverted Index data structure (Project `term` -> `file_id` + `frequency`).
- [X] Implement the `mmap` logic for writing/reading large index files to/from disk.
- [X] Create a CLI interface to accept a directory path and output index stats.
- [X] **Deliverable:** A compiled binary (`vortex-indexer`) that takes a folder path and successfully creates an index file.

### Phase 2: MVP 2 (The Connectivity Bridge)
**Goal:** Implement the pybind11 wrapper and verify that Python can query the C++ index in under 10ms.
- [ ] create `setup.py` and configure pybind11 bindings.
- [ ] Expose C++ `Index` class methods (`load_index`, `search`) to Python.
- [ ] Implement BM25 ranking algorithm within the C++ search logic.
- [ ] Create a Python script to import the module, load the index, and run benchmark queries.
- [ ] **Deliverable:** A Python script that prints search results from the C++ index with execution time < 10ms.

### Phase 3: MVP 3 (The API & UI)
**Goal:** Set up the FastAPI server and a basic React dashboard to display search results.
- [ ] Initialize FastAPI project structure.
- [ ] Create API endpoints:
    - `POST /index`: Trigger indexing of a directory.
    - `GET /search?q=...`: Query the index and return ranked snippets.
- [ ] Initialize React + Tailwind CSS project.
- [ ] Build the "Command Center" UI:
    - Search bar.
    - Result list with highlighted snippets.
    - "Indexing status" indicator.
- [ ] **Deliverable:** A functional web app where users can type a query and see local file results instantly.

### Phase 4: MVP 4 (Local AI Integration)
**Goal:** Integrate Ollama to take search results and generate natural language answers.
- [ ] Set up local Ollama instance (Llama 3 or Mistral).
- [ ] Implement the "Context Prompt" builder in Python:
    - Retrieve top K chunks from C++ index.
    - Format them into a system prompt.
- [ ] Connect FastAPI to Ollama API.
- [ ] Update UI to show "AI Answer" above search results.
- [ ] **Deliverable:** A "Chat with your Data" experience where the LLM answers questions based on indexed files.

### Phase 5: MVP 5 (The Power User Update)
**Goal:** Add support for PDF parsing and "Live Watch" mode.
- [ ] Integrate a C++ or Python library for PDF text extraction.
- [ ] Implement a file system watcher (e.g., `watchdog` in Python or platform-native API in C++) to detect file changes.
- [ ] Create "Incremental Update" logic in the C++ engine to add/remove files without full re-indexing.
- [ ] Add "Citation Engine" to UI (clickable references).
- [ ] **Deliverable:** A fully robust system that handles live file updates and complex document formats.

---

## 3. Technical Breakdown

### Stage 1: The High-Throughput Indexer (C++)
- **Language:** C++20 (STL, SIMD optimizations).
- **Concurrency:** `std::jthread` for parallel file reading.
- **Memory Management:** `mmap` for zero-copy access to large index files.
- **Output:** Binary Inverted Index file.

### Stage 2: The Python-to-C++ Bridge
- **Tool:** pybind11.
- **Function:** Exposes high-performance C++ search functions as native Python methods.
- **Optimization:** Minimizes data copying between C++ and Python runtimes.

### Stage 3: Contextual Retrieval (RAG)
1.  **Keyword Scoring:** C++ engine uses BM25 to rank document chunks.
2.  **LLM Synthesis:** Top-ranked snippets are sent to the local LLM (Ollama) to generate the final answer.

### Stage 4: Semantic Management
- **Dynamic Re-indexing:** Background "Watcher" service for real-time updates.
- **Citation Engine:** UI links LLM claims to source documents.

---

## 4. Tech Stack Overview
| Component | Technology |
| :--- | :--- |
| **Indexing Engine** | C++20 |
| **Bridge** | pybind11 |
| **Backend** | Python 3.12, FastAPI |
| **Frontend** | React, Tailwind CSS |
| **AI / LLM** | Ollama (Llama 3 / Mistral) |
| **Build System** | CMake |

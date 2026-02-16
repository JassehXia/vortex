# Vortex

**Vortex** is a high-performance, local Search Engine and Retrieval-Augmented Generation (RAG) platform. It combines the raw speed of C++ indexing with the flexibility of Python-driven LLMs to provide instantaneous, private access to your personal data libraries.

## 🚀 Key Features

*   **Sub-10ms Retrieval:** Custom C++ Inverted Indexer using `mmap` and multithreading.
*   **100% Privacy:** Runs entirely offline with local LLMs (Ollama + Llama 3/Mistral).
*   **Hybrid Architecture:** C++ core for heavy lifting, Python/FastAPI for orchestration.
*   **Zero Latency:** "Hardware-Aware" design using C++20 and SIMD optimization.

## 🛠️ Tech Stack

*   **Core Engine:** C++20 (STL, `std::jthread`, `mmap`)
*   **Bridge:** pybind11
*   **Backend:** Python 3.12, FastAPI
*   **Frontend:** React, Tailwind CSS
*   **AI:** Ollama
*   **Build:** CMake

## 📂 Project Structure

```
vortex/
├── src/            # C++ Source Code (Indexer)
├── python/         # Python Wrapper & Backend
├── frontend/       # React Dashboard
├── docs/           # Documentation
└── tests/          # Unit & Integration Tests
```

## 🏁 Getting Started

### Prerequisites
*   CMake 3.15+
*   C++20 compliant compiler (GCC/Clang/MSVC)
*   Python 3.12+
*   Node.js & npm
*   Ollama

### Building the Project
*(Instructions to be added as MVPs are completed)*

1.  **Clone the repo:**
    ```bash
    git clone https://github.com/yourusername/vortex.git
    cd vortex
    ```

2.  **Build the C++ Engine:**
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

## 🗺️ Roadmap

See [ROADMAP.md](./ROADMAP.md) for the detailed implementation plan.

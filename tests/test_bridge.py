import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

import vortex
import time

def test_vortex():
    print("Testing Vortex Python Bridge...")
    
    # Initialize Index
    index = vortex.InvertedIndex()
    
    # Add documents
    docs = [
        "This is the first document about search engines",
        "Python is great for glue code but C++ is fast",
        "Search engines use inverted indexes for speed",
        "Vortex combines C++ and Python"
    ]
    
    start = time.time()
    for i, doc in enumerate(docs):
        index.add_document(i, doc)
    end = time.time()
    print(f"Indexed {len(docs)} docs in {(end-start)*1000:.4f} ms")
    
    # Search
    query = "search"
    results = index.search(query)
    print(f"\nSearch results for '{query}':")
    for res in results:
        print(f" - Doc ID: {res.file_id}, Freq: {res.frequency}")
        
    # Verify results
    # "search" appears in doc 0 and doc 2
    doc_ids = [r.file_id for r in results]
    assert 0 in doc_ids
    assert 2 in doc_ids
    print("\n[SUCCESS] Search logic verification passed!")

    # Test Save/Load
    index.save("test_py.index")
    loaded = vortex.InvertedIndex()
    loaded.load("test_py.index")
    results_loaded = loaded.search(query)
    assert len(results_loaded) == len(results)
    print("[SUCCESS] Serialization verification passed!")
    
    # Cleanup
    if os.path.exists("test_py.index"):
        os.remove("test_py.index")

if __name__ == "__main__":
    test_vortex()

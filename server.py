from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import List
import vortex
import os

app = FastAPI(title="Vortex Search Engine")

# --- Global State ---
# In a real app, we might handle this more gracefully, but for MVP
# a global variable is fine.
index = vortex.InvertedIndex()
INDEX_FILE = "vortex.index"

# Load existing index on startup if available
if os.path.exists(INDEX_FILE):
    print(f"Loading existing index from {INDEX_FILE}...")
    index.load(INDEX_FILE)

# --- Request Models ---
class IndexRequest(BaseModel):
    directory: str

class SearchResult(BaseModel):
    file_id: int
    frequency: int
    # In a real app, we'd map file_id -> actual filename here
    
# --- Endpoints ---

@app.post("/index")
async def trigger_indexing(request: IndexRequest):
    if not os.path.exists(request.directory):
        raise HTTPException(status_code=404, detail="Directory not found")
    
    # scan for .txt files recursively
    count = 0 
    for root, dirs, files in os.walk(request.directory):
        for file in files:
            if file.endswith(".txt"):
                path = os.path.join(root, file)
                try:
                    with open(path, "r", encoding="utf-8") as f:
                        content = f.read()
                        # TODO: We need a mapping of ID -> Filename in Python too!
                        # For now, just using a simple counter isn't enough because
                        # we need to persistently know which ID is which file.
                        # Let's fix this in the next step.
                        index.add_document(count, content)
                        count += 1
                except Exception as e:
                    print(f"Skipping {path}: {e}")

    index.save(INDEX_FILE)
    return {"message": f"Successfully indexed {count} files", "index_saved": True}

@app.get("/search", response_model=List[SearchResult])
async def search(q: str):
    if not q:
        return []
    
    results = index.search(q)
    
    # Convert C++ binding objects to Pydantic models
    response = []
    for res in results:
        response.append(SearchResult(file_id=res.file_id, frequency=res.frequency))
        
    # Sort by frequency (descending)
    response.sort(key=lambda x: x.frequency, reverse=True)
    return response

@app.get("/health")
async def health_check():
    return {"status": "ok", "engine": "running"}

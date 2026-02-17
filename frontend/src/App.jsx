import { useState } from 'react'

function App() {
  const [query, setQuery] = useState('')
  const [results, setResults] = useState([])
  const [indexing, setIndexing] = useState(false)
  const [indexDir, setIndexDir] = useState('')
  const [msg, setMsg] = useState('')

  const handleSearch = async (e) => {
    e.preventDefault()
    if (!query) return
    try {
      const res = await fetch(`http://localhost:8000/search?q=${query}`)
      const data = await res.json()
      setResults(data)
    } catch (err) {
      console.error(err)
    }
  }

  const handleIndex = async () => {
    if (!indexDir) return
    setIndexing(true)
    setMsg('Indexing...')
    try {
      const res = await fetch('http://localhost:8000/index', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ directory: indexDir })
      })
      const data = await res.json()
      setMsg(data.message)
    } catch (err) {
      setMsg('Error indexing: ' + err.message)
    }
    setIndexing(false)
  }

  return (
    <div className="min-h-screen p-8 max-w-4xl mx-auto">
      <h1 className="text-4xl font-bold mb-8 bg-gradient-to-r from-cyan-400 to-blue-500 bg-clip-text text-transparent">
        Vortex Engine
      </h1>

      {/* Indexing Section */}
      <div className="bg-slate-800 p-6 rounded-lg mb-8 border border-slate-700">
        <h2 className="text-xl font-semibold mb-4 text-slate-200">Index Data</h2>
        <div className="flex gap-4">
          <input
            type="text"
            value={indexDir}
            onChange={(e) => setIndexDir(e.target.value)}
            placeholder="Absolute folder path (e.g. C:/Users/.../vortex/data)"
            className="flex-1 bg-slate-900 border border-slate-600 rounded px-4 py-2 text-slate-200 focus:outline-none focus:border-cyan-500"
          />
          <button
            onClick={handleIndex}
            disabled={indexing}
            className="bg-cyan-600 hover:bg-cyan-500 text-white px-6 py-2 rounded font-medium disabled:opacity-50"
          >
            {indexing ? 'Indexing...' : 'Index'}
          </button>
        </div>
        {msg && <p className="mt-2 text-sm text-cyan-300">{msg}</p>}
      </div>

      {/* Search Section */}
      <div className="bg-slate-800 p-6 rounded-lg border border-slate-700 min-h-[400px]">
        <h2 className="text-xl font-semibold mb-4 text-slate-200">Search</h2>
        <form onSubmit={handleSearch} className="flex gap-4 mb-6">
          <input
            type="text"
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            placeholder="Search query..."
            className="flex-1 bg-slate-900 border border-slate-600 rounded px-4 py-2 text-slate-200 focus:outline-none focus:border-cyan-500"
          />
          <button
            type="submit"
            className="bg-blue-600 hover:bg-blue-500 text-white px-8 py-2 rounded font-medium"
          >
            Search
          </button>
        </form>

        <div className="space-y-4">
          {results.map((res, i) => (
            <div key={i} className="bg-slate-900 p-4 rounded border border-slate-700 hover:border-cyan-500/50 transition-colors">
              <div className="flex justify-between items-start">
                <h3 className="text-lg font-medium text-cyan-400">Doc ID: {res.file_id}</h3>
                <span className="text-xs bg-slate-700 px-2 py-1 rounded text-slate-300">Matches: {res.frequency}</span>
              </div>
              <p className="text-slate-400 text-sm mt-1">
                (Content preview not yet implemented in MVP 3)
              </p>
            </div>
          ))}
          {results.length === 0 && query && (
            <p className="text-slate-500 text-center mt-8">No results found.</p>
          )}
        </div>
      </div>
    </div>
  )
}

export default App

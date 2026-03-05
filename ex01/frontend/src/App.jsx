import { useState } from 'react'
import './App.css'

const API_BASE = 'http://localhost:8080/api'

function App() {
  const [helloMessage, setHelloMessage] = useState('')
  const [userInput, setUserInput] = useState('')
  const [echoResult, setEchoResult] = useState('')
  const [loading, setLoading] = useState(false)
  const [mode, setMode] = useState('detecting')

  const fetchHello = async () => {
    try {
      const res = await fetch(`${API_BASE}/hello`)
      const data = await res.json()
      setHelloMessage(data.message)
      setMode('api')
    } catch {
      setHelloMessage('Hello, World!')
      setMode('local')
    }
  }

  const submitInput = async () => {
    if (!userInput.trim()) return
    setLoading(true)
    try {
      const res = await fetch(`${API_BASE}/echo`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ input: userInput }),
      })
      const data = await res.json()
      setEchoResult(data.echo)
    } catch {
      setEchoResult('You entered: ' + userInput)
    } finally {
      setLoading(false)
    }
  }

  const handleKeyDown = (e) => {
    if (e.key === 'Enter') submitInput()
  }

  return (
    <div className="app">
      <header className="header">
        <h1>Ex01 - HelloWorld</h1>
        <p className="subtitle">Java Spring Boot + React</p>
        {mode !== 'detecting' && (
          <span className={`badge ${mode}`}>
            {mode === 'api' ? 'API Connected' : 'Standalone Mode'}
          </span>
        )}
      </header>

      <main className="main">
        <section className="card">
          <h2>Hello World</h2>
          <p className="description">
            Click the button to fetch a greeting from the Spring Boot backend.
          </p>
          <button className="btn" onClick={fetchHello}>
            Say Hello
          </button>
          {helloMessage && (
            <div className="result">{helloMessage}</div>
          )}
        </section>

        <section className="card">
          <h2>Keyboard Input</h2>
          <p className="description">
            Enter text below and submit to echo it back via the backend API.
          </p>
          <div className="input-group">
            <input
              type="text"
              className="input"
              placeholder="Type something..."
              value={userInput}
              onChange={(e) => setUserInput(e.target.value)}
              onKeyDown={handleKeyDown}
            />
            <button className="btn" onClick={submitInput} disabled={loading}>
              {loading ? 'Sending...' : 'Submit'}
            </button>
          </div>
          {echoResult && (
            <div className="result">{echoResult}</div>
          )}
        </section>
      </main>

      <footer className="footer">
        <p>class_demo / ex01 &mdash; Built with Spring Boot 3.2 &amp; React 19</p>
      </footer>
    </div>
  )
}

export default App

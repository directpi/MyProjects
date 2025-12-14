import React, { useState } from 'react';
import MazeCanvas from './MazeCanvas';
import './App.css';

function App() {
  const [mazeData, setMazeData] = useState(null);

  const [rows, setRows] = useState(10);
  const [cols, setCols] = useState(15);
  const [selectedFile, setSelectedFile] = useState(null);

  const [startPoint, setStartPoint] = useState(null);
  const [endPoint, setEndPoint] = useState(null);
  const [path, setPath] = useState(null);

  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState('');

  const [saveFilename, setSaveFilename] = useState('maze.txt');

  const resetOutputState = () => {
    setError('');
    setMazeData(null);
    setStartPoint(null);
    setEndPoint(null);
    setPath(null);
  };

  const handleGenerate = async () => {
    setIsLoading(true);
    resetOutputState();
    try {
      const url = `http://localhost:8080/api/maze/generate?rows=${rows}&cols=${cols}`;
      const response = await fetch(url);
      const responseData = await response.json();
      if (!response.ok) {
        throw new Error(responseData.error || 'Ошибка при генерации лабиринта');
      }
      setMazeData(responseData);
    } catch (err) {
      setError(err.message);
    } finally {
      setIsLoading(false);
    }
  };

  const handleFileChange = (event) => {
    const file = event.target.files[0];
    setSelectedFile(file);
    resetOutputState();
  };

  const handleSubmit = async () => {
    if (!selectedFile) return;
    setIsLoading(true);
    resetOutputState();
    const formData = new FormData();
    formData.append('maze_file', selectedFile);
    try {
      const response = await fetch('http://localhost:8080/api/maze/load', { method: 'POST', body: formData });
      const responseData = await response.json();
      if (!response.ok) throw new Error(responseData.error || 'Ошибка при загрузке файла');
      setMazeData(responseData);
    } catch (err) {
      setError(err.message);
    } finally {
      setIsLoading(false);
    }
  };

  const handleCanvasClick = (point) => {
    setPath(null);
    if (!startPoint) {
      setStartPoint(point);
    } else if (!endPoint) {
      setEndPoint(point);
    } else {
      setStartPoint(point);
      setEndPoint(null);
    }
  };

  const handleSolveMaze = async () => {
    if (!startPoint || !endPoint) return;
    setIsLoading(true);
    setError('');
    try {
      const startArray = [startPoint.row, startPoint.col];
      const endArray = [endPoint.row, endPoint.col];
      const response = await fetch('http://localhost:8080/api/maze/solve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ start: startArray, end: endArray }),
      });
      const responseData = await response.json();
      if (!response.ok) throw new Error(responseData.error || 'Ошибка при поиске пути');
      const pathAsObjects = responseData.map(point => ({ row: point[0], col: point[1] }));
      setPath(pathAsObjects);
    } catch (err) {
      setError(err.message);
    } finally {
      setIsLoading(false);
    }
  };

  const handleSaveMazeToFile = () => {
    if (!mazeData) {
      setError('Нет лабиринта для сохранения.');
      return;
    }

    let filename = saveFilename.trim(); // Убираем пробелы по краям
    if (!filename) {
      filename = 'maze.txt'; // Имя по умолчанию, если поле пустое
    }
    if (!filename.toLowerCase().endsWith('.txt')) {
      filename += '.txt'; // Добавляем расширение, если его нет
    }

    const { rows, cols, RightWalls, DownWalls } = mazeData;

    let fileContent = `${rows} ${cols}\n`;

    const rightWallsString = RightWalls.map(row => row.join(' ')).join('\n');
    fileContent += rightWallsString;

    fileContent += '\n\n';

    const downWallsString = DownWalls.map(row => row.join(' ')).join('\n');
    fileContent += downWallsString;

    const blob = new Blob([fileContent], { type: 'text/plain' });

    const url = URL.createObjectURL(blob);

    const link = document.createElement('a');
    link.href = url;
    link.download = filename;

    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);

    URL.revokeObjectURL(url);
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Генератор Лабиринтов</h1>
      </header>
      <main>
        <div className="actions-container">

          <div className="action-section">
            <h2>Сгенерировать новый лабиринт</h2>
            <div className="generate-form">
              <label>Строки:<input type="number" value={rows} onChange={e => setRows(e.target.value)} min="1" max="50" disabled={isLoading} /></label>
              <label>Столбцы:<input type="number" value={cols} onChange={e => setCols(e.target.value)} min="1" max="50" disabled={isLoading} /></label>
              <button onClick={handleGenerate} disabled={isLoading}>{isLoading ? 'Генерация...' : 'Сгенерировать'}</button>
            </div>
          </div>

          <div className="action-section">
            <h2>Использовать файл (.txt)</h2>
            <div className="upload-form">
              <input type="file" onChange={handleFileChange} accept=".txt" disabled={isLoading} />
              <div className="button-group">
                <button onClick={handleSubmit} disabled={isLoading || !selectedFile}>Построить из файла</button>
              </div>
            </div>
          </div>

          <div className="action-section">
            <h2>Использовать файл (.txt)</h2>
            <div className="upload-form">
              <input
                type="text"
                value={saveFilename}
                onChange={(e) => setSaveFilename(e.target.value)}
                placeholder="Введите имя файла..."
                className="filename-input"
              />
              <button onClick={handleSaveMazeToFile} className="save-button">
                Сохранить в файл
              </button>
            </div>
          </div>
        </div>

        <div className="result-container">
          {isLoading && <p className="loading-message">Обработка...</p>}
          {error && <p className="error-message">Ошибка: {error}</p>}

          {(mazeData) && (
            <div className="maze-display">
              {mazeData && <h2>Ваш Лабиринт:</h2>}

              {mazeData && (
                <MazeCanvas
                  mazeData={mazeData}
                  startPoint={startPoint}
                  endPoint={endPoint}
                  path={path}
                  onCanvasClick={handleCanvasClick}
                />
              )}

              {mazeData && (
                <div className="solve-controls">
                  <p>Кликните на лабиринт, чтобы выбрать старт (зеленый) и финиш (синий).</p>
                  <div className="button-group">
                    <button onClick={handleSolveMaze} disabled={!startPoint || !endPoint || isLoading}>
                      Найти путь
                    </button>
                  </div>
                </div>
              )}
            </div>
          )}
        </div>
      </main>
    </div>
  );
}

export default App;
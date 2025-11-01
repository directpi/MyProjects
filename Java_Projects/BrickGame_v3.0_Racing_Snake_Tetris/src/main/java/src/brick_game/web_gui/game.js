/**
 * @file: game.js
 * @description: Web GUI client для BrickGame Racing v3.0
 * @created: 2025-10-05
 */

const API_BASE = '/api';
let gameStarted = false;
let gamePaused = false;
let pollInterval = null;

// Создаем игровое поле 10x20
const grid = document.getElementById('grid');
for (let y = 0; y < 20; y++) {
  for (let x = 0; x < 10; x++) {
    const cell = document.createElement('div');
    cell.className = 'cell';
    cell.dataset.x = x;
    cell.dataset.y = y;
    grid.appendChild(cell);
  }
}

// API функции
async function startGame() {
  try {
    const response = await fetch(`${API_BASE}/games/1`, { method: 'POST' });
    if (response.ok) {
      gameStarted = true;
      gamePaused = false;
      updateStatus('Running');
      startPolling();
      console.log('Game started');
    } else {
      console.error('Failed to start game:', response.status);
    }
  } catch (err) {
    console.error('Error starting game:', err);
  }
}

async function sendAction(actionId, hold = false) {
  if (!gameStarted) return;
  try {
    await fetch(`${API_BASE}/actions`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ action_id: actionId, hold })
    });
  } catch (err) {
    console.error('Error sending action:', err);
  }
}

async function getState() {
  if (!gameStarted) return null;
  try {
    const response = await fetch(`${API_BASE}/state`);
    if (response.ok) {
      return await response.json();
    }
  } catch (err) {
    console.error('Error getting state:', err);
  }
  return null;
}

// Обновление UI
function updateGameField(state) {
  if (!state || !state.field) return;
  
  const cells = document.querySelectorAll('.cell');
  cells.forEach(cell => {
    const x = parseInt(cell.dataset.x);
    const y = parseInt(cell.dataset.y);
    const isActive = state.field[y] && state.field[y][x];
    cell.classList.toggle('active', isActive);
  });
  
  // Обновляем информационную панель
  document.getElementById('score').textContent = state.score || 0;
  document.getElementById('hiScore').textContent = state.high_score || 0;
  document.getElementById('level').textContent = state.level || 1;
  document.getElementById('speed').textContent = state.speed || 1;
  
  if (state.pause) {
    updateStatus('Paused');
  } else if (state.game_running === false) {
    updateStatus('Game Over');
    stopPolling();
  } else {
    updateStatus('Running');
  }
}

function updateStatus(status) {
  document.getElementById('status').textContent = status;
}

// Поллинг состояния игры
function startPolling() {
  if (pollInterval) return;
  pollInterval = setInterval(async () => {
    const state = await getState();
    if (state) {
      updateGameField(state);
    }
  }, 100); // Опрашиваем каждые 100ms
}

function stopPolling() {
  if (pollInterval) {
    clearInterval(pollInterval);
    pollInterval = null;
  }
}

// Actions: Start=0, Pause=1, Terminate=2, Left=3, Right=4, Up=5, Down=6, Action=7
const Actions = {
  START: 0,
  PAUSE: 1,
  TERMINATE: 2,
  LEFT: 3,
  RIGHT: 4,
  UP: 5,
  DOWN: 6,
  ACTION: 7
};

// Обработчики кнопок
document.getElementById('btnStart').addEventListener('click', () => {
  if (!gameStarted) {
    startGame();
  } else {
    sendAction(Actions.START);
  }
});

document.getElementById('btnPause').addEventListener('click', () => {
  sendAction(Actions.PAUSE);
  gamePaused = !gamePaused;
});

document.getElementById('btnLeft').addEventListener('click', () => {
  sendAction(Actions.LEFT);
});

document.getElementById('btnRight').addEventListener('click', () => {
  sendAction(Actions.RIGHT);
});

document.getElementById('btnAccel').addEventListener('click', () => {
  sendAction(Actions.UP, true);
});

// Обработка клавиатуры
let keysPressed = new Set();

document.addEventListener('keydown', (e) => {
  if (keysPressed.has(e.code)) return; // Игнорируем повторы
  keysPressed.add(e.code);
  
  switch(e.code) {
    case 'Space':
      e.preventDefault();
      if (!gameStarted) {
        startGame();
      } else {
        sendAction(Actions.START);
      }
      break;
    case 'KeyP':
      e.preventDefault();
      sendAction(Actions.PAUSE);
      gamePaused = !gamePaused;
      break;
    case 'ArrowLeft':
      e.preventDefault();
      sendAction(Actions.LEFT);
      break;
    case 'ArrowRight':
      e.preventDefault();
      sendAction(Actions.RIGHT);
      break;
    case 'ArrowUp':
      e.preventDefault();
      sendAction(Actions.UP, true);
      break;
    case 'Escape':
      e.preventDefault();
      sendAction(Actions.TERMINATE);
      stopPolling();
      gameStarted = false;
      updateStatus('Stopped');
      break;
  }
});

document.addEventListener('keyup', (e) => {
  keysPressed.delete(e.code);
  
  if (e.code === 'ArrowUp') {
    sendAction(Actions.UP, false);
  }
});

console.log('BrickGame Racing Web GUI loaded. Press Space to start!');
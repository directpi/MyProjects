import React, { useRef, useEffect } from 'react';

/**
 * React-компонент для отрисовки лабиринта на HTML5 Canvas.
 * @param {object} props - Свойства компонента.
 * @param {object} props.mazeData - Данные лабиринта {rows, cols, RightWalls, DownWalls}.
 * @param {object|null} props.startPoint - Координаты начальной точки {row, col}.
 * @param {object|null} props.endPoint - Координаты конечной точки {row, col}.
 * @param {Array|null} props.path - Массив точек пути [{row, col}, ...].
 * @param {function} props.onCanvasClick - Callback-функция, вызываемая при клике на холст.
 */
function MazeCanvas({ mazeData, startPoint, endPoint, path, onCanvasClick }) {
    const canvasRef = useRef(null);

    useEffect(() => {
        if (!mazeData || !canvasRef.current) return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

        const { rows, cols, RightWalls, DownWalls } = mazeData;

        const cellWidth = canvas.width / cols;
        const cellHeight = canvas.height / rows;

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        ctx.strokeStyle = 'black';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(0, 0); ctx.lineTo(canvas.width, 0);
        ctx.moveTo(0, 0); ctx.lineTo(0, canvas.height);
        for (let i = 0; i < rows; i++) {
            for (let j = 0; j < cols; j++) {
                if (RightWalls[i][j] === 1) {
                    ctx.moveTo((j + 1) * cellWidth, i * cellHeight);
                    ctx.lineTo((j + 1) * cellWidth, (i + 1) * cellHeight);
                }
                if (DownWalls[i][j] === 1) {
                    ctx.moveTo(j * cellWidth, (i + 1) * cellHeight);
                    ctx.lineTo((j + 1) * cellWidth, (i + 1) * cellHeight);
                }
            }
        }
        ctx.stroke();

        if (path && path.length > 1) {
            ctx.strokeStyle = 'rgba(255, 0, 0, 0.7)';
            ctx.lineWidth = Math.min(cellWidth, cellHeight) / 4;
            ctx.lineCap = 'round';
            ctx.lineJoin = 'round';
            ctx.beginPath();
            ctx.moveTo((path[0].col + 0.5) * cellWidth, (path[0].row + 0.5) * cellHeight);
            for (let i = 1; i < path.length; i++) {
                ctx.lineTo((path[i].col + 0.5) * cellWidth, (path[i].row + 0.5) * cellHeight);
            }
            ctx.stroke();
        }

        const drawPoint = (point, color) => {
            if (!point) return;
            const centerX = (point.col + 0.5) * cellWidth;
            const centerY = (point.row + 0.5) * cellHeight;
            const radius = Math.min(cellWidth, cellHeight) / 3.5;

            ctx.fillStyle = color;
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
            ctx.fill();
        };

        drawPoint(startPoint, 'green');
        drawPoint(endPoint, 'blue');

    }, [mazeData, startPoint, endPoint, path]);

    const handleCanvasClick = (event) => {
        if (!mazeData || !onCanvasClick) return;

        const canvas = canvasRef.current;
        const rect = canvas.getBoundingClientRect();

        const x = event.clientX - rect.left;
        const y = event.clientY - rect.top;

        const cellWidth = canvas.width / mazeData.cols;
        const cellHeight = canvas.height / mazeData.rows;

        const col = Math.floor(x / cellWidth);
        const row = Math.floor(y / cellHeight);

        onCanvasClick({ row, col });
    };

    return (
        <canvas
            ref={canvasRef}
            width={500}
            height={500}
            className="maze-canvas"
            onClick={handleCanvasClick}
            style={{ cursor: mazeData ? 'pointer' : 'default' }}
        />
    );
}

export default MazeCanvas;

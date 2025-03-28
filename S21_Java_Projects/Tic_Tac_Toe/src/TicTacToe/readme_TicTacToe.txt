RUN Java application

To start game in terminal enter:
curl -k -X POST https://localhost:8443/game/start -u myserver:password

To make move enter:
curl -k https://localhost:8443/game/move/bf731861-c980-44ef-93c4-bf304f2cc47d -H "Content-Type: application/json" -d '{"id": "bf731861-c980-44ef-93c4-bf304f2cc47d", "board": [[1, 0, 0], [0, 0, 0], [0, 0, 0]]}'

UUID example - a94d0c3b-016b-45bc-a475-e3d543ef2666
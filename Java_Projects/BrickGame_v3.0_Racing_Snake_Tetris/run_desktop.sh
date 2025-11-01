#!/bin/bash
cd src/main/java/src/brick_game/BrickGame_v2.0-1/gui/desktop || exit 1
./s21_brickgame_desktop &
disown
echo "Desktop app started (PID: $!). Use jobs/ps to manage."

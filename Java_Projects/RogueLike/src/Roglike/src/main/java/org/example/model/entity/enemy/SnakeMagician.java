package org.example.model.entity.enemy;

import org.example.controller.ConsoleMessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Character;
import org.example.model.entity.Enemy;
import org.example.model.entity.Health;
import org.example.model.entity.UserHero;
import org.example.model.entity.enemy.strategy.DiagonalChaseStrategy;
import org.example.model.entity.enemy.strategy.DiagonalMoveStrategy;
import org.example.model.entity.enemy.strategy.EnemyMovementStrategy;
import org.example.model.util.Coordinate;

public class SnakeMagician extends Enemy {
    private final EnemyMovementStrategy diagonalMoveStrategy;
    private final EnemyMovementStrategy diagonalChaseStrategy;

    public SnakeMagician(int levelCoefficient) {
        super(new Health(24 + levelCoefficient), 11 + levelCoefficient, 1, 1 + levelCoefficient, 9 + levelCoefficient, 's', null, new ConsoleMessageHandler());
        this.diagonalMoveStrategy = new DiagonalMoveStrategy();
        this.diagonalChaseStrategy = new DiagonalChaseStrategy();
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        if (Math.random() < 0.3) {
            handler.sendMessage("The hero is put to sleep for one turn!");
            UserHero.getHero().setAsleep(true);
        }
        return false;
    }

    @Override
    public void move(Coordinate playerCoord, Room room) {
        setState(!shouldChasePlayer(playerCoord)); // Если игрок в зоне преследования, переключаемся на преследование
        if (isAdjacent(getCoordinate(), playerCoord)) {
            attack(UserHero.getHero()); // Если игрок рядом — атакуем
            return;
        }
        if (getState()) {
            diagonalMoveStrategy.move(this, playerCoord, room);
        } else {
            handler.sendMessage("The snake mage is chasing the player.");
            diagonalChaseStrategy.move(this, playerCoord, room);
        }
    }
}


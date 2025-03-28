package org.example.model.entity.enemy;

import org.example.controller.ConsoleMessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Character;
import org.example.model.entity.Enemy;
import org.example.model.entity.Health;
import org.example.model.entity.UserHero;
import org.example.model.entity.enemy.strategy.EnemyMovementStrategy;
import org.example.model.entity.enemy.strategy.MoveToPlayerStrategy;
import org.example.model.entity.enemy.strategy.ZombieChaseStrategy;
import org.example.model.util.Coordinate;

public class Zombie extends Enemy {
    private final EnemyMovementStrategy zombieChaseStrategy = new ZombieChaseStrategy();
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();

    public Zombie(int levelCoefficient) {
        super(new Health(29 + levelCoefficient), 4 + levelCoefficient, 1, 4 + levelCoefficient, 4 + levelCoefficient, 'z', null, new ConsoleMessageHandler());
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        return false;
    }

    @Override
    public void move(Coordinate playerCoord, Room room) {
        setState(!shouldChasePlayer(playerCoord)); // Если игрок в зоне преследования, переключаемся на преследование
        if (isAdjacent(getCoordinate(), playerCoord)) {
            attack(UserHero.getHero()); // Если игрок рядом — атакуем
            return;
        }
        EnemyMovementStrategy strategy = this.getState() ? zombieChaseStrategy : moveToPlayerStrategy;
        strategy.move(this, playerCoord, room);
        if (!this.getState()) {
            handler.sendMessage("Zombies are chasing the player.");
        }
    }
}

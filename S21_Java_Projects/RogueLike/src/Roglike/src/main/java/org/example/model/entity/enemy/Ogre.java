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

public class Ogre extends Enemy {
    //    private boolean resting = false;
    private final EnemyMovementStrategy moveStrategy = new ZombieChaseStrategy();
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();

    public Ogre(int levelCoefficient) {
        super(new Health(44 + levelCoefficient), 3 + levelCoefficient, 2, 9 + levelCoefficient, 5 + levelCoefficient, 'O', null, new ConsoleMessageHandler());
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        if (resting) {
            resting = false;
            handler.sendMessage("The ogre is resting after an attack.");
            return true;
        } else {
            resting = true;
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
            for (int i = 0; i < this.speed; i++) {
                if (!moveStrategy.move(this, playerCoord, room)) {
                    // Если не удалось сделать шаг, прерываем цикл
                    break;
                }
            }
        } else {
            handler.sendMessage("The ogre is chasing the player.");
            moveToPlayerStrategy.move(this, playerCoord, room);
        }
    }
}

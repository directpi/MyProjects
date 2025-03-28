package org.example.model.entity.enemy;

import org.example.controller.ConsoleMessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Character;
import org.example.model.entity.Enemy;
import org.example.model.entity.Health;
import org.example.model.entity.UserHero;
import org.example.model.entity.enemy.strategy.EnemyMovementStrategy;
import org.example.model.entity.enemy.strategy.MoveToPlayerStrategy;
import org.example.model.entity.enemy.strategy.StayOnPlaceStrategy;
import org.example.model.util.Coordinate;

public class Vampire extends Enemy {
    private final EnemyMovementStrategy stayOnPlaceStrategy = new StayOnPlaceStrategy();
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();

    public Vampire(int levelCoefficient) {
        super(new Health(31 + levelCoefficient), 8 + levelCoefficient, 1, 3 + levelCoefficient, 7 + levelCoefficient, 'v', null, new ConsoleMessageHandler());
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        if (target instanceof UserHero hero) {
            int vampDamage = (int) (hero.getHealth().getMaxHealth() * 0.1); // 10 % от максимального здоровья
            hero.getHealth().takingDamage(vampDamage);
            handler.sendMessage("A vampire takes away 10% of the player's maximum health! Damage: " + vampDamage);
            return true;
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
        EnemyMovementStrategy strategy = this.getState() ? stayOnPlaceStrategy : moveToPlayerStrategy;
        strategy.move(this, playerCoord, room);
        if (!this.getState()) {
            handler.sendMessage("The vampire is chasing the player.");
        }
    }
}
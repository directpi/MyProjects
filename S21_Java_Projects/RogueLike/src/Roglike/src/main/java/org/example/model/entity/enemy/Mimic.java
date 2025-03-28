package org.example.model.entity.enemy;

import org.example.controller.ConsoleMessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Character;
import org.example.model.entity.Health;
import org.example.model.entity.Enemy;
import org.example.model.entity.UserHero;
import org.example.model.entity.enemy.strategy.EnemyMovementStrategy;
import org.example.model.entity.enemy.strategy.MoveToPlayerStrategy;
import org.example.model.entity.enemy.strategy.StayOnPlaceStrategy;
import org.example.model.util.Coordinate;

public class Mimic extends Enemy {
    private static final char ITEM_SKIN = 'I';
    private static final char MIMIK_SKIN = 'm';

    private char curent_skin; // Текущий "облик" мимика

    private final EnemyMovementStrategy stayOnPlaceStrategy = new StayOnPlaceStrategy();
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();

    public Mimic(int levelCoefficient) {
        super(new Health(29 + levelCoefficient), 7 + levelCoefficient, 1, 3 + levelCoefficient, 1 + levelCoefficient, ITEM_SKIN, null, new ConsoleMessageHandler());
        this.curent_skin = ITEM_SKIN; // Начальный облик - предмет
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        if (target instanceof UserHero hero) {
            int stolenGold = Math.min(5, hero.getTreasure()); // Крадет 5 золота
            hero.addTreasure(-stolenGold);
            handler.sendMessage("The mimic stole" + stolenGold + " gold!");
            return true;
        }
        return false;
    }

    @Override
    public void move(Coordinate playerCoord, Room room) {

        setState(!shouldChasePlayer(playerCoord)); // Если игрок в зоне преследования, переключаемся на преследование
        if (isAdjacent(getCoordinate(), playerCoord)) {
            transformToMimic();
            attack(UserHero.getHero()); // Если игрок рядом — атакуем
            return;
        }
        EnemyMovementStrategy strategy = this.getState() ? stayOnPlaceStrategy : moveToPlayerStrategy;
        strategy.move(this, playerCoord, room);
        if (!this.getState()) {
            transformToMimic();
            handler.sendMessage("The mimic reveals itself and pursues the player.");
        } else {
            transformToItem();
        }

    }

    public void transformToMimic() {
        skin = MIMIK_SKIN;
    }

    public void transformToItem() {
        skin = ITEM_SKIN;
    }
}

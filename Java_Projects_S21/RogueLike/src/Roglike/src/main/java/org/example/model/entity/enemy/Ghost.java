package org.example.model.entity.enemy;

import org.example.controller.ConsoleMessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Character;
import org.example.model.entity.Enemy;
import org.example.model.entity.Health;
import org.example.model.entity.UserHero;
import org.example.model.entity.enemy.strategy.GhostTeleportStrategy;
import org.example.model.util.Coordinate;

import java.util.Random;

public class Ghost extends Enemy {
    private static final Random rand = new Random();
    private boolean isHidden;

    public Ghost(int levelCoefficient) {
        super(new Health(19 + levelCoefficient), 9 + levelCoefficient, 0, 1 + levelCoefficient, 2 + levelCoefficient, 'g', new GhostTeleportStrategy(new ConsoleMessageHandler()), new ConsoleMessageHandler());
        this.isHidden = false;
        this.state = true;
    }

    @Override
    public void move(Coordinate playerCoord, Room room) {
        setState(!shouldChasePlayer(playerCoord)); // Если игрок в зоне преследования, переключаемся на преследование
        if (isAdjacent(getCoordinate(), playerCoord)) {
            attack(UserHero.getHero()); // Если игрок рядом — атакуем
            return;
        }
        movementStrategy.move(this, playerCoord, room);
        isHidden = rand.nextDouble() < 0.3;
        if (!this.getState()) {
            handler.sendMessage("A ghost haunts the player.");
        }
    }

    @Override
    protected boolean uniqueAttack(Character target) {
        if (isHidden) {
            handler.sendMessage(getNameBySkin(getSkin()) + " hidden and unable to attack.");
            return true;
        }
        return false;
    }
}
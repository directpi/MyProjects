package org.example.model.entity;


import lombok.Getter;
import lombok.Setter;
import org.example.datalayer.GameStatistic;
import org.example.model.entity.enemy.strategy.*;
import org.example.model.entity.enemy.strategy.EnemyMovementStrategy;
import org.example.controller.MessageHandler;
import org.example.model.util.Coordinate;
import org.example.model.dungeon.Room;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public abstract class Enemy extends Character {

    private static final Random rand = new Random();
    private static final Map<java.lang.Character, String> skinToName = new HashMap<>();
    protected boolean resting = false;

    static {
        skinToName.put('z', "Zombie");
        skinToName.put('v', "Vampire");
        skinToName.put('g', "Ghost");
        skinToName.put('O', "Ogre");
        skinToName.put('s', "Snake mage");
        skinToName.put('m', "Mimic");
    }

    @Getter
    protected int hostility; // враждебность
    @Setter
    protected boolean state; // состояние "покоя(true)/атаки(false)"
    protected EnemyMovementStrategy movementStrategy; // Стратегия передвижения
    @Setter
    protected MessageHandler handler;


    public Enemy(Health health, int dexterity, int speed, int strength, int hostility, char skin, EnemyMovementStrategy movementStrategy, MessageHandler handler) {
        super(health, dexterity, speed, strength, skin);
        this.hostility = hostility;
        this.state = true;
        this.movementStrategy = movementStrategy;
        this.handler = handler;
    }

    public boolean getState() {
        return state;
    }

    public String getNameBySkin(char skin) {
        return skinToName.getOrDefault(skin, "Неизвестный враг");
    }

    public void attack(Character target) {
        if (shouldChasePlayer(target.getCoordinate())) {
            setState(false);
            if (isAdjacent(this.getCoordinate(), target.getCoordinate()) && canHit(this.dexterity, target.getDexterity())) {
                boolean exitFlag = uniqueAttack(target); // Вызов специфической атаки
                if (exitFlag) {
                    return;
                }
                int extraValue = rand.nextInt(3);
                int damage = calculateDamage(this.strength, extraValue);
                target.getHealth().takingDamage(damage);
                handler.sendMessage(getNameBySkin(getSkin()) + " attacking. Damage: " + damage);
                GameStatistic.addMissingStrike(); // Считаем, что герой пропустил удар
            } else {
                handler.sendMessage(getNameBySkin(getSkin()) + " unsuccessfully attacking. The player dodged the blow.");
            }
        } else {
            handler.sendMessage(getNameBySkin(getSkin()) + " Too far to attack.");
        }
    }

    protected abstract boolean uniqueAttack(Character target);

    public void move(Coordinate playerCoord, Room room) {

        state = !shouldChasePlayer(playerCoord); // Проверка, что герой в зоне атаки
        if (movementStrategy != null) {
            movementStrategy.move(this, playerCoord, room);
        }
    }

    public boolean shouldChasePlayer(Coordinate playerCoord) {
        return this.getCoordinate().distanceTo(playerCoord) <= this.hostility;
    }

//    public boolean isAlive() {
//        return !this.getHealth().isDead();
//    }

    public boolean isWalkable(Coordinate coord) {
        if (!(this.getLocation() instanceof Room currentRoom)) {
            return false;
        }
        return !currentRoom.isWall(coord);
    }

    public void dropLoot() {
        UserHero hero = UserHero.getHero();
        int lootAmount = this.getHostility() + this.getStrength() + this.getDexterity() + this.getHealth().getMaxHealth();
        handler.sendMessage("You have received " + lootAmount + " gold!");
        hero.addTreasure(lootAmount);
        GameStatistic.addJewel(lootAmount);
    }
}

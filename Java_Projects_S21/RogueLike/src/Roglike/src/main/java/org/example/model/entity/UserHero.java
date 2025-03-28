package org.example.model.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Getter;
import lombok.Setter;
import org.example.controller.ConsoleMessageHandler;
import org.example.datalayer.GameStatistic;
import org.example.model.ActiveElixir;
import org.example.model.dungeon.Room;
import org.example.model.entity.item.Elixir;
import org.example.model.entity.item.Weapon;
import org.example.model.util.Coordinate;

import java.util.ArrayList;

public class UserHero extends Character {
    private static UserHero hero;
    @Setter
    @Getter
    private String heroName;
    @Setter
    @Getter
    private Backpack backpack;
    @JsonIgnore
    private Coordinate coordinate;
    @Setter
    @Getter
    private int treasure;
    @Setter
    @Getter
    private ArrayList<ActiveElixir> activeElixir;
    @Setter
    @Getter
    private Weapon equippedWeapon;
    @Setter
    @Getter

    protected boolean asleep = false;
    private boolean firstAttackAgainstVampire = true;

    {
        heroName = "";
        coordinate = new Coordinate();
        backpack = new Backpack();
        activeElixir = new ArrayList<>();
    }

    private UserHero() {
        super(new Health(100), 10, 1, 15, 'h');
        this.coordinate = new Coordinate(0, 0);
    }

    private UserHero(Coordinate coordinate) {
        super(new Health(100), 10, 1, 15, 'h');
        this.coordinate = coordinate;
    }

    public static UserHero getHero() {
        if (hero == null) {
            hero = new UserHero();
        }
        return hero;
    }

    @Override
    public void attack(Character target) {
        ConsoleMessageHandler handler = new ConsoleMessageHandler();
        if (isAdjacent(this.getCoordinate(), target.getCoordinate())) {
            if (target.getSkin() == 'v') {
                // Проверяем, был ли уже первый удар по вампиру
                if (firstAttackAgainstVampire) {
                    handler.sendMessage("The first blow to a vampire is always a miss!");
                    firstAttackAgainstVampire = false;
                    return;
                }
            }
            if (canHit(this.dexterity, target.getDexterity()) && !getHero().isAsleep()) {

                int weaponModifier = 0;
                if (backpack.getItems().containsKey("Weapon")) {
                    var weapons = backpack.getItems().get("Weapon");
                    if (!weapons.isEmpty()) {
                        Item item = weapons.getFirst();
                        if (item instanceof Weapon weapon) {
                            weaponModifier = weapon.getModifier();
                        }
                    }
                }

                int damage = calculateDamage(this.strength, weaponModifier);//Добавил модификатор оружия
                target.getHealth().takingDamage(damage);
                handler.sendMessage("The hero attacks. Damage: " + damage);
                GameStatistic.addMyHit();
                if (target.getHealth().isDead()) {
                    Enemy enemy = (Enemy) target;
                    handler.sendMessage(enemy.getNameBySkin(enemy.getSkin()) + " is dead!");
                    GameStatistic.addKillingEnemy();
                    enemy.dropLoot();
                    if (this.location instanceof Room currentRoom) {
                        currentRoom.getEnemies().remove(enemy); // Удаляем врага из списка
                    }
                }
            } else {
                if (getHero().isAsleep()) {
                    handler.sendMessage("The hero is euthanized by the snake magician for one turn.");
                    setAsleep(false);
                } else {
                    handler.sendMessage("The hero's attack is unsuccessful. The enemy dodged the blow.");
                }
            }
        } else {
            handler.sendMessage("The hero is too far away to attack.");
        }
    }

    protected boolean uniqueAttack(Character target) {
        return false;
    }

    public void setCoordinate(int x, int y) {
        if (this.coordinate == null) {
            this.coordinate = new Coordinate(x, y);
        } else {
            this.coordinate.x = x;
            this.coordinate.y = y;
        }
    }

    public void addTreasure(int treasure) {
        this.treasure += treasure;
    }

    public boolean pickUpItem(Item item) {
        return backpack.addItem(item);
    }

    public void addStrength(int strength) {
        this.strength += strength;
    }

    public void addDexterity(int dexterity) {
        this.dexterity += dexterity;
    }

    public void addMaxHp(int hp) {
        health.addMaxHealth(hp);
    }

    public void decreaseStrength(int strength) {
        this.strength -= strength;
    }

    public void decreaseDexterity(int dexterity) {
        this.dexterity -= dexterity;
    }

    public void decreaseHealth(int health) {
        this.health.decreaseMaxHealth(health);
    }

    public void addActiveElixir(Elixir.TypeElixir typeElixir, int duration, int characteristic) {
        activeElixir.add(new ActiveElixir(typeElixir, characteristic, duration));
    }

    public void EquipWeapon(Weapon weapon) {
        equippedWeapon = weapon;
    }

    public boolean dropItem(Item item) {
        var hero = UserHero.getHero();
        var location = hero.getLocation();
        if (location instanceof Room) {
            item.setCoordinate(hero.getCoordinate());
            ((Room) location).addEntity(item);
            return true;
        }
        return false;
    }

    @Override
    public String toString() {
        return String.format(
                "    Hits:   %s    Str:  %d    Gold:    %d    Dexterity:   %d",
                getHealth().toString(), // Текущее и максимальное здоровье
                getStrength(), // Сила
                getTreasure(), // Золото
                getDexterity() // Ловкость
        );
    }


      public void setHealth(Health _health) {
        health = _health;
    }

    public void setHero(UserHero hero) {
        UserHero.hero = hero;
    }
    public static void resetHero(){
        UserHero.hero = null;
    }

}

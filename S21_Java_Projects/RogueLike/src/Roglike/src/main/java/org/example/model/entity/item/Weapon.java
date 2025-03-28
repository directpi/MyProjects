package org.example.model.entity.item;

import lombok.Getter;
import lombok.NoArgsConstructor;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;

import java.util.Random;

@NoArgsConstructor(force = true)
@Getter
public class Weapon extends Item {
    public enum WeaponType {SWORD, AXE, DAGGER, SABER}
    private final WeaponType weaponType;
    private final int modifier;

    public Weapon(Coordinate coordinate) {
        super(coordinate);
        weaponType = WeaponType.values()[new Random().nextInt(4)];
        modifier = rand.nextInt(6) + 1;
    }

    @Override
    public boolean useItem() {
        UserHero.getHero().setEquippedWeapon(this);
        return true;
    }

    @Override
    public String toString() {
        return super.toString() + "attack +" + modifier;
    }

}

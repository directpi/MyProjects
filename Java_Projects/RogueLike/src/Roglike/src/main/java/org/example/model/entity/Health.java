package org.example.model.entity;

import jcurses.system.CharColor;
import jcurses.system.Toolkit;
import lombok.Getter;
import lombok.Setter;

@Getter
public class Health implements Cloneable {
    @Setter
    private int maxHealth;
    @Setter
    private int currentHealth;
    private boolean dead;

    public Health(int max_health) {
        this.maxHealth = max_health;
        this.currentHealth = max_health;
    }

    public Health() {
        maxHealth = 0;
        currentHealth = 0;
    }

    public void takingDamage(int health) {
        if (this.currentHealth <= health) {
            this.currentHealth = 0;
            this.dead = true;
        } else {
            this.currentHealth -= health;
        }
    }

    public void addCurrentHealth(int health) {
        this.currentHealth = Math.min(health + this.currentHealth, maxHealth);
    }

    public void addMaxHealth(int health) {
        if (this.maxHealth == this.currentHealth) {
            this.currentHealth += health;
        }
        this.maxHealth += health;
    }

    public void decreaseMaxHealth(int health) {
        this.maxHealth -= health;
        if (this.maxHealth < this.currentHealth) {
            this.currentHealth = this.maxHealth;
        }
    }

    @Override
    public Health clone() {
        try {
            return (Health) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public String toString() {
        return getCurrentHealth() + "(" + getMaxHealth() + ")";
    }
}

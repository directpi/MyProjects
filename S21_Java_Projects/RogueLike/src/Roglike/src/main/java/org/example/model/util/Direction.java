package org.example.model.util;

import java.util.Random;

public enum Direction {TOP, RIGHT, BOTTOM, LEFT;
    private static final Random random = new Random();
    public static Direction randomDirection() {
        Direction[] directions = values();
        return directions[random.nextInt(directions.length)];
    }
}

package org.example.model.entity.enemy;

import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;

import java.util.Random;

public class StaticEnemyFactory {
    public enum EnemyType {
        Zombie, Vampire, Ghost, Ogre, Snake_magician, Mimic;

        private static final Random random = new Random();

        public static EnemyType randomEnemy() {
            EnemyType[] types = values();
            return types[random.nextInt(types.length)];
        }
    }

    public static Enemy createRandomEnemy(int levelCoefficient) {
        return createEnemy(EnemyType.randomEnemy(), levelCoefficient);
    }

    public static Enemy createRandomEnemy(Coordinate coordinate, int levelCoefficient) {
        var tmp = createEnemy(EnemyType.randomEnemy(), levelCoefficient);
        tmp.setCoordinate(coordinate);
        return tmp;
    }

    public static Enemy createEnemy(EnemyType type, int levelCoefficient) {
        return switch (type) {
            case Zombie -> new Zombie(levelCoefficient);
            case Vampire -> new Vampire(levelCoefficient);
            case Ghost -> new Ghost(levelCoefficient);
            case Ogre -> new Ogre(levelCoefficient);
            case Snake_magician -> new SnakeMagician(levelCoefficient);
            case Mimic -> new Mimic(levelCoefficient);
        };
    }
}

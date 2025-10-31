package org.example.view;

import jcurses.system.InputChar;
import jcurses.system.Toolkit;


public class Input {
    public static char getCh() {
        InputChar input;
        do {
            input = Toolkit.readCharacter();
            if (!input.isSpecialCode()) {
                break;
            }
        }while (true);
        return input.getCharacter();
    }
    public static InputChar getCh2() {
        InputChar input;
        do {
            input = Toolkit.readCharacter();
            if (!input.isSpecialCode()) {
                break;
            }
        }while (true);
        return input;
    }
}

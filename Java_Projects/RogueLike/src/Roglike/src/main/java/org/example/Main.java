package org.example;

import jcurses.system.Toolkit;

public class Main {

    public static void main(String[] args) {
        try {
            App app = new App();
            app.run();
        } catch (Exception e) {
            Toolkit.shutdown();
            e.printStackTrace();
        }
    }
}

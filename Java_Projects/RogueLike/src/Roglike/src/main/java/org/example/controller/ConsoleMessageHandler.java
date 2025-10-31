package org.example.controller;

import org.example.model.Screen;

public class ConsoleMessageHandler implements MessageHandler {
    @Override
    public void sendMessage(String message) {
        Screen.addMessage(message);
    }
}

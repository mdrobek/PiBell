package ait;

import ait.gui.AnswerItWindow;
import ait.gui.UIListener;
import ait.rest.Rest;

import javax.swing.*;
import java.io.IOException;

public class AnswerItController implements UIListener {

    private static final boolean LOCAL = false;

    private static final String USER_ID = "marc";
    private static final String ELLIS   = "ellis";
//    private static final String ELLIS   = "test";
    private Rest rest;
    private AnswerItWindow window;

    public static void main(String[] args) {
        new AnswerItController();
    }

    public AnswerItController() {
        this.rest = new Rest(USER_ID, LOCAL);
        this.window = new AnswerItWindow(this, LOCAL);
        this.window.setVisible(true);
    }

    @Override
    public void callTriggered() {
        try {
            System.out.println("Now triggering call to: " + ELLIS);
            this.rest.performCallRequest(ELLIS);
        } catch (IOException e) {
            e.printStackTrace();
            JDialog foo = new JDialog(this.window);
            foo.setTitle("Problem");
            foo.add(new JLabel(e.getLocalizedMessage()));
            foo.setSize(750, 100);
            foo.setLocationRelativeTo(this.window);
            foo.setVisible(true);
        }
    }
}

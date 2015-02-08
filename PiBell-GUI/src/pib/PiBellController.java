package pib;

import pib.gui.PiBellWindow;
import pib.gui.UIListener;
import pib.rest.Rest;

import javax.swing.*;
import java.io.IOException;

public class PiBellController implements UIListener {

    private static final boolean LOCAL = false;

    private static final String USER_ID     = "mdrobek";
    private static final String RECIPIENT   = "ellis";
//    private static final String RECIPIENT   = "test";
    private Rest rest;
    private PiBellWindow window;

    public static void main(String[] args) {
        new PiBellController();
    }

    public PiBellController() {
        this.rest = new Rest(USER_ID, LOCAL);
        this.window = new PiBellWindow(this, LOCAL);
        this.window.setVisible(true);
    }

    @Override
    public void callTriggered() {
        try {
            System.out.println("Now triggering call to: " + RECIPIENT);
            this.rest.performCallRequest(RECIPIENT);
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

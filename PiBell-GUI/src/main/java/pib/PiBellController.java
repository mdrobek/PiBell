package pib;

import pib.gui.PiBellWindow;
import pib.gui.UIListener;
import pib.rest.Rest;

import javax.swing.*;
import java.io.IOException;

/**
 * The main controller class that connects the UI and Rest functionality together. Whenever the UI
 * reports a click on the "trigger a call button" the controller will forward that command to the
 * REST object, which then performs the actual call request to the backend.
 *
 * @author mdrobek
 * @version 0.1
 * @since 30/12/2014
 */
public class PiBellController implements UIListener {

    // Whether we're running in local mode (PiBell-Server runs in the intranet) or not
    private static final boolean LOCAL = false;
    // The local name used to tell the recipient who called (PiBell-GUI name)
    private static final String USER_ID     = "mdrobek";
    // The name of the recipient for outgoing calls (the PiBell-Pi name)
    private static final String RECIPIENT   = "ellis";
    // The Rest object to perform requests to the backend
    private Rest rest;
    // The PiBell UI window
    private PiBellWindow window;

    public static void main(String[] args) {
        new PiBellController();
    }

    /**
     * Creates a new PiBell controller object, which itself creates and shows the window.
     */
    public PiBellController() {
        this.rest = new Rest(USER_ID, LOCAL);
        this.window = new PiBellWindow(this, LOCAL);
        this.window.setVisible(true);
    }

    @Override
    public void callTriggered() {
        try {
            // The button has been clicked -> perform a call request to the backend
            this.rest.performCallRequest(RECIPIENT);
        } catch (IOException e) {
            e.printStackTrace();
            // If we are experiencing any errors while performing a call request to the backend
            // -> simply notify the user about this error in a popup dialog
            JDialog foo = new JDialog(this.window);
            foo.setTitle("Problem");
            foo.add(new JLabel(e.getLocalizedMessage()));
            foo.setSize(750, 100);
            foo.setLocationRelativeTo(this.window);
            foo.setVisible(true);
        }
    }
}

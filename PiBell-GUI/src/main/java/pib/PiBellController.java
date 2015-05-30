package pib;

import pib.gui.PiBellWindow;
import pib.gui.UIListener;
import pib.rest.Rest;
import pib.rest.RestListener;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.*;
import java.util.Timer;

/**
 * The main controller class that connects the UI and Rest functionality together. Whenever the UI
 * reports a click on the "trigger a call button" the controller will forward that command to the
 * REST object, which then performs the actual call request to the backend.
 *
 * @author mdrobek
 * @version 0.1
 * @since 30/12/2014
 */
public class PiBellController implements UIListener, RestListener {

    // Whether we're running in local mode (PiBell-Server runs in the intranet) or not
    private static final boolean LOCAL = true;
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
        java.util.Timer timer = new Timer();
        this.rest = new Rest(USER_ID, this, LOCAL);
        this.window = new PiBellWindow(this, LOCAL);
        this.window.setVisible(true);
        this.window.showContact(RECIPIENT);
        this.window.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent windowEvent) {
                timer.cancel();
                timer.purge();
            }
        });
        TimerTask tt = new TimerTask() {
            @Override
            public void run() {
                try {
                    PiBellController.this.rest.performStatusRequest(RECIPIENT);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        };
        timer.schedule(tt, 0, 3000);
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

    @Override
    public void statusChange(String contact, boolean onlineNow, long lastSeenOnline) {
        this.window.changeOnlineStatus(contact, onlineNow, lastSeenOnline);
    }

}

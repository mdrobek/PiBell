package pib.gui;

import org.javatuples.Pair;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * This class is the main UI entity which enables the interaction with a user. UI state changes
 * (such as a triggered call to the backend) will be propagated to a registered UI listener (which
 * is most likely the Controller in this simple use-case).
 *
 * @author MD
 * @version 0.1
 * @since 30/12/2014
 */
public class PiBellWindow extends JFrame implements ActionListener {

    // The name of the Window
    private String title        = "PiBell-GUI";
    // Whether the PiBell-GUI runs in local mode or not
    private boolean isLocal     = false;
    // The UI listener that should be notified about state changes
    private UIListener listener;
    // Maps from contact name to Pair<online status label, last seen label>
    private Map<String, Pair<JLabel, JLabel>> statusMap = new HashMap<>();

    /**
     * Creates a new PiBell window object for the given listener and the network mode.
     * @param aListener The listener object that should be notified about state changes in the UI.
     * @param isLocal True - The PiBell-GUI runs in local mode.
     *                False - Otherwise.
     */
    public PiBellWindow(UIListener aListener, boolean isLocal) {
        this.listener = aListener;
        this.isLocal = isLocal;
        setSize(300, 100);
        setLocationRelativeTo(null);
        // Adjust the window title, if we're running in local mode
        if (this.isLocal) title += " - LOCAL VERSION";
        setTitle(title);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setLayout(new GridLayout(1,3));
    }

    /**
     * Create a new entry in the window for a given contact name.
     * @param name The name of the contact for which a new row should be added to the UI.
     */
    public void showContact(String name) {
        JButton callButton = new JButton("Call somebody");
        callButton.setSize(150, 50);
        callButton.addActionListener(this);

        add(new JLabel(name));
        add(callButton);
        JLabel onlineStatus = new JLabel("offline");
        onlineStatus.setOpaque(true);
        onlineStatus.setBackground(Color.red);
        JLabel lastSeen     = new JLabel("never");
        this.statusMap.put(name, new Pair<>(onlineStatus, lastSeen));
        add(onlineStatus);
        add(lastSeen);
    }

    /**
     * Changes the visible 'status' flag of the given contact name.
     * @param name The name of the contact whose status should be changed.
     * @param isOnline Whether to change the status to online or offline.
     * @param lastSeen MS from 1970 when the contact was last seen online.
     */
    public void changeOnlineStatus(String name, boolean isOnline, long lastSeen) {
        if (this.statusMap.containsKey(name)) {
            System.out.println("Changing status of name: " + name);
            JLabel curStatus = this.statusMap.get(name).getValue0();
            if (isOnline) {
                curStatus.setBackground(Color.green);
                curStatus.setText("online");
            } else {
                curStatus.setBackground(Color.red);
                curStatus.setText("offline");
            }
            JLabel lastSeenLabel = this.statusMap.get(name).getValue1();
            String lastSeenText = lastSeen <= 0 ? "never" : new Date(lastSeen).toString();
            lastSeenLabel.setText(lastSeenText);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        // Currently, we're only having one action that could possible be propagated
        // (which is a triggered call)
        this.listener.callTriggered();
    }
}

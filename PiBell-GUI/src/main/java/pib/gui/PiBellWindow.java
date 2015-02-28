package pib.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

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
    // The button used to initiate a call request to the backend
    private JButton callButton;
    // The UI listener that should be notified about state changes
    private UIListener listener;

    /**
     * Creates a new PiBell window object for the given listener and the network mode.
     * @param aListener The listener object that should be notified about state changes in the UI.
     * @param isLocal True - The PiBell-GUI runs in local mode.
     *                False - Otherwise.
     */
    public PiBellWindow(UIListener aListener, boolean isLocal) {
        this.listener = aListener;
        this.isLocal = isLocal;
        super.setSize(300, 100);
        super.setLocationRelativeTo(null);
        // Adjust the window title, if we're running in local mode
        if (this.isLocal) title += " - LOCAL VERSION";
        super.setTitle(title);
        super.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        addLayout();
    }

    /**
     * Create and add a default GridLayout to our main window that only contains the trigger call
     * button so far.
     */
    private void addLayout() {
        this.callButton = new JButton("Call somebody");
        this.callButton.setSize(150, 50);
        this.callButton.addActionListener(this);

        GridLayout layout = new GridLayout();
        super.setLayout(layout);
        super.add(this.callButton);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        // Currently, we're only having one action that could possible be propagated
        // (which is a triggered call)
        this.listener.callTriggered();
    }
}

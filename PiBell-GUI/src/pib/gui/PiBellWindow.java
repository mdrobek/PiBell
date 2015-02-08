package pib.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Document this file!
 * Creation date: 30/12/14
 *
 * @author MD
 */
public class PiBellWindow extends JFrame implements ActionListener {
    private JButton callButton;
    private boolean isLocal = false;
    private UIListener listener;

    public PiBellWindow(UIListener aListener, boolean isLocal) {
        this.listener = aListener;
        this.isLocal = isLocal;
        super.setSize(300, 100);
        super.setLocationRelativeTo(null);
        String title = "PiBell-GUI";
        if (this.isLocal) title += " - LOCAL VERSION";
        super.setTitle(title);
        super.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        addLayout();
    }

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
//        System.out.println(e.toString());
        this.listener.callTriggered();
    }
}

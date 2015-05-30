package pib.gui;

/**
 * A listener interface that provides all notification methods available for the UI to inform the
 * controller about state changes.
 *
 * @author mdrobek
 * @version 0.1
 * @since 30/12/2014
 */
public interface UIListener {

    /**
     * This method notifies the controller that a call request has just been sent to the
     * PiBell-Server.
     */
    void callTriggered();
}

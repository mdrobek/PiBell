package pib.rest;

/**
 * A listener interface that provides all methods used by the REST classes to notify a controller
 * for new changes.
 *
 * @author mdrobek
 * @version 1.0
 * @since 30/05/15
 */
public interface RestListener {

    /**
     * Is called whenever a contacts online status has been changed.
     * @param contact The name of the contact whose status has changed.
     * @param onlineNow True - The contact is online now
     *                  False - The contact is offline now
     * @param lastSeenOnline MS since 1970 when the contact has last been seen online
     */
    void statusChange(String contact, boolean onlineNow, long lastSeenOnline);
}

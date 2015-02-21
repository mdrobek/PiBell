package pib.rest;

import com.google.gson.JsonObject;
import org.javatuples.Pair;
import org.springframework.stereotype.Component;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import java.util.HashMap;
import java.util.Map;

/**
 * This class is a simple prototypical implementation for a PiBell-Server REST API. It provides a
 * 'ping' and 'call' web-service that can be used to trigger and check for calls. However, to keep
 * it simple, all calls are stored in the app layer and are lost if the application server is
 * restarted.
 *
 * @author mdrobek
 * @version 0.1
 * @since 01/02/15
 */
@Component
@Path("/rest")
public class PiBellServices {

    // Time to buffer a call before it gets discarded
    private static long BUFFERING_TIME_MIN = 5;
    private static long BUFFERING_TIME_MS = BUFFERING_TIME_MIN * 60 * 1000;
    // Maps from recipient name (Raspberry Pi name) ->
    //      Pair<calling time in ms, name of caller (UI client)>
    // e.g.: "PiBell-Pi name" -> <1276326542, "PiBell-GUI name">
    private static Map<String, Pair<Long, String>> ringing = new HashMap<>();

    /**
     * This method checks for a given 'userID', whether a call has been registered in the last
     * BUFFERING_TIME_MIN minutes. If that is the case, it returns a <b>JSON object</b> with the
     * following properties:
     * <ul>
     *     <li>isCalling:Boolean - Whether a call for the given userID has been registered in the
     *     last BUFFERING_TIME_MIN minutes.</li>
     *     <li>caller:String - The name of the caller that triggered the call.</li>
     *     <li>callTime:long - The local time the call has been registered at the server
     *     (server-time) in ms since 1/1/1970.</li>
     *     <li>isCallOutDated:Boolean - Whether the registered call is older than
     *     BUFFERING_TIME_MIN minutes.</li>
     * </ul>
     * If for any reason the execution of this method produces an error it'll return a <b>JSON
     * error object</b> with the following properties:
     * <ul>
     *     <li>isError:Boolean - Always true to indicate an error while this method was executed.
     *     </li>
     *     <li>message:String - An error message for more information about the error.</li>
     * </ul>
     * @param userID The user ID String to check for a registered call (PiBell-GUI name).
     * @return A JSON object that contains all information for the registered call. In case of any
     * execution errors of this method, a JSON error object will be returned.
     */
    @POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
    @Path("/ping")
    public String ping(@FormParam("uid") String userID) {
        try {
            boolean isCalling = false;
            String caller = "";
            long serverCallTime = -1;
            boolean isCallOutdated = false;
            // 1) Check for last call
            Pair<Long, String> lastCall = popLastCall(userID);
            if (null != lastCall) {
                // a) someone tried to call
                caller = lastCall.getValue1();
                serverCallTime = lastCall.getValue0();
                isCallOutdated = isCallOutdated(lastCall.getValue0());
                // b) only set 'calling' if call is not outdated
                if (!isCallOutdated) isCalling = true;
            }
            // 2) Build the JSON ping response object
            JsonObject jsonRing = new JsonObject();
            jsonRing.addProperty("isCalling", isCalling);
            jsonRing.addProperty("caller", caller);
            jsonRing.addProperty("callTime", serverCallTime);
            jsonRing.addProperty("isCallOutdated", isCallOutdated);
            return jsonRing.toString();
        } catch(Exception e) {
            // Something went wrong while processing the ping request -> Notify the requesting side
            // about this by server-side issue with a JSON error object
            JsonObject error = new JsonObject();
            error.addProperty("isError", true);
            error.addProperty("message", String.format("An error occurred, while checking for a "
                    + "call of recipient %s", userID));
            return error.toString();
        }
    }

    /**
     * This method registers a new call initiated from the given 'caller' name (PiBell-GUI name) for
     * the given 'rec' recipient (PiBell-Pi name). Once the call has been registered, this method
     * creates a <b>JSON response object</b> to indicate a successful processing. This object
     * contains the following information:
     * <ul>
     *     <li>buffered:Boolean - Always true to indicate a successful registration of the call.
     *     </li>
     *     <li>bufferedTill:long - The local server time in ms since 1/1/1970 until the call will
     *     be treated as outdated.</li>
     *     <li>bufferTime:long - The buffer duration of the call in minutes.</li>
     * </ul>
     * If for any reason the execution of this method produces an error it'll return a <b>JSON
     * error object</b> with the following properties:
     * <ul>
     *     <li>isError:Boolean - Always true to indicate an error while this method was executed.
     *     </li>
     *     <li>message:String - An error message for more information about the error.</li>
     * </ul>
     * @param caller The user ID String used to register a new call (PiBell-GUI name).
     * @param recipient The name of the recipient (PiBell-Pi name).
     * @return A JSON object that contains all information for the registered call. In case of any
     * execution errors of this method, a JSON error object will be returned.
     */
    @POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
    @Path("/call")
    public String call(@FormParam("caller") String caller,
                       @FormParam("rec") String recipient) {
        try {
            // 1) Retrieve the current call time and store the the call in the global ringing map
            long now = System.currentTimeMillis();
            ringing.put(recipient, new Pair<>(now, caller));
            // 2) Create a JSON call response object to notify the caller about a successful call
            JsonObject call = new JsonObject();
            call.addProperty("buffered", true);
            call.addProperty("bufferedTill", now+BUFFERING_TIME_MS);
            call.addProperty("bufferTime", BUFFERING_TIME_MIN);
            return call.toString();
        } catch(Exception e) {
            // Something went wrong -> inform the requesting party with a JSON error object
            JsonObject error = new JsonObject();
            error.addProperty("isError", true);
            error.addProperty("message", String.format("An error occurred, while trying to perform "
                    + "the call: %s -> %s", caller, recipient));
            return error.toString();
        }
    }

    /**
     * Checks if a call is buffered for the given recipient and if so, returns this call and
     * removes it from the buffer.
     * @param forRecipient The name of the recipient to check for (PiBell-Pi name).
     * @return A Pair that contains the registered last calling time as its first entry and the
     * name of the caller as its second argument.
     * Null - if no call has been registered for the given recipients name
     */
    private Pair<Long, String> popLastCall(String forRecipient) {
        if (ringing.containsKey(forRecipient)) {
            Pair<Long, String> pair = ringing.get(forRecipient);
            ringing.remove(forRecipient);
            return pair;
        }
        return null;
    }

    /**
     * This method checks for a given call time in ms, whether it is outdated or not.
     * @param lastCallTime The server time in ms a call has been triggered
     * @return True - The call for the given calling time is outdated.
     * False - Otherwise
     */
    private boolean isCallOutdated(long lastCallTime) {
        long now = System.currentTimeMillis();
        return now - lastCallTime > BUFFERING_TIME_MS;
    }
}

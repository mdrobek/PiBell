package pib.rest;

import com.google.gson.JsonObject;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.javatuples.Pair;
import org.springframework.stereotype.Component;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import java.util.HashMap;
import java.util.Map;

/**
 * TODO: Document this file!
 *
 * @author mdrobek
 * @version 1.0
 * @since 01/02/15
 */
@Component
@Path("/rest")
public class PiBellServices {
    // Time to buffer a call (5 minutes in ms)
    private static long BUFFERING_TIME_MIN = 5;
    private static long BUFFERING_TIME_MS = BUFFERING_TIME_MIN * 60 * 1000;
    private Log logger = LogFactory.getLog(PiBellServices.class);

    // maps from recipient name (Raspberry Pi name) ->
    // Pair<calling time in ms, name of caller (UI client)>
    // e.g.: "ellis" -> <1276326542, "marc">
    private static Map<String, Pair<Long, String>> ringing =
            new HashMap<>();
//    static { ringing.put("ellis", "marc"); }

    @POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
    @Path("/ping")
    /**
     */
    public String ping(@FormParam("uid") String userID) {
        System.out.println("Checking ring message for user: " + userID);
        try {
            boolean isCalling = false;
            String caller = "";
            long serverCallTime = -1;
            boolean isCallOutdated = false;
            Pair<Long, String> lastCall = popLastCall(userID);
            // 1) Check for last call
            if (null != lastCall) {
                // a) someone tried to call
                caller = lastCall.getValue1();
                serverCallTime = lastCall.getValue0();
                isCallOutdated = isCallOutdated(lastCall.getValue0());
                // b) only set 'calling' if call is not outdated
                if (!isCallOutdated) isCalling = true;
            }
            JsonObject jsonRing = new JsonObject();
            jsonRing.addProperty("isCalling", isCalling);
            jsonRing.addProperty("caller", caller);
            jsonRing.addProperty("callTime", serverCallTime);
            jsonRing.addProperty("isCallOutdated", isCallOutdated);
            String retVal = jsonRing.toString();
            System.out.println(retVal);
            return retVal;
        } catch(Exception e) {
            JsonObject error = new JsonObject();
            error.addProperty("isError", true);
            error.addProperty("message", String.format("An error " +
                            "occurred, while checking for a call of recipient %s",
                    userID));
            return error.toString();
        }
    }

    @POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
    @Path("/call")
    /**
     */
    public String call(@FormParam("caller") String caller,
                       @FormParam("rec") String recipient) {
        System.out.println(String.format("Call from: %s -> %s",
                caller, recipient));
        try {
            long now = System.currentTimeMillis();
            ringing.put(recipient, new Pair<>(now, caller));
            JsonObject call = new JsonObject();
            call.addProperty("buffered", true);
            call.addProperty("bufferedTill", now+BUFFERING_TIME_MS);
            call.addProperty("bufferTime", BUFFERING_TIME_MIN);
            return call.toString();
        } catch(Exception e) {
            JsonObject error = new JsonObject();
            error.addProperty("isError", true);
            error.addProperty("message", String.format("An error " +
                    "occurred, while trying to perform the call: %s -> " +
                    "%s", caller, recipient));
            return error.toString();
        }
    }

    /**
     * Checks if a call is buffered for the given recipient and if so,
     * returns this call and removes it from the buffer.
     * @param forRecipient
     * @return
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
     *
     * @param lastCallTime in ms (server time when call has been
     *                     received)
     * @return
     */
    private boolean isCallOutdated(long lastCallTime) {
        long now = System.currentTimeMillis();
        return now - lastCallTime > BUFFERING_TIME_MS;
    }
}

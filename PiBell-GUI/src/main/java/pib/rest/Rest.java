package pib.rest;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import javax.net.ssl.*;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;

/**
 * This class encapsulates all REST specific methods used to talk to the PiBell-Server, e.g. to
 * register a new Call request to a PiBell-Pi device. It can be used in local mode which means
 * the PiBell-Server is expected to run in the local intranet (for testing), or in remote mode that
 * expects the PiBell-Server on a static
 *
 * @author mdrobek
 * @version 0.1
 * @since 30/12/14
 */
public class Rest {

    // Default server address (TODO: move into external config file)
    private static final String REMOTE_HOST_ADDRESS =
        "https://unionwork.org";
    // Localhost server for testing purposes
    private static final String LOCAL_HOST_ADDRESS =
        "http://192.168.0.3:8080";
    // The deployment prefix to access the REST API
    private static final String REST_PREFIX = "/pibell/v1/rest";
    // Backend REST interfaces
    private static final String CALL_URI = "/call";
    private static final String STATUS_URI = "/status";
    // Connection factory to initiate SSL calls
    private SSLConnectionSocketFactory sslsf;

    // The compiled REST API address
    private final String HOST_ADDRESS;
    // The local PiBell-GUI user ID name
    private final String USER_ID;
    // The listener to be notified about new changes
    private RestListener listener;

    /**
     * Creates a new REST object with the given 'userID' name which is either set in local or remote
     * mode.
     * @param aUserID The local PiBell-GUI name to carry out call requests.
     * @param local True - The Rest object will be initialised with local server settings that
     *              expect the PiBell-Server to be running in the intranet.
     *              False - All requests will be sent to a remotely running PiBell-Server.
     */
    public Rest(String aUserID, RestListener aListener, boolean local) {
        // 1) Check for local or remote mode and initialise the HOST address respectively
        if (local) HOST_ADDRESS = LOCAL_HOST_ADDRESS + REST_PREFIX;
        else HOST_ADDRESS = REMOTE_HOST_ADDRESS + REST_PREFIX;
        USER_ID = aUserID;
        this.listener = aListener;

        // 2) Initialise a SecurityContext to enable SSL calls
        SSLContext ctx;
        try {
            // FIXME: Initialise a TrustManager, that accepts everything
            // -> programatically import the certificate to the java keystore
            // see:
            // http://stackoverflow.com/questions/6755180/java-ssl-connect-add-server-cert-to-keystore-programatically
            ctx = SSLContext.getInstance("TLS");
            ctx.init(new KeyManager[0], new TrustManager[]{
                        new DefaultTrustManager()
                    },
                    new SecureRandom());
            SSLContext.setDefault(ctx);
            this.sslsf = new SSLConnectionSocketFactory(ctx);
        } catch (NoSuchAlgorithmException | KeyManagementException e) {
            e.printStackTrace();
        }
    }

    /**
     * Carries out a call request to the PiBell-Server with the given recipients name (PiBell-Pi
     * name).
     * @param recipient Name of recipient to be called.
     */
    public void performStatusRequest(String recipient) throws IOException {
        // 1) Create an SSL HttpClient
        CloseableHttpClient httpClient = HttpClients.custom()
                .setSSLSocketFactory(this.sslsf)
                .build();
        // 2) Create the call POST request
        HttpPost statusReq = createStatusRequest(recipient);
        // 3) Carry out the call request
        HttpResponse response = httpClient.execute(statusReq);
        HttpEntity resEntity = response.getEntity();
        final String response_str = EntityUtils.toString(resEntity);
        System.out.println("Response is: " + response_str);
        if (null != response_str) {
            JsonObject res = new JsonParser().parse(response_str).getAsJsonObject();
            this.listener.statusChange(recipient, res.get("isOnline").getAsBoolean(),
                    res.get("lastSeen").getAsLong());
        }
    }

    /**
     * Carries out a call request to the PiBell-Server with the given recipients name (PiBell-Pi
     * name).
     * @param recipient Name of recipient to be called.
     */
    public void performCallRequest(String recipient) throws IOException {
        // 1) Create an SSL HttpClient
        CloseableHttpClient httpClient = HttpClients.custom()
            .setSSLSocketFactory(this.sslsf)
            .build();
        // 2) Create the call POST request
        HttpPost callReq = createCallRequest(USER_ID, recipient);
        // 3) Carry out the call request
        HttpResponse response = httpClient.execute(callReq);
        HttpEntity resEntity = response.getEntity();
        final String response_str = EntityUtils.toString(resEntity);
//        System.out.println("Response is: " + response_str);
    }

    /**
     * Creates a new HTTPPost request with the given userID (local PiBell-GUI name) to check its
     * online status.
     * @param userID The local user name (PiBell-GUI name) that is used to indicate the origin of
     *               the registered call.
     */
    private HttpPost createStatusRequest(String userID) {
        // Create data (payload)
        List<NameValuePair> reqParams = new ArrayList<>();
        reqParams.add(new BasicNameValuePair("target", userID));
        return createPostRequest(STATUS_URI, reqParams);
    }

    /**
     * Creates a new HTTPPost request with the given userID (local PiBell-GUI name) and the given
     * recipients name (PiBell-Pi name) as the requests form parameters.
     * @param userID The local user name (PiBell-GUI name) that is used to indicate the origin of
     *               the registered call.
     * @param recipient The name of the recipient for the registered call (PiBell-Pi name).
     */
    private HttpPost createCallRequest(String userID, String recipient) {
        List<NameValuePair> reqParams = new ArrayList<>();
        reqParams.add(new BasicNameValuePair("caller", userID));
        reqParams.add(new BasicNameValuePair("rec", recipient));
        return createPostRequest(CALL_URI, reqParams);
    }

    /**
     * Creates a new HTTPPost request for the given URI suffix and its request parameters.
     * @param URI_suffix The last part of the Server URI, e.g., CALL, STATUS and so on
     * @param reqParams A list of request parameters
     * @return A created POST request.
     */
    private HttpPost createPostRequest(String URI_suffix, List<NameValuePair> reqParams) {
        try {
            // Create the form parameter objects for the POST request
            UrlEncodedFormEntity reqEntity = new UrlEncodedFormEntity(reqParams);
            // Create the post request for the given HOST address and the REST API location
            HttpPost post = new HttpPost(HOST_ADDRESS + URI_suffix);
            // Add the form parameters to the POST request
            post.setEntity(reqEntity);
            return post;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * A simple and empty TrustManager implementation. This is not secure and shouldn't be used
     * in a productive environment, since it merely accepts every certificate.
     */
    private static class DefaultTrustManager implements X509TrustManager {
        @Override
        public void checkClientTrusted(X509Certificate[] arg0, String arg1) {}
        @Override
        public void checkServerTrusted(X509Certificate[] arg0, String arg1) {}
        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return null;
        }
    }
}

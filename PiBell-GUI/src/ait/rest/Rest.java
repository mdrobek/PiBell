package ait.rest;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.conn.ssl.SSLContextBuilder;
import org.apache.http.conn.ssl.TrustSelfSignedStrategy;
import org.apache.http.conn.ssl.X509HostnameVerifier;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import javax.net.ssl.*;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;

/**
 * Document this file!
 * Creation date: 30/12/14
 *
 * @author MD
 */
public class Rest {

    // Default addresses
    private static final String REMOTE_HOST_ADDRESS =
        "https://unionwork.org";
    private static final String LOCAL_HOST_ADDRESS =
        "http://192.168.2.121:8080";
    private static final String REST_PREFIX = "/pibell/v1";

    // Backend interfaces
    private static final String CALL_URI = "/server/call";

    private final String HOST_ADDRESS;
    private final String USER_ID;

    private SSLConnectionSocketFactory sslsf;

    public Rest(String aUserID, boolean local) {
        if (local) HOST_ADDRESS = LOCAL_HOST_ADDRESS + REST_PREFIX;
        else HOST_ADDRESS = REMOTE_HOST_ADDRESS + REST_PREFIX;
        USER_ID = aUserID;

        SSLContext ctx;
        try {
            // FIXME: Initialise a TrustManager, that accepts everything
            // -> programmatically import the certificate to the java keystore
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
     * @param recipient Name of recipient to be called.
     */
    public void performCallRequest(String recipient) throws IOException {
        CloseableHttpClient httpClient = HttpClients.custom()
            .setSSLSocketFactory(this.sslsf)
            .build();
        HttpPost callReq = createCallRequest(this.USER_ID, recipient);
        HttpResponse response = httpClient.execute(callReq);

        HttpEntity resEntity = response.getEntity();
        final String response_str = EntityUtils.toString(resEntity);
//        System.out.println("Response is: " + response_str);
    }

    /**
     * The created request is a SECURED request.
     */
    private HttpPost createCallRequest(String userID, String recipient) {
        // Create data (payload)
        List<NameValuePair> reqParams = new ArrayList<>();
        reqParams.add(new BasicNameValuePair("caller", userID));
        reqParams.add(new BasicNameValuePair("rec", recipient));
        try {
            // Create authorisation
            UrlEncodedFormEntity reqEntity = new UrlEncodedFormEntity(reqParams);
            // Add stuff to post request
            HttpPost post = new HttpPost(HOST_ADDRESS + CALL_URI);
            post.setEntity(reqEntity);
            return post;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


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

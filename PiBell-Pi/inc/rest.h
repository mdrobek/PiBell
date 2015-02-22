#ifndef REST_H
#define REST_H

#include <stdbool.h>
#include <curl/curl.h>
#include "../inc/cJSON.h"

#define HTTP "http"
#define HTTPS "https"
// Backend remote domain address
#define REST_REMOTE_ADDRESS "yourdomain.com"
// We assume an SSL connection (HTTPS) when remotely connecting to the backend
#define REST_REMOTE_PORT 443
// Locahost address for local test connections
#define REST_LOCAL_ADDRESS "localhost"
// Default Tomcat port
#define REST_LOCAL_PORT 8080
// The PiBell-Server REST API interface static part of the URI
#define REST_API_URI "v1/rest"
// The REST URI endpoint for the ping service
#define PING_ENDPOINT "ping"
// Ping service parameters: uid:String
#define PING_PARAM_UID "uid"

// The Rest object definition that contains all necessary information to
// perform REST requests to the PiBell-Server backend 
typedef struct _Rest {
    // The current PiBell-Pi username used to check for incoming calls
    char *recipientName;
    // A request struct to access the 'ping' REST interface
    struct RestRequest *pingReq;
    // Whether to print debugging output or not
    bool verbose;
} Rest;

// The parsed JSON content returned from the PING interface
struct PingJSON {
    // If a call for the Pi has been triggered
    bool isCalling;
    // The name of the call initiator
    char *caller;
};

// The plain response as returned from the backend + the json root node
struct RestResponse {
    // JSON string response from the REST API
    char *jsonString;
    // The cJSON object
    cJSON *json;
};

// Structure that contains all information for a REST request
struct RestRequest {
    // request specific part of URI
    char endpoint[20];
    // the final compiled address
    char *address;
    // length of address (without \0)
    size_t size;
};

// Structure that contains the basic REST information (how to connect to the server)
struct RestConfig {
    // http | https
    char protocol[6];
    // name or ip address
    char hostname[30];
    // 80 | 8080
    char port[7];
    // Deployment location of the PiBell-Server web-app (e.g., pibell)
    char deploymentLocation[50];
};
    
///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
/**
 * Creates a new Rest object that can be used to initiate REST API calls.
 * @param[in] RestConmfig restConf The struct that contains all necessary
 *            information to perform REST calls to the PiBell-Server backend.
 * @param[in] String recipientName The current PiBell-Pi username as defined in
 *            the configuration ini file. It will be used to check for incoming
 *            calls.
 * @param[in] bool verbose True - Logging output for all REST API calls will be
 *            printed to the console.
 *            False - Otherwise
 * @return A fully initialized Rest object that can be used to perform REST calls.
 */
extern void * Rest_new(struct RestConfig *restConf, const char *recipientName,
        bool verbose);

/**
 * Destroys the given Rest object and frees all allocated memory.
 * @param[in] Rest rest The Rest object to be destroyed.
 */
extern void Rest_delete(void *rest);

/**
 * This method performs a 'ping' request to the PiBell-Server backend. This
 * request checks for incoming calls targeted to the current Pi and returns
 * a JSON object with more information about the call. However, requests might
 * also fail due to server outages or internal server errors. In these cases
 * the returned JSON object contains a JSON error object.
 * @param[in] Rest rest The current Rest object used to perform the request.
 * @return A JSON object with detailed information about the call if the
 *         request was successful. Otherwise a JSON error object that contains
 *         more information about the underlying problem.
 */
extern struct PingJSON * Rest_ping(void *rest);

///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
/**
 * Evaluates the given REST configration file information and creates the
 * respective REST interface address for the given endpoint (e.g., the ping
 * request endpoint).
 * @param[in] RestConfig conf The read REST configuration part that contains
 *            all information necessary to connect to the PiBell-Server backend.
 * @param[in] String endpoint The REST interface endpoint that is specific to
 *            a particular request, such as the 'ping' request. (This is the
 *            non-static part of the server address that changes for each unique
 *            endpoint).
 * @return A RestRequest struct that contains the full address to access the
 *         endpoint at the PiBell-Server backend.
 */
static struct RestRequest * createRequest_(struct RestConfig *conf,
        const char *endpoint);

/**
 * Performs a 'ping' REST request to the PiBell-Server backend and evaluates
 * whether the request has failed due to a server outtage. If this is not the
 * case, the method evaluates the returned JSON result and transforms it into
 * a PingJSON struct, which might then contain more information about a
 * possible received call request.
 * @param[in] Rest rest The Rest object used to perform the 'ping' request.
 * @param[in,out] PingJSON ping The parsed JSON result from the 'ping' request,
 *                or an empty struct if the REST interface is not accessible.
 * @return The CURLcode - if the REST interface is not accessible
 *                        (connection error)
 *         <0 - if the 'ping' REST interface returned a JSON error object
 *              (PiBell internal error)
 *         CURLE_OK - if the returned JSON object was successful
 *                    (all good)
 */
static int doPingRequest_(Rest *rest, struct PingJSON *ping);

/**
 * This method performs a REST request to the given address via curl and
 * appends the given POST data fields. The JSON result of the performed request
 * will be stored in the given RestResponse object.
 * @param[in] String address The full server address used to perform a REST
 *            call.
 * @param[in] String fields The concatenated POST data that should be sent
 *            along with the REST request.
 * @param[in,out] RestResponse result The RestResponse struct that contains
 *                the JSON result for the performed REST request.
 * @param[in] bool verbose True - Debugging output will be printed while the
 *            REST request is performed.
 *            False - Otherwise.
 * @return A CURLcode that indicates whether the curl request was successful or
 *         not. It'll also provide further information in the latter case.
 */
static CURLcode performRequest_(char *address, const char *fields,
        struct RestResponse *result, bool verbose);

/**
 * This method disables some curl SSL options to allow SSL connections to less
 * secure HTTPS REST interfaces.
 * @param[in] CURL curl The curl object whose SSL parameters shall be altered.
 */
static void addSSLOptions_(CURL *curl);

/**
 * Enriches a given curl request with POST data parameters that will be sent
 * along with the request.
 * @param[in] CURL curl The curl object used to add POST request parameters.
 * @param[in] String fields All concatenated parameters that'll be appended
 *            to the curl POST request.
 */
static void addPOSTFields_(CURL *curl, const char *fields);

/**
 * A callback function for the 'ping' REST request that is called from within
 * curl. It will parse the result returned from curl and transform it into
 * a cJSON object.
 * @param[in] String ptr The returned String response from the curl request.
 * @param[in] int size The size of one element of the String response in bytes.
 * @param[in] int nmemb The number of elements in the String response.
 * @param[in] RestResponse userp The RestResponse struct to store the parsed
 *            JSON results in.
 * @return The number of parsed bytes in the curl response String.
 */
static size_t ping_Cb_(void *ptr, size_t size, size_t nmemb, void *userp);

/**
 * Checks whether the returned JSON object from a previously performed request
 * is an error object.
 * @param[in] cJSON root The root node of the returned JSON object.
 * @return True - The given JSON object is an error object.
 *         False - otherwise.
 */
static bool isJSONError_(cJSON *root);

#endif

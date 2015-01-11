#ifndef REST_H
#define REST_H

#include <stdbool.h>
#include <curl/curl.h>
#include "../inc/cJSON.h"

#define HTTP "http"
#define HTTPS "https"
// FIXME: Read the following information from an external properties file
// Backend remote domain address
#define REST_REMOTE_ADDRESS "yourdomain.com"
// We assume an SSL connection (HTTPS) when remotely connecting to the backend
#define REST_REMOTE_PORT 443
// Locahost address for local test connections
#define REST_LOCAL_ADDRESS "localhost"
// Default Tomcat port
#define REST_LOCAL_PORT 8080
// The static part of the URI where the PiBell backend is located
#define REST_URI "rest/v1/answerit"

// The REST URI endpoint for the ping service
#define PING_ENDPOINT "ping"
// Ping service parameters: uid:String
#define PING_PARAM_UID "uid"

typedef struct _Rest {
    char *recipientName;
    struct RestRequest *pingReq;
    bool verbose;
} Rest;

// The parsed JSON content returned from the PING interface
struct PingJSON {
    bool isCalling;
    char *caller;
};

// The plain response as returned from the backend + the json root node
struct RestResponse {
    char *jsonString;
    cJSON *json;
};

// Structure that contains all information for a REST request
struct RestRequest {
    // http | https
    char protocol[6];
    // name or ip address
    char hostname[30];
    // 80 | 8080
    char port[7];
    // static part of REST URI
    char restURI[17];
    // request specific part of URI
    char endpoint[20];
    // the final compiled address
    char *address;
    // length of address (without \0)
    size_t size;
};

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
// Memory management
extern void * Rest_new(bool local, const char *recipientName, bool verbose);
extern void Rest_delete(void *rest);

// Functionality
extern struct PingJSON * Rest_ping(void *rest);

///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////

static struct RestRequest * createRequest(bool local, const char *endpoint);

static int doPingRequest(Rest *rest, struct PingJSON *ping);
static CURLcode performRequest(char *address, const char *fields,
        struct RestResponse *result, bool verbose);
static void addSSLOptions(CURL *curl);
static void addPOSTFields(CURL *curl, const char *fields);
static size_t pingCallback(void *ptr, size_t size, size_t nmemb, void *userp);
static bool isJSONError(cJSON *root);


#endif

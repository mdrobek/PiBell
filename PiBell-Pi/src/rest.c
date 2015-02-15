#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curl/curl.h>
#include "../inc/rest.h"
#include "../inc/cJSON.h"

// Memory management
void * Rest_new(struct RestConfig *restConf, const char *recipientName, bool verbose) {
    Rest *rest = malloc(sizeof(Rest));
    rest->verbose = verbose;
    rest->recipientName = strdup(recipientName);
    rest->pingReq = createRequest(restConf, PING_ENDPOINT);
	return rest;
};

void Rest_delete(void *rest) {
	if (NULL != rest) {
        Rest *rest_ = (Rest*) rest;
        free(rest_->recipientName);
        free(rest_->pingReq->address);
        free(rest_->pingReq);
        free(rest);
	}
}

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
// Functionality
struct PingJSON * Rest_ping(void *rest) {
    Rest *rest_ = (Rest*) rest;
    struct PingJSON *ping = malloc(sizeof(struct PingJSON));
    ping->isCalling = false;
    ping->caller = NULL;
    if (0 == doPingRequest(rest, ping)) {
        /*printf("## Ping request was successfull\n");*/
    } else {
        printf("Backend returned a json error for request: %s\n",
                rest_->pingReq->address);
    }
	return ping;
}


///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
static struct RestRequest * createRequest(struct RestConfig *conf, const char *endpoint) {
    struct RestRequest *req = malloc(sizeof(struct RestRequest));
    strcpy(req->endpoint, endpoint);
    // The length is equal to all subparts of the URI + 7 additional chars to
    // separate the URI parts and port (://, :, /, /, /) -> this DOES NOT include
    // \0
    req->size = strlen(conf->protocol) + strlen(conf->hostname)
        + strlen(conf->port) + strlen(conf->deploymentLocation) + strlen(REST_API_URI)
        + strlen(req->endpoint) + 7;
    req->address = malloc(sizeof(char)*(req->size+1));
    sprintf(req->address, "%s://%s:%s/%s/%s/%s", conf->protocol, conf->hostname,
            conf->port, conf->deploymentLocation, REST_API_URI, req->endpoint);
    printf("#### starting allocating: %s | %zu %zu\n", req->address,
            strlen(req->endpoint), req->size);
    return req;
}

static int doPingRequest(Rest *rest, struct PingJSON *ping) {
    char fields[20];
    sprintf(fields, "%s=%s", PING_PARAM_UID, rest->recipientName);
    struct RestResponse *result = malloc(sizeof(struct RestResponse));
    result->jsonString = "";
    result->json = NULL;

    CURLcode res = performRequest(rest->pingReq->address, fields, result,
           rest->verbose);
    
    /* Check for errors */ 
    if(res != CURLE_OK) {
        fprintf(stderr,
                "curl_easy_perform() failed: %s, while trying to connect to: %s\n",
                curl_easy_strerror(res), rest->pingReq->address);
    } else {
        // 1) Check for JSON error code from backend
        if (isJSONError(result->json)) res = -10;
        else {
            // 2) No backend error code, proceed extracting JSON information
            ping->isCalling = cJSON_GetObjectItem(result->json,
                "isCalling")->valueint;
            ping->caller = strdup(cJSON_GetObjectItem(result->json, "caller")
                ->valuestring);
        }
        // Clean up the Response struct content
        free(result->jsonString);
        cJSON_Delete(result->json); 
    }
    free(result);
    return res;
}

static CURLcode performRequest(char* address, const char *fields,
        struct RestResponse *result, bool verbose)
{
    CURL *curl = NULL;
    CURLcode res = -1;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, address);
        /* we want to use our own read function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, pingCallback);
        /* pointer to pass to our read function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
        /* get verbose debug output please */ 
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
        if (0 == strncmp(address, HTTPS, strlen(HTTPS)))
            addSSLOptions(curl);
        if (NULL != fields && strcmp("", fields))
            addPOSTFields(curl, fields);
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    return res;
}

static void addSSLOptions(CURL *curl) {
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */ 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */ 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
}

/**
 * 
 * @param curl
 * @param fields
 */
static void addPOSTFields(CURL *curl, const char *fields) {
    /* Now specify we want to POST data */ 
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(fields));
}

static bool isJSONError(cJSON *root) {
    if (NULL != root) {
        cJSON *errorNode = cJSON_GetObjectItem(root, "isError");
        if (errorNode) return errorNode->valueint;
    }
    return false;
}


static size_t pingCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    if(size*nmemb < 1) return 0;

    /*printf("json return: %s\n", ptr);*/
    struct RestResponse *pooh = (struct RestResponse *)userp;
    size_t realsize = size * nmemb;
    pooh->jsonString = malloc(realsize+1);
    memcpy(pooh->jsonString, ptr, realsize);
    pooh->json = cJSON_Parse(pooh->jsonString);
    return realsize;
}

#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include "../inc/ctrler.h"
#include "../inc/rest.h"
#include "../inc/ini.h"

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
extern void * Ctrler_new(char *execPath, char *configFileName) {
    Ctrler *ctrler = malloc(sizeof(Ctrler));
    ctrler->execDirName = dirname(execPath);
    ctrler->resourceFolder = getResourceFolder(ctrler);
    ctrler->mainConf = readConfigFile_(ctrler, configFileName);
    return ctrler; 
}

extern void Ctrler_delete(void *ctrler) {
    if (NULL != ctrler) {
        Ctrler *myCtrler = (Ctrler*) ctrler;
        free(myCtrler->mainConf);
        free(myCtrler->mainConf);
        free(ctrler);
    }
}

// Functionality
extern void Ctrler_run(void *ctrler) {
    Ctrler *myCtrler = (Ctrler*) ctrler;
    Rest *rest = Rest_new(myCtrler->mainConf->restConf, myCtrler->mainConf->username,
           myCtrler->mainConf->verbose);
    printf("new rest is: %s | %s\n", rest->pingReq->address,
            rest->recipientName);
    while(1) {
        // 1) Do the ping to the backend
        struct PingJSON *ping = Rest_ping(rest);
        // 2) Check, if someone is calling ...
        if (ping->isCalling) {
            // a) Print something for notification
            printf("Someone is calling: %d | %s\n", ping->isCalling,
                   ping->caller); 
            // b) Ring the bell
            ring_(getResourceFolder(ctrler), RINGTONE_FILENAME, 0);
        } else {
            // TODO: Do some logging or so
        }
        // 3) clean up
        if (NULL != ping->caller && strcmp("", ping->caller)) {
            free(ping->caller);
        } 
        free(ping);
        // 4) Go to sleep for a couple of seconds
        sleep(REQUEST_SLEEP_TIME);
    }
    // FIXME: Is never happening -> check for SIGNALS
    Rest_delete(rest);
}

///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
static void ring_(char *resourceFolder, char *ringtoneName, int vol) {
    if (NULL == ringtoneName || !strcmp("", ringtoneName)) {
        fprintf(stderr, "No ringtone has been provided: %s\n", ringtoneName);
    }
    char omxCmd[100];
    if (vol > 0) {
        sprintf(omxCmd, "%s %s %d %s%s", OMXPLAYER, OMX_VOL, vol,
                resourceFolder, ringtoneName);
    } else {
        sprintf(omxCmd, "%s %s%s", OMXPLAYER, resourceFolder, ringtoneName);
    }
    printf("#### Ringing the bell with command: %s\n", omxCmd);
    // b) Check if we're on the PI, and if so, ring the bell, otherwise do
    //    nothing
    if (isARMPlattform_()) {
        // Start omxplayer and play the sound
        system(omxCmd);
        // Close the player (just in case) -> previous command is blocking
        // and afterwards the command is finished anyways -> this causes
        // an error when killall runs
        /*system("killall omxplayer.bin");*/
    }
}

static bool isARMPlattform_() {
    bool isARM = !strcmp("arm", CMAKE_PLATTFORM);
    /*printf("plattform: %s, %d\n", CMAKE_PLATTFORM, isARM);*/
    return isARM;
}

static struct Config * readConfigFile_(Ctrler *ctrler, char *configFileName) {
    struct Config *mainConf = malloc(sizeof(struct Config));
    mainConf->restConf = NULL;
    mainConf->username = NULL;
    mainConf->verbose = false;
    char *filePath = malloc(strlen(ctrler->resourceFolder)
            + strlen(configFileName) + 2);
    sprintf(filePath, "%s/%s", ctrler->resourceFolder, configFileName);
    if (ini_parse(filePath, configFileReader_Cb_, mainConf) < 0) {
        fprintf(stderr, "[ERROR]: Can't load config file '%s'\n", filePath);
        exit(-1);
    }
    if (mainConf->verbose) {
        printf("Config loaded successfully from file '%s'\n: \t%s\n | \t%d\n | "
                "\t%s\n | \t%s\n | \t%s\n | \t%s\n",
                filePath, mainConf->username, mainConf->verbose,
                mainConf->restConf->protocol, mainConf->restConf->hostname,
                mainConf->restConf->port, mainConf->restConf->deploymentLocation);
    }
    return mainConf;
}

static int configFileReader_Cb_(void *conf, const char *section,
        const char *name, const char *value) {
    struct Config *mainConf = (struct Config*) conf;
    if (NULL == mainConf->restConf) {
        mainConf->restConf = malloc(sizeof(struct RestConfig));
    }
    
    /*printf("In callback function: %s | %s | %s\n", section, name, value);*/

    if (MATCH("Login", "piUsername")) {
        mainConf->username = strdup(value);
    } else if (MATCH("Debug", "verbose")) {
        mainConf->verbose = GETBOOL(value);
    // RestConfig part
    } else if (MATCH("Server", "hostname")) {
        // Check if empty (this means we're using localhost)
        char *curHostname = (0 == strlen(value)) ?
            strdup(REST_LOCAL_ADDRESS) :
            strdup(value);
        strcpy(mainConf->restConf->hostname, curHostname);
    } else if (MATCH("Server", "port")) {
        strcpy(mainConf->restConf->port, value);
    } else if (MATCH("Server", "deployedAt")) {
        strcpy(mainConf->restConf->deploymentLocation, value);
    } else if (MATCH("Server", "useSSL")) {
        char *protocol = GETBOOL(value) ? HTTPS : HTTP;
        strcpy(mainConf->restConf->protocol, protocol);
    // unknown section/name, error
    } else {
        return -1;
    }
    return 1;
}

static char * getResourceFolder(Ctrler *ctrler) {
    char *resourcePath = malloc(strlen(ctrler->execDirName)
            + strlen(RESOURCE_FOLDER) + 5);
    sprintf(resourcePath, "%s/../%s", ctrler->execDirName, RESOURCE_FOLDER);
    /*printf("Resource path is: %s\n", resourcePath);*/
    return resourcePath;
}

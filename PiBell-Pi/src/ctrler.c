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
            ring_(RINGTONE_FILENAME, 0);
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
/**
 * @param vol The volume is in between [-6000, 0], with 0 being the loudest and
 *            -6000 being very quiet
 */
static void ring_(char *ringtoneName, int vol) {
    if (NULL == ringtoneName || !strcmp("", ringtoneName)) {
        fprintf(stderr, "No ringtone has been provided: %s\n", ringtoneName);
    }
    char omxCmd[100];
    if (vol > 0) {
        // length of strings + 3 blanks + \0 sequence
        /*int cmdLength = strlen(OMXPLAYER) + strlen(OMX_VOL) +...*/
            /*+ strlen(RINGTONE_FOLDER) + strlen(ringtoneName) + 4;*/
        /*omxCmd = (char*) malloc(sizeof(char)*cmdLength);*/
        sprintf(omxCmd, "%s %s %d %s%s", OMXPLAYER, OMX_VOL, vol,
                RINGTONE_FOLDER, ringtoneName);
    } else {
        sprintf(omxCmd, "%s %s%s", OMXPLAYER, RINGTONE_FOLDER, ringtoneName);
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
    char *filePath = malloc(strlen(ctrler->resourceFolder)
            + strlen(configFileName) + 2);
    printf("Building path to template file: %s | %s\n", ctrler->resourceFolder,
            configFileName);
    sprintf(filePath, "%s/%s", ctrler->resourceFolder, configFileName);
    printf("File path is: %s\n", filePath);
    if (ini_parse(filePath, configFileReader_Callback_, mainConf) < 0) {
        printf("Can't load config file '%s'\n", filePath);
    }
    printf("Config loaded successfully from file '%s': %s | %d | %s | %s | %s | %s\n",
            filePath, mainConf->username, mainConf->verbose,
            mainConf->restConf->protocol, mainConf->restConf->hostname,
            mainConf->restConf->port, mainConf->restConf->deploymentLocation);
    return mainConf;
}

static int configFileReader_Callback_(void *conf, const char *section, const char *name,
        const char *value) {
    struct Config *mainConf = (struct Config*) conf;
    if (NULL == mainConf->restConf) {
        mainConf->restConf = malloc(sizeof(struct RestConfig));
    }
    
    printf("In callback function: %s | %s | %s\n", section, name, value);

    if (MATCH("Login", "piUsername")) {
        mainConf->username = strdup(value);
    } else if (MATCH("Debug", "verbose")) {
        mainConf->verbose = GETBOOL(value);
    // RestConfig part
    } else if (MATCH("Server", "hostname")) {
        strcpy(mainConf->restConf->hostname, value);
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
    printf("Resource path is: %s\n", resourcePath);
    return resourcePath;
}

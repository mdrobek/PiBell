#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../inc/ctrler.h"
#include "../inc/rest.h"

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
extern void * Ctrler_new() {
    Ctrler *ctrler = malloc(sizeof(Ctrler));
    return ctrler; 
}

extern void Ctrler_delete(void *ctrler) {
    if (NULL != ctrler) free(ctrler);
}

// Functionality
extern void Ctrler_run(void *ctrler, bool local, const char *recipientName,
        bool verbose) {
    Rest *rest = Rest_new(local, recipientName, verbose);
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

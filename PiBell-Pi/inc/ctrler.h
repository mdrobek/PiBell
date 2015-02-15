#ifndef CTRLER_H
#define CTRLER_H

#include <stdbool.h>
#include <strings.h>
#include <string.h>

#ifndef CMAKE_PLATTFORM
// Set the default plattform to x86
// Macro will be set by default when running make
#define CMAKE_PLATTFORM "x86"
#endif

#define OMXPLAYER "omxplayer"
#define OMX_VOL "--vol"
#define RESOURCE_FOLDER "res"
#define TEMPLATE_CONFIG_FILE "template.ini"
#define RINGTONE_FOLDER "/home/pi/PiBell/res/"
#define RINGTONE_FILENAME "ring.mp3"
// sleep time in seconds before restarting the ping request
#define REQUEST_SLEEP_TIME 3

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
#define GETBOOL(sb) (strcasecmp(sb, "true") == 0 || atoi(sb) != 0)

typedef struct _Ctrler {
    char *execDirName;
    char *resourceFolder;
    struct Config *mainConf;
} Ctrler;

// Structure that contains all configuration parameters
struct Config {
    // The Pi username it'll use to check for ring requests
    char *username;
    // whether to print debug information or not
    bool verbose;
    // The REST config (how to connect to the backend server)
    struct RestConfig *restConf;
};

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
/*
 * @param execPath The relative path to the PiBell-Pi executable as seen from the users
 *                 current working directory (cwd)
 */
extern void * Ctrler_new(char *execPath, char *configFileName);
extern void Ctrler_delete(void *ctrler);

// Functionality
extern void Ctrler_run(void *ctrler);



///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
static void ring_(char *ringtoneName, int vol);
static bool isARMPlattform_();
static struct Config * readConfigFile_(Ctrler *ctrler, char *configFileName);
static int configFileReader_Callback_(void* user, const char* section, const char* name,
        const char* value);
/**
 * @return The path to the resource folder (without trailing /)
 */
static char * getResourceFolder(Ctrler *ctrler);
#endif

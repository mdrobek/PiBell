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

// The name of the audio tool on the ARM plattform to play an mp3 ringtone 
#define OMXPLAYER "omxplayer"
// Audio tool argument to control the volume
#define OMX_VOL "--vol"
// Name of the resource folder
#define RESOURCE_FOLDER "res"
// The default configuration file name
#define TEMPLATE_CONFIG_FILE "template.ini"
// The name of the mp3 ringtone file
#define RINGTONE_FILENAME "ring.mp3"
// sleep time in seconds before restarting the ping request
#define REQUEST_SLEEP_TIME 3

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
#define GETBOOL(sb) (strcasecmp(sb, "true") == 0 || atoi(sb) != 0)

// The Controller object that stores all necessary information to run the PiBell
typedef struct _Ctrler {
    char *execDirName;
    char *resourceFolder;
    struct Config *mainConf;
} Ctrler;

// Structure that contains all configuration parameters
struct Config {
    // The Pi username used to check for ring requests
    char *username;
    // whether to print debug information or not
    bool verbose;
    // The REST config (how to connect to the backend server)
    struct RestConfig *restConf;
};

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
/**
 * Constructs a new Controller object with the given information.
 * @param[in] String execPath The relative path to the PiBell-Pi executable as
 *            seen from the users current working directory (cwd)
 * @param[in] String configFileName Name of the configuration file
 * @return A newly constructed controller object.
 */
extern void * Ctrler_new(char *execPath, char *configFileName);

/**
 * Destroys the given controller object by freeing all allocated memory space.
 * @param[in] Ctrler The Controller object
 */
extern void Ctrler_delete(void *ctrler);

/**
 * This method starts the main loop which continuously pings the PiBell-Server
 * backend to check for a newly registered call to this PiBell-Pi instance.
 * @param[in] Ctrler The Controller object
 */
extern void Ctrler_run(void *ctrler);

///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
/**
 * This method has a plattform-dependent behaviour:
 * - arm: On the Raspberry Pi, this method runs the omxplayer program to play
 *        the given ringtone mp3 file.
 * - x86: On x86 plattforms, this method only prints the omxplayer command
 *        it would execute on an ARM plattform to the console.
 * @param[in] String resourceFolder The absolute path to the resource folder.
 * @param[in] String ringtoneName The name of the mp3 ringtone file.
 * @param[in] int vol The volume is in between [-6000, 0], with 0 being the
 *            loudest and -6000 being very quiet
 */
static void ring_(char *resourceFolder, char *ringtoneName, int vol);

/**
 * This method checks, whether the program is currently running on an ARM
 * plattform (Pi) or on an x86 system.
 * @return True - The program currently runs on an ARM plattform.
 * False - Otherwise.
 */
static bool isARMPlattform_();

/**
 * Reads the configuration ini file from the given file name and stores it in
 * a Config struct element.
 * @param[in] Ctrler ctrler The current controller object.
 * @param[in] String configFileName The name of the configuration ini file
 *            located in the resource folder.
 * @return A parsed Config struct that contains all the configuration parameters.
 */
static struct Config * readConfigFile_(Ctrler *ctrler, char *configFileName);

/**
 * A callback function used to read specific sections and parameters from a
 * configuration ini file.
 * @param[in] Config user The Config struct object used to store the
 *            configuration parameters. (If NULL, this method creates a new
 *            Config struct.)
 * @param[in] String section The current section part of the configarion file.
 * @param[in] String name The name of the currently read config parameter.
 * @param[in] String value The value of the currently read config parameter.
 * @return >=0 - If the given parameter is known and could be stored in the
 *         Config struct.
 *         <0 - Otherwise.
 */
static int configFileReader_Cb_(void* user, const char* section, const char* name,
        const char* value);

/**
 * This method returns the absolute path to the PiBell-Pi resource folder.
 * @param[in] Ctrler ctrler The controller object.
 * @return The path to the resource folder (without the trailing /)
 */
static char * getResourceFolder(Ctrler *ctrler);
#endif

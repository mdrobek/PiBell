#ifndef CTRLER_H
#define CTRLER_H

#include <stdbool.h>

#ifndef CMAKE_PLATTFORM
// Set the default plattform to x86
// Macro will be set by default when running make
#define CMAKE_PLATTFORM "x86"
#endif

#define OMXPLAYER "omxplayer"
#define OMX_VOL "--vol"
#define RINGTONE_FOLDER "/home/pi/PiBell/res/"
#define RINGTONE_FILENAME "ring.mp3"
// sleep time in seconds before restarting the ping request
#define REQUEST_SLEEP_TIME 3

typedef struct _Ctrler {} Ctrler;

///////////////////////////////////////////////////////////////////////////////
///                               PUBLIC METHODS                            ///
///////////////////////////////////////////////////////////////////////////////
extern void * Ctrler_new();
extern void Ctrler_delete(void *ctrler);

// Functionality
extern void Ctrler_run(void *ctrler, bool local, const char *recipientName, bool verbose);



///////////////////////////////////////////////////////////////////////////////
///                               PRIVATE METHODS                           ///
///////////////////////////////////////////////////////////////////////////////
static void ring_(char *ringtoneName, int vol);
static bool isARMPlattform_();
#endif

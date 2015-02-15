#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../inc/ctrler.h"

int main (int argc, char *argv[])
{
    char *configFileName = NULL;
    int readOption;

    // 1) Check for config file input
    while ((readOption = getopt(argc, argv, "c:")) != -1) {
        switch (readOption) {
            case 'c':
                configFileName = optarg;
                break;
          case '?':
                if (optopt == 'c')
                    fprintf(stderr,
                            "Option -%c requires the configuration file name.\n",
                            optopt);
                else
                    fprintf(stderr, "Missing config file name. Usage is as follows:\n"
                            "\tPiBell-arm|x86 -c remoteConfigFile.ini\n"
                            "NOTE: All config files need to be located in the "
                            "res/ folder.\n");
                return -1;
          default:
                abort();
          }
    }

    // 2) Check if the config file name is given and if not, quit the program with an
    //    appropriate message
    if (NULL == configFileName) {
        fprintf(stderr, "Missing config file name. Usage is as follows:\n"
            "\tPiBell-arm|x86 -c remoteConfigFile.ini\n"
            "NOTE: All config files need to be located in the res/ folder.\n");
        return -1;
    } else {
        // 3) Initialize the controller
        Ctrler *ctrler = Ctrler_new(argv[0], configFileName);
        // 4) Start the main while loop
        Ctrler_run(ctrler);
        // 5) Free memory
        Ctrler_delete(ctrler);
    }
    return 0;
}

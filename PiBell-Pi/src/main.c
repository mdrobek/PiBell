#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../inc/ctrler.h"

int main (int argc, char *argv[])
{
    // The PiBell username of the pi
    /*const char *recipientName = "myPiBellUsername";*/
    // Contact localhost server (false => connects remote server)
    /*bool local = false;*/
    // Print verbose output
    /*bool verbose = false;*/
    /*char cwd[100];*/
    /*printf("argv[0]: %d | %s | %s \n", argc, argv[0], getcwd(cwd, 100));*/

    // 1) Initialize the controller
    Ctrler *ctrler = Ctrler_new(argv[0], TEMPLATE_CONFIG_FILE);
    // 2) Start the main while loop
    Ctrler_run(ctrler);
    // 3) Free memory
    Ctrler_delete(ctrler);

    return 0;
}

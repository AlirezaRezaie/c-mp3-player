#include "globals.h"
#include <fcntl.h>

char ch;
/* adjusts audio volume asyncrounasly */
void* adj_vol(void *arguments){
    struct arg_struct *args = arguments;
    printf("INFO : you can turn the volume up with (p+) key and down with (o-)\n[press 'E' to exit]\n");

    // setting terminal to non-canonical mode and disabling echo
    struct termios info;
    fcntl(0, F_SETFL, O_NONBLOCK); /* make it non-blockin for constantly checking for if running */
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_lflag &= ~ECHO;        /* disable user input logging (echo) */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    unsigned short int vol= 100;
    while((ch = getchar()) != 'E'  & args ->running) {
        switch (ch)
        {
        case 'p':
            vol += 10;
            printf("\n%d\r",vol);
            libvlc_audio_set_volume(args -> mp,vol);
            break;
        case 'o':
            vol -= +10;
            printf("\n%d\r",vol);
            libvlc_audio_set_volume(args -> mp,vol);
            break;
        }
    }

    // setting the terminal back to canonical mode
    tcgetattr(0, &info);
    info.c_lflag |= ICANON;
    info.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &info);

    printf("exiting volume controler thread\n");
    return NULL;
}
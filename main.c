#include <stdio.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char **argv)
{
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    // load the vlc engine
    if ( (inst = libvlc_new(0, NULL)) == 0){
        printf("%s",libvlc_errmsg ());
        return 1;
    }
    
    // create a new item
    m = libvlc_media_new_path(inst, argv[1]);

    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);

    // play the media_player
    libvlc_media_player_play(mp);

    struct termios info;
    fcntl(0, F_SETFL, O_NONBLOCK); /* make it non-blockin for constantly checking for if running */
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_lflag &= ~ECHO;        /* disable user input logging (echo) */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    // insuring the media started playing instead of blindly sleeping
    while (!libvlc_media_player_is_playing(mp));

    unsigned short int vol= 100;
    char ch;
    long int sec = time(NULL);
    while (libvlc_media_player_is_playing(mp) && (ch = getchar()) != 'E')
    {
        int64_t milliseconds = libvlc_media_player_get_time(mp);
        int64_t seconds = milliseconds / 1000;
        int64_t minutes = seconds / 60;
        milliseconds -= seconds * 1000;
        seconds -= minutes * 60;

        if (time(NULL) >=  sec + 1){
        printf("Current time: %ld:%ld\r", minutes, seconds);
        sec = time(NULL);
        }
        switch (ch)
        {
        case 'p':
            vol += 10;
            printf("\n%d\r",vol);
            libvlc_audio_set_volume(mp,vol);
            break;
        case 'o':
            vol -= +10;
                    fflush(stdout);

            printf("\n%d\r",vol);
            libvlc_audio_set_volume( mp,vol);
            break;
        }
        fflush(stdout);

    }

    // setting the terminal back to canonical mode
    tcgetattr(0, &info);
    info.c_lflag |= ICANON;
    info.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &info);

    printf("volume thread terminated successfully\n");

    // Stop playing
    libvlc_media_player_stop (mp);
 
    // Free the media_player
    libvlc_media_player_release (mp);
 
    // free the libvlc instance
    libvlc_release (inst);

    return 0;
}
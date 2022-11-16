#include "globals.h"

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

    // initializing thread arguments
    struct arg_struct t;
    struct arg_struct *av_args = &t;
    av_args->mp = mp;
    av_args->running = true;

    // create volume adjust thread
    pthread_t av;
    printf("initializing vol adjust thread\n");
    pthread_create(&av, NULL, adj_vol,(void*)av_args);

    sleep(1);
    while (libvlc_media_player_is_playing(mp) && ch != 'E')
    {
        int64_t milliseconds = libvlc_media_player_get_time(mp);
        int64_t seconds = milliseconds / 1000;
        int64_t minutes = seconds / 60;
        milliseconds -= seconds * 1000;
        seconds -= minutes * 60;

        printf("Current time: %ld:%ld\r", minutes, seconds);
        fflush(stdout);
        sleep (1);

    }
    printf("\na");
    fflush(stdout);
    /* av thread listens for running argument to see if it should continue working
       setting it to false cause it to break out of while loop and terminates the thread
    */ 
    av_args->running = false;
    
    // waiting to fully terminate
    pthread_join(av,NULL);
    fflush(stdout);
    printf("volume thread terminated successfully\n");

    // Stop playing
    libvlc_media_player_stop (mp);
 
    // Free the media_player
    libvlc_media_player_release (mp);
 
    // free the libvlc instance
    libvlc_release (inst);

    return 0;
}
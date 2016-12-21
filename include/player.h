#ifndef PLAYER_H
#define PLAYER_H

#include <vlc/vlc.h>

#include <mutex>

class PlayerController;


class Player
{
public:
    Player();
    ~Player();
    void *p_imgData;
    std::mutex mutex;

    // Frame size
    unsigned i_width;
    unsigned i_height;

    void setOnPositionChangedCallback(PlayerController *c, std::function<void(PlayerController&, float)> f);
    void setPlayingCallback(PlayerController *c, std::function<void(PlayerController&)> f);
    void setPausedCallback(PlayerController *c, std::function<void(PlayerController&)> f);
    void startPlayback(std::string mediaPath);
    void pause();
    void play();
    void changePosition(float pos);

    //std::function<void(PlayerController&, float)> testPointer;

private:
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_event_manager_t *evManager;

    std::function<void(PlayerController&, float)> onPositionChanged;
    PlayerController *onPositionChangedController;

    std::function<void(PlayerController&)> onPlaying;
    PlayerController *onPlayingController;

    std::function<void(PlayerController&)> onPaused;
    PlayerController *onPausedController;

    static void libVLCPositionChanged(const struct libvlc_event_t *ev, void *data);
    static void libVLCPlaying(const struct libvlc_event_t *ev, void *data);
    static void libVLCPaused(const struct libvlc_event_t *ev, void *data);
};

void *lock(void *opaque, void **planes);
void display(void *opaque, void *picture);
unsigned format(void **opaque, char *chroma,
                unsigned *width, unsigned *height,
                unsigned *pitches,
                unsigned *lines);
void log(void *data, int level, const libvlc_log_t *ctx,
         const char *fmt, va_list args);


#endif // PLAYER_H

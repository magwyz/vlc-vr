
#include <iostream>
#include <cstring>

#include "player.h"


Player::Player()
    : onPositionChangedController(NULL),
      onPlayingController(NULL),
      onPausedController(NULL)
{
    inst = libvlc_new(0, NULL);
    libvlc_log_set(inst, log, NULL);
}


Player::~Player()
{
    libvlc_release(inst);
}


void *lock(void *opaque, void **planes)
{
    Player *p = (Player *)opaque;

    p->mutex.lock();
    planes[0] = p->p_imgData;

    return NULL;
}


void display(void *opaque, void *picture)
{
    Player *p = (Player *)opaque;
    p->mutex.unlock();
}


unsigned format(void **opaque, char *chroma,
                unsigned *width, unsigned *height,
                unsigned *pitches,
                unsigned *lines)
{
    Player *p = (Player *)*opaque;
    memcpy(chroma, "RV24", 4);
    *pitches = (*width) * 3;
    *lines = *height;

    p->i_height = *height;
    p->i_width = *width;
    p->p_imgData = (void*)aligned_alloc(32, (*width) * (*height) * 3);
}


void log(void *data, int level, const libvlc_log_t *ctx,
         const char *fmt, va_list args)
{
    /*vprintf(fmt, args);
    printf("\n");*/
}


void Player::startPlayback(std::string mediaPath)
{
    libvlc_media_t *m;

    /* Create a new item */
    m = libvlc_media_new_path(inst, mediaPath.c_str());

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media(m);
    evManager = libvlc_media_player_event_manager(mp);

    libvlc_event_attach(evManager, libvlc_MediaPlayerPositionChanged, libVLCPositionChanged, this);
    libvlc_event_attach(evManager, libvlc_MediaPlayerTimeChanged, libVLCTimeChanged, this);
    libvlc_event_attach(evManager, libvlc_MediaPlayerLengthChanged, libVLCLengthChanged, this);
    libvlc_event_attach(evManager, libvlc_MediaPlayerPlaying, libVLCPlaying, this);
    libvlc_event_attach(evManager, libvlc_MediaPlayerPaused, libVLCPaused, this);

    /* No need to keep the media now */
    libvlc_media_release(m);

    libvlc_video_set_callbacks(mp, lock, NULL, display, (void *)this);
    libvlc_video_set_format_callbacks(mp, format, NULL);

    libvlc_media_player_play(mp);
}


void Player::pause()
{
    libvlc_media_player_pause(mp);
}


void Player::play()
{
    libvlc_media_player_play(mp);
}


void Player::changePosition(float pos)
{
    libvlc_media_player_set_position(mp, pos);
}


void Player::setOnPositionChangedCallback(PlayerController *c, std::function<void(PlayerController&, float)> f)
{
    onPositionChanged = f;
    onPositionChangedController = c;
}


void Player::libVLCPositionChanged(const struct libvlc_event_t *ev, void *data)
{
    Player *p = (Player *)data;
    float pos = libvlc_media_player_get_position(p->mp);
    if (p->onPositionChangedController != NULL)
        p->onPositionChanged(*(p->onPositionChangedController), pos);
}


void Player::setTimeChangedCallback(PlayerController *c, std::function<void(PlayerController&, int64_t)> f)
{
    timeChanged = f;
    timeChangedController = c;
}


void Player::libVLCTimeChanged(const struct libvlc_event_t *ev, void *data)
{
    Player *p = (Player *)data;
    int64_t t = libvlc_media_player_get_time(p->mp);
    if (p->timeChangedController != NULL)
        p->timeChanged(*(p->timeChangedController), t);
}


void Player::setLengthChangedCallback(PlayerController *c, std::function<void(PlayerController&, int64_t)> f)
{
    lengthChanged = f;
    lengthChangedController = c;
}


void Player::libVLCLengthChanged(const struct libvlc_event_t *ev, void *data)
{
    Player *p = (Player *)data;
    int64_t t = libvlc_media_player_get_length(p->mp);
    if (p->lengthChangedController != NULL)
        p->lengthChanged(*(p->lengthChangedController), t);
}


void Player::setPlayingCallback(PlayerController *c, std::function<void(PlayerController&)> f)
{
    onPlaying = f;
    onPlayingController = c;
}


void Player::libVLCPlaying(const struct libvlc_event_t *ev, void *data)
{
    Player *p = (Player *)data;
    if (p->onPlayingController != NULL)
        p->onPlaying(*(p->onPlayingController));
}


void Player::setPausedCallback(PlayerController *c, std::function<void(PlayerController&)> f)
{
    onPaused = f;
    onPausedController = c;
}


void Player::libVLCPaused(const struct libvlc_event_t *ev, void *data)
{
    Player *p = (Player *)data;
    if (p->onPausedController != NULL)
        p->onPaused(*(p->onPausedController));
}

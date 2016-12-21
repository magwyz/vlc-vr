#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <userinterface.h>
#include <player.h>


class PlayerController
{
public:
    PlayerController(Slider *timeSlider, Player *player,
                     Button *play, Button *pause);

    void positionChanged(float pos);
    void playing();
    void paused();
    void playClick();
    void pauseClick();
    void userChangedPosition(float pos);
    void sliderLocked();
    void sliderUnlocked();

private:
    Player *player;
    Slider *timeSlider;
    Button *play;
    Button *pause;
};

#endif // PLAYERCONTROLLER_H

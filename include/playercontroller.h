#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <userinterface.h>
#include <player.h>


class PlayerController
{
public:
    PlayerController(Slider *timeSlider, Player *player,
                     Button *play, Button *pause,
                     Label *curTime, Label *length);

    void positionChanged(float pos);
    void timeChanged(int64_t t);
    void lengthChanged(int64_t t);
    void playing();
    void paused();
    void playClick();
    void pauseClick();
    void userChangedPosition(float pos);
    void sliderLocked();
    void sliderUnlocked();

private:
    std::string getTimeString(int64_t t);

    Player *player;
    Slider *timeSlider;
    Button *play;
    Button *pause;
    Label *curTime;
    Label *length;
};

#endif // PLAYERCONTROLLER_H

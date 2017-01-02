#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <userinterface.h>
#include <player.h>


class PlayerController
{
public:
    PlayerController(Slider<PlayerController> *timeSlider, Player *player,
                     Button<PlayerController> *play, Button<PlayerController> *pause,
                     Label<PlayerController> *curTime, Label<PlayerController> *length,
                     UserInterface<PlayerController> *intfScreen);

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
    void zoomIn();
    void zoomOut();

private:
    std::string getTimeString(int64_t t);

    Player *player;
    Slider<PlayerController> *timeSlider;
    Button<PlayerController> *play;
    Button<PlayerController> *pause;
    Label<PlayerController> *curTime;
    Label<PlayerController> *length;
    UserInterface<PlayerController> *intfScreen;
};

#endif // PLAYERCONTROLLER_H

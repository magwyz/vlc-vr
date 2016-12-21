#include <iostream>

#include "playercontroller.h"


PlayerController::PlayerController(Slider *timeSlider, Player *player,
                                   Button *play, Button *pause)
    : timeSlider(timeSlider), player(player),
      play(play), pause(pause)
{ }


void PlayerController::positionChanged(float pos)
{
    timeSlider->setValue(pos);
}


void PlayerController::userChangedPosition(float pos)
{
    player->changePosition(pos);
}


void PlayerController::sliderLocked()
{
    player->pause();
}


void PlayerController::sliderUnlocked()
{
    player->play();
}


void PlayerController::playing()
{
    std::cout << "Playing" << std::endl;
    play->setVisibility(false);
    pause->setVisibility(true);
}


void PlayerController::paused()
{
    std::cout << "Paused" << std::endl;
    play->setVisibility(true);
    pause->setVisibility(false);
}


void PlayerController::playClick()
{
    player->play();
}


void PlayerController::pauseClick()
{
    player->pause();
}

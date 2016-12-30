#include <iostream>
#include <sstream>
#include <iomanip>

#include "playercontroller.h"


PlayerController::PlayerController(Slider *timeSlider, Player *player,
                                   Button *play, Button *pause,
                                   Label *curTime, Label *length)
    : timeSlider(timeSlider), player(player),
      play(play), pause(pause),
      curTime(curTime), length(length)
{ }


void PlayerController::positionChanged(float pos)
{
    timeSlider->setValue(pos);
}


std::string PlayerController::getTimeString(int64_t t)
{
    std::stringstream ss;
    t /= 1000;
    int64_t min = t / 60;
    int64_t sec = t % 60;
    ss << std::setfill('0') << std::setw(2) << min
       << ":"
       << std::setfill('0') << std::setw(2) << sec;
    return ss.str();
}


void PlayerController::timeChanged(int64_t t)
{
    curTime->setText(getTimeString(t));
}


void PlayerController::lengthChanged(int64_t t)
{
    length->setText(getTimeString(t));
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

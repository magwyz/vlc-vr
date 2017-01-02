#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "playercontroller.h"


PlayerController::PlayerController(Slider<PlayerController> *timeSlider, Player *player,
                                   Button<PlayerController> *play, Button<PlayerController> *pause,
                                   Label<PlayerController> *curTime, Label<PlayerController> *length,
                                   UserInterface<PlayerController> *intf,
                                   UserInterface<PlayerController> *intfScreen)
    : timeSlider(timeSlider), player(player),
      play(play), pause(pause),
      curTime(curTime), length(length),
      intf(intf), intfScreen(intfScreen)
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


void PlayerController::zoomIn()
{
    float x, y, z;
    intfScreen->getPosition(x, y, z);
    z += 0.05f;
    z = std::min(z, -0.7f);
    intfScreen->setPosition(x, y, z);
}


void PlayerController::zoomOut()
{
    float x, y, z;
    intfScreen->getPosition(x, y, z);
    z -= 0.05f;
    z = std::max(z, -4.f);
    intfScreen->setPosition(x, y, z);
}


void PlayerController::toggleScreenVisibility()
{
    bool visible = intf->getVisibility();
    intf->setVisibility(!visible);
}

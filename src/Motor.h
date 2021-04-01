#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#include "Direction.h"

class Motor
{
private:
    int pinA = 0;
    int pinB = 0;
protected:
    Direction direction = Direction::NONE;
    int pwmLevel = 0;
public:
    Motor();
    Motor(int pinA, int pinB);

    void update();

    Direction getDirection() { return direction; }
    void setDirection(Direction direction);
    int getPWMLevel() { return pwmLevel; }
    void setPWMLevel(int pwmLevel);
};

#endif
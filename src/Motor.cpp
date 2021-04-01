#include "Motor.h"

Motor::Motor()
{
    //ctor
}

Motor::Motor(int pinA, int pinB)
{
    this->pinA = pinA;
    this->pinB = pinB;
}

void Motor::update()
{
    switch (direction)
    {
    case Direction::FORWARDS:
        analogWrite(pinA, 0);
        analogWrite(pinB, pwmLevel);
        break;
    case Direction::NONE:
        analogWrite(pinA, 0);
        analogWrite(pinB, 0);
        break;
    case Direction::BACKWARDS:
        analogWrite(pinA, pwmLevel);
        analogWrite(pinB, 0);
        break;
    default:
        break;
    }
}

void Motor::setDirection(Direction direction)
{
    if(direction != this->direction)
    {
        this->direction = direction;
        update();
    }
}

void Motor::setPWMLevel(int pwmLevel)
{
    if(pwmLevel != this->pwmLevel)
    {
        this->pwmLevel = pwmLevel;
        update();
    }
}
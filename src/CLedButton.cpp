#include <Arduino.h>
#include "CLedButton.h"


IPushListener *CLedButton::PushListener = NULL;
int CLedButton::EnableCallbackPin = -1;


CLedButton::CLedButton()
: LedPin(-1)
, ButtonPin(-1)
{
}


CLedButton::~CLedButton()
{
}


void CLedButton::SetPushListener( IPushListener *push_listener )
{
    PushListener = push_listener;
}


void CLedButton::Setup( int led_pin, int button_pin )
{
    LedPin = led_pin;
    ButtonPin = button_pin;

    printf("Setup: LedPin=%d, ButtonPin=%d\n", LedPin, ButtonPin);

    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
    pinMode(ButtonPin, INPUT_PULLUP);
}


void CLedButton::Loop()
{
}


void CLedButton::Light( bool onoff )
{
    digitalWrite(LedPin, onoff);
}


void CLedButton::EnablePushCallback()
{
    attachInterrupt(ButtonPin, CLedButton::ButtonPushed, FALLING);
    EnableCallbackPin = ButtonPin;
}


void CLedButton::DisablePushCallback()
{
    detachInterrupt(EnableCallbackPin);
}


IRAM_ATTR void CLedButton::ButtonPushed()
{
    detachInterrupt(EnableCallbackPin);
    PushListener->OnPush();
}


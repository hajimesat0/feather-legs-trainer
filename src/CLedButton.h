#ifndef __LedButton_h__
#define __LedButton_h__

#include <Arduino.h>
#include "IPushListener.h"

class CLedButton
{
public:
    CLedButton();
    ~CLedButton();

    static void SetPushListener( IPushListener *push_listener );
    static void ButtonPushed();

    void Setup( int led_pin, int button_pin );
    void Loop();

    void Light( bool onoff );
    void EnablePushCallback();
    void DisablePushCallback();


private:
    static IPushListener *PushListener;
    static int EnableCallbackPin;
    int LedPin;
    int ButtonPin;

};

#endif // __LedButton_h__

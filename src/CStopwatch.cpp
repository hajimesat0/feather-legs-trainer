#include <Arduino.h>
#include "CStopwatch.h"

CStopwatch::CStopwatch()
: StartTime(0)
, StopTime(0)
, Running(false)
{
}


CStopwatch::~CStopwatch()
{
}


void CStopwatch::Start()
{
    if( !Running ) {
        StartTime = millis();
        Running = true;
    }
}


void CStopwatch::Stop()
{
    if( Running ) {
        StopTime = millis();
        Running = false;
    }
}


void CStopwatch::Reset()
{
    StartTime = 0;
    StopTime = 0;
    Running = false;
}


unsigned long CStopwatch::Elapsed()
{
    if( Running ) {
        return millis() - StartTime;
    }
    else {
        return StopTime - StartTime;
    }
}


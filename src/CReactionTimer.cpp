#include "CReactionTimer.h"
#include "CLedButton.h"

CReactionTimer::CReactionTimer( unsigned int button_count )
: State( EStatus_Wait )
, Event( EEvent_None )
, WebComm( new CWebComm() )
, ButtonCount(button_count)
, Buttons(new CLedButton[button_count])
, Stopwatch( new CStopwatch() )
, RequestLightTargetButtonIndex(-1)
, RequestLightOnOff(false)
{
}


CReactionTimer::~CReactionTimer()
{
}


void CReactionTimer::Setup()
{
    if( WebComm == NULL )
    {
        return;
    }
    WebComm->Setup( this );

    if( Buttons == NULL )
    {
        return;
    }

    if( ButtonCount != 2 )
    {
        return;
    }
    Buttons[0].Setup( 33, 35 );
    Buttons[1].Setup( 32, 34 );

    CLedButton::SetPushListener( this );

    if( Stopwatch == NULL )
    {
        return;
    }
    Stopwatch->Reset();
}


void CReactionTimer::Loop()
{
    switch( State )
    {
    case EStatus_Wait:
        if( Event == EEvent_ReqLight )
        {
            // ボタンを光らせる
            if( (0 <= RequestLightTargetButtonIndex) && (RequestLightTargetButtonIndex < ButtonCount) )
            {
                Buttons[RequestLightTargetButtonIndex].Light( RequestLightOnOff );
                Buttons[RequestLightTargetButtonIndex].EnablePushCallback();
            }

            // 時間計測開始
            Stopwatch->Start();
            State = EStatus_Measuring;
            Event = EEvent_None;
        }
        break;

    case EStatus_Measuring:
        if( Event == EEvent_Pushed )
        {
            // ボタン消灯　←　即時性が必要なため、割り込みコンテキストで実施
            // 時間計測終了　←　即時性が必要なため、割り込みコンテキストで実施
            // 結果をWebサーバに通知
            WebComm->SendMeasuringResult( RequestLightTargetButtonIndex, Stopwatch->Elapsed() );
            State = EStatus_Wait;
            Event = EEvent_None;
        }
        break;

    deafult:
        break;
    }

    WebComm->Loop();

}


void CReactionTimer::OnReset()
{
}


void CReactionTimer::OnLight( unsigned int button_index, bool onoff )
{
    if( State == EStatus_Wait )
    {
        // イベント通知
        Event = EEvent_ReqLight;
        RequestLightTargetButtonIndex = button_index;
        RequestLightOnOff = onoff;
    }
}


void CReactionTimer::OnPush()
{
    if( State == EStatus_Measuring )
    {
        // 時間計測を終了する
        Stopwatch->Stop();

        // LED消灯
        if( (0 <= RequestLightTargetButtonIndex) && (RequestLightTargetButtonIndex < ButtonCount) )
        {
            Buttons[RequestLightTargetButtonIndex].Light( false );
        }

        // イベント通知
        Event = EEvent_Pushed;
    }
}






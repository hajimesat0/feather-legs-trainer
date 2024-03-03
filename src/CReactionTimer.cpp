#include "CReactionTimer.h"
#include "CLedButton.h"

CReactionTimer::CReactionTimer( unsigned int button_count )
: ButtonCount(button_count)
, Buttons(new CLedButton[button_count])
{
    WebComm = new CWebComm(this);

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
}


void CReactionTimer::Loop()
{
    WebComm->Loop();
}


void CReactionTimer::OnReset()
{
}


void CReactionTimer::OnLight( unsigned int button_id, bool onoff )
{
    printf("OnLight: button_id=%d, onoff=%d\n", button_id, onoff);

    // 状態遷移

    // 対応するボタンを光らせる
    Buttons[button_id].Light( onoff );
    Buttons[button_id].EnablePushCallback();
    // 時間計測を開始する
}


void CReactionTimer::OnPush()
{
    // ※入力ピン割り込みコンテキストから呼ばれる
    // printf("OnPush: button_id=%d\n", 0);

    // 時間計測を終了する

    // LED消灯
    unsigned int button_index = 0;
    Buttons[button_index].Light( false );
    // Buttons[button_index].DisablePushCallback();


    // 状態更新
}






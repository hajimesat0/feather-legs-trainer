#ifndef __ReactionTimer_h__
#define __ReactionTimer_h__

#include "IRequestListener.h"
#include "IPushListener.h"
#include "CWebComm.h"
#include "CLedButton.h"
#include "EStatus.h"
#include "EEvent.h"

class CReactionTimer : public IRequestListener, public IPushListener
{
public:
    CReactionTimer( unsigned int button_count );
    ~CReactionTimer();

    void Setup();
    void Loop();

    virtual void OnReset();
    virtual void OnLight( unsigned int button_index, bool onoff );
    virtual void OnPush();

private:
    EStatus State;
    EEvent Event;
    CWebComm *WebComm;
    unsigned int ButtonCount;
    CLedButton *Buttons;
    unsigned int RequestLightTargetButtonIndex;
    bool RequestLightOnOff;
    

};

#endif // __ReactionTimer_h__

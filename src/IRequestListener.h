#ifndef __IRequestListener_h__
#define __IRequestListener_h__

class IRequestListener
{
public:
    virtual void OnReset() = 0;
    virtual void OnLight( unsigned int button_id, bool onoff ) = 0;
};

#endif // __IRequestListener_h__

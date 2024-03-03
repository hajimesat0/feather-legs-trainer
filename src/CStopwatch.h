#ifndef __CStopwatch_h__
#define __CStopwatch_h__

class CStopwatch
{
public:
    CStopwatch();
    ~CStopwatch();

    void Start();
    void Stop();
    void Reset();
    unsigned long Elapsed();

private:
    unsigned long StartTime;
    unsigned long StopTime;
    bool Running;
};

#endif // __CStopwatch_h__

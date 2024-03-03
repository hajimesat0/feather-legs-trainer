#ifndef __WebComm_h__
#define __WebComm_h__

#include <ESPAsyncWebServer.h>
#include "IRequestListener.h"

class CWebComm
{
public:
    CWebComm( IRequestListener *request_listener );
    ~CWebComm();

    void Loop();

    void OnWsEvent(
        AsyncWebSocket * server,
        AsyncWebSocketClient * client,
        AwsEventType type,
        void * arg,
        uint8_t *data,
        size_t len
    );
    void SendMeasuringResult( unsigned int button_id, unsigned long time_ms );

private:
    AsyncWebServer *WebServer;
    AsyncWebSocket *WebSocketServer;

    IRequestListener *RequestListener;
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);


};

#endif // __WebComm_h__

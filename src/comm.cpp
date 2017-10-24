#include "comm.h"

#define PAYLOAD_SIZE	32

#ifdef __arm__

#define PIN_IRQ         23
#define PIN_CE          17
#define PIPE_ADDRESS	0xF0F0F0F0E1
#define CHANNEL         0x76

RF24 Comm::radio = RF24(PIN_CE,0);

Comm::Comm()
{
    data = new char[PAYLOAD_SIZE];
    f = 0;

    radio.begin();
    radio.setPayloadSize(PAYLOAD_SIZE);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(CHANNEL);
    radio.setAutoAck(true);
    radio.enableDynamicPayloads();
    radio.enableAckPayload();
    //	radio.printDetails();
    radio.openReadingPipe(1,PIPE_ADDRESS);
    radio.startListening();

    attachInterrupt(PIN_IRQ, INT_EDGE_FALLING, &Comm::intHandler);
}

Comm::~Comm()
{
}

void Comm::intHandler()
{
    radio.read((void*)Comm::Instance().data,PAYLOAD_SIZE);
    if (Comm::Instance().f!=0) Comm::Instance().f();
}

#else

#include <QTimer>

Comm::Comm()
{
    static QTimer timer;

    data = new char[PAYLOAD_SIZE];

    QObject::connect(&timer, &QTimer::timeout, [this](){
        if (f!=0)
        {
            data = {"27.5\0"};
            f();
        }
    });

    timer.start(3000);
}
Comm::~Comm(){}
void Comm::intHandler(){}

#endif

#ifndef COMM_H
#define COMM_H

#include <functional>

#ifdef __arm__
#include <RF24/RF24.h>
#endif

class Comm
{
public:
    typedef std::function<void(void)>	TCbFunction;

    static Comm & Instance()
    {
	static Comm instance;
	return instance;
    }

    TCbFunction f;
    const char * data;

private:
    Comm();
    ~Comm();
    static void intHandler();

#ifdef __arm__
    static RF24 radio;
#else
#endif
};

#endif //COMM_H

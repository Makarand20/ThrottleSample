#ifndef THROTTLE_H
#define THROTTLE_H

#include "ThrottleProfile.h"
#include<list>
#include<thread>
#include "Order.h"

class IRequestHandler {
public:
	IRequestHandler() {};
	virtual ~IRequestHandler() {};

	virtual bool processMessage(Order& request) = 0;
	virtual bool sendMessage(Order& request) = 0;
	virtual bool notify(bool bStart) = 0;
	virtual bool isProcessingAllowed() = 0;
};

class Throttle
{
public:
	Throttle(ThrottleProfile* ptProfile);
	~Throttle();

	void init();
	bool utilizeThrottle();
	void registerRequestHandler(IRequestHandler* pRequestHandler) { m_pRequestHandler = pRequestHandler; }
	void timerThread();

private:
	Throttle(Throttle& handle) = delete;
	Throttle(Throttle&& handle) = delete;
	Throttle& operator=(const Throttle& other) = delete;
	Throttle& operator=(Throttle&& other) = delete;

	ThrottleProfile* m_pTProfile;
	IRequestHandler* m_pRequestHandler;
	std::thread* m_pthread;
	bool m_bTimer;
};
#endif // THROTTLE_H


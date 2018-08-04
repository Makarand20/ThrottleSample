#include "stdafx.h"
#include "Throttle.h"


Throttle::Throttle(ThrottleProfile* ptProfile) :m_pTProfile(ptProfile), m_pRequestHandler(0), m_pthread(0), m_bTimer(false){
}


Throttle::~Throttle()
{
	if (m_pthread) {
		m_bTimer = false;
		m_pthread->join();
		delete m_pthread;
		m_pthread = 0;
	}
}

void Throttle::init() {
	if (!m_pthread) {
		m_pthread = new std::thread(&Throttle::timerThread, this);
		m_bTimer = true;
	}
}

void Throttle::timerThread() {
	while (m_bTimer)
	{
		if (m_pTProfile) {
			if (m_pTProfile->isThrottleExpired()) {
				if (m_pRequestHandler) {
					m_pRequestHandler->notify(true);
				}
			}
		}
		std::this_thread::sleep_for(1ms);// minimum resolution of time is 1 ms
	}
}

bool Throttle::utilizeThrottle() {
	if (m_pTProfile) {
		if( m_pTProfile->utilizeThrottle() ){
			return true;
		}
	}
	return false;
}
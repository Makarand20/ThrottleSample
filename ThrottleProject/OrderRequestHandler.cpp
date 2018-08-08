#include "stdafx.h"
#include "OrderRequestHandler.h"

//================ Utility====================
void printOrder(Order& req) {
	if (req.orderType_ == newOrd) {
		std::cout << req.newOrder.id_ << " " << req.newOrder.symbol_ << " " << req.newOrder.side_ << " " << req.newOrder.size_ << " " << req.newOrder.price_ << std::endl;
	}
	else if (req.orderType_ == amendOrd) {
		std::cout << req.amendOrder.id_ << " " << req.amendOrder.size_ << " " << req.amendOrder.price_ << std::endl;
	}
	else if (req.orderType_ == pullOrd) {
		std::cout << req.pullOrder.id_ << std::endl;
	}
}
//============================================

OrderRequestHandler::OrderRequestHandler(long lCapacity):m_lCapacity(lCapacity), m_bIsProcessingAllowed(true)
{
	m_pThrottleProfile = new ThrottleProfile(10,1000,10);// 100 0rders 1000 milliseconds 10 milli
	m_pThrottle = new Throttle(m_pThrottleProfile);
	m_pThrottle->registerRequestHandler(this);
	m_pThrottle->init();
}


OrderRequestHandler::~OrderRequestHandler()
{
	std::cout << "Destructor called " << std::endl;
	if (m_pThrottle)
		delete m_pThrottle;
	m_pThrottle = 0;

	if (m_pThrottleProfile)
		delete m_pThrottleProfile;
	m_pThrottleProfile = 0;
	// Dump or cleanup messages
}

bool OrderRequestHandler::processPriorityQueue() {
	while (!m_priorityQueue.empty()) {
		Order ord = m_priorityQueue.front();
		if (m_pThrottle) {
			if (m_pThrottle->utilizeThrottle()) {
				sendMessage(ord);// check status and retain or discard order
				m_priorityQueue.pop();
			}
			else
				return false;
		}
	}
	return true;
}

bool OrderRequestHandler::processPendingMessages() {
	if (processPriorityQueue()) {
		while (!m_qOrderBuffer.empty()) {
			Order ord = m_qOrderBuffer.front();
			if (m_pThrottle) {
				if (m_pThrottle->utilizeThrottle()) {
					sendMessage(ord);// check status and retain or discard order
					m_qOrderBuffer.pop();
				}
				else
					return false;
			}
		}
	}
	else
		return false;//All pending cancel requests exhausted throttle

	return true;
}

bool OrderRequestHandler::processMessage(Order& request) {
	if (isProcessingAllowed()) {
		if (processPendingMessages()) {
			if (m_pThrottle) {
				if (m_pThrottle->utilizeThrottle()) {
					sendMessage(request);
					return true;
				}
				else if ((m_qOrderBuffer.size() + m_priorityQueue.size()) < m_lCapacity) {
					if (request.orderType_ == pullOrd) {
						m_priorityQueue.emplace(request);
						return true;
					}else {// Amend or New
						m_qOrderBuffer.emplace(request);
						return true;
					}
				}
				else {
					// Exceeded capacity...
					m_bIsProcessingAllowed = false;
					return false;
				}
			}
		}
		else {
			// Still pending orders in queue but all throttle utilized
			if ((m_qOrderBuffer.size() + m_priorityQueue.size()) < m_lCapacity) {
				if (request.orderType_ == pullOrd) {
					m_priorityQueue.emplace(request);
					return true;
				}else {// Amend or New
					m_qOrderBuffer.emplace(request);
					return true;
				}
			}
		}
	}
	return false;
}

bool OrderRequestHandler::sendMessage(Order & request)
{
	// Send Order to destination
	printOrder(request);
	return false;
}

bool OrderRequestHandler::notify(bool bStart)
{
	m_bIsProcessingAllowed = bStart;
	return m_bIsProcessingAllowed;
}

bool OrderRequestHandler::isProcessingAllowed()
{
	return m_bIsProcessingAllowed;
}

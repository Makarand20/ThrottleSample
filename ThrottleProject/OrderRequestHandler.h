#ifndef ORDER_REQUEST_HANDLER_H
#define ORDER_REQUEST_HANDLER_H

#include "Throttle.h"
#include "Order.h"
#include <queue>

class OrderRequestHandler :
	public IRequestHandler
{
public:
	OrderRequestHandler(long lCapacity);
	~OrderRequestHandler();

	virtual bool isProcessingAllowed();
	virtual bool processMessage(Order& request);
	virtual bool notify(bool bStart);
	bool processPendingMessages();

private:
	OrderRequestHandler(OrderRequestHandler& handle) = delete;
	OrderRequestHandler(OrderRequestHandler&& handle) = delete;
	OrderRequestHandler& operator=(const OrderRequestHandler& other) = delete;
	OrderRequestHandler& operator=(OrderRequestHandler&& other) = delete;

	bool processPriorityQueue();
	virtual bool sendMessage(Order& request);

	unsigned long m_lCapacity;
	std::queue<Order> m_qOrderBuffer;
	bool m_bIsProcessingAllowed;

	ThrottleProfile* m_pThrottleProfile;
	Throttle* m_pThrottle;

	std::queue<Order> m_priorityQueue;
};

#endif // ORDER_REQUEST_HANDLER_H
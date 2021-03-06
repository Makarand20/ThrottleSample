// ThrottleProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OrderRequestHandler.h"
//TODO: exception handling, +ve value validations in Profile
//		removing default functions

int main()
{
	OrderRequestHandler handler(8);
	Order ord;

	ord.orderType_ = newOrd;
	strcpy_s(ord.newOrder.id_,"00001");
	strcpy_s(ord.newOrder.symbol_,"QQQ");
	ord.newOrder.side_ = 'B';
	ord.newOrder.size_ = 10;
	ord.newOrder.price_ = 1000;
// Following section should be part of message handling loop
	for (int i = 0; i < 35; ++i) {
		if (handler.processMessage(ord))
			std::cout << i << " Order processed" << std::endl;
		else
			std::cout << i << " Order Rejected" << std::endl;
	}

	while (true) {
		if (handler.isProcessingAllowed())
			handler.processPendingMessages();

	}

    return 0;
}


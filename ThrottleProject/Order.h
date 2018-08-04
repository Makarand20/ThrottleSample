#ifndef ORDER_H
#define ORDER_H

struct NewOrder
{
	char id_[12];
	char side_; //'B' for Buy 'S' for Sell
	int price_;
	int size_;
	char symbol_[10];
};

struct AmendOrder
{
	char id_[12];
	int price_;
	int size_;
};

struct PullOrder
{
	char id_[12];
};

enum OrdType{
	newOrd,
	amendOrd,
	pullOrd,
};

struct Order {
	OrdType orderType_;
	union {
		NewOrder newOrder;
		AmendOrder amendOrder;
		PullOrder pullOrder;
	};
};

void printOrder(Order& req);

#endif // ORDER_H

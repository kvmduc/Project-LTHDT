#pragma once
#include "../_INCLUDES_.h"
#include "Account.h"

class Buyer : public Account
{
	public:
		Buyer() : Account() {}
		Buyer(const Buyer& b) : Account(b) {}

		void ListOrder_Pending();

		void AddOrder(string _ProductID);
		void AcceptOrder(string _OrderID);
		void RejectOrder(string _OrderID);

		void GetProductByID(string _ProductID);
		void ListProductBySellerID(string _SellerID);
		void ListProductByQuery(string _ProductName);
		
		void Rate(string _OrderID);
};

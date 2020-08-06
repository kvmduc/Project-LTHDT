#pragma once
#include "../_INCLUDES_.h"
#include "Account.h"

class Shipper : public Account
{
	private:
	public:
		Shipper() : Account() {}
		Shipper(const Shipper& h) : Account(h) {}

		void ListOrder_Pending();

		void FinishOrder(string _OrderID);

		void StatsByMonth(int16_t _Year,int8_t _Month);
};

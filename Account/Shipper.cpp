﻿#include "Shipper.h"
#include "../Product/Product.h"
#include "../Order/OrderProvider.h"

void Shipper::ListOrder_Pending()
{
	list<Order> FilteredOrders = OrderProvider::GetInstance().ListByAccountID(this->ID());

	FilteredOrders.remove_if(OrderProvider::isNotPending_Shipper);

	cout << "Total pending order count: " << FilteredOrders.size() << endl;
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		cout << (*i).ID() << " : " << (*i).Status_String() << endl;
	}
}

void Shipper::FinishOrder(string _OrderID)
{
	if (!OrderProvider::isRelated(this->ID(), _OrderID))
	{
		cout << "Order not found." << endl;
		return;
	}

	Order* _Order = OrderProvider::GetInstance().GetByID(_OrderID);
	
	if (_Order->Status() != SHIPPING_PENDING)
	{
		cout << "This order was already completed." << endl;
	}
	_Order->Status(COMPLETED);

	_Order->ShippingDate(Date::Today());

	AccountProvider::GetInstance().GetBuyer(_Order->BuyerID())->Withdraw(_Order->TotalPrice());

	AccountProvider::GetInstance().GetSeller(_Order->SellerID())->Deposit(_Order->TotalPrice() - _Order->ShippingFee());

	AccountProvider::GetInstance().GetShipper(this->ID())->Deposit(_Order->ShippingFee());

	ProductProvider::GetInstance().GetByID(_Order->ProductID())->RemoveStock(_Order->Quantity());
}

void Shipper::StatsByMonth(int16_t _Year ,int8_t _Month)
{
	if (_Year < 1900 || _Year > Date::Today().Year)
	{
		cout << "Invalid year." << endl;
		return;
	}

	if (_Month < 1 || _Month > 12)
	{
		cout << "Invalid month" << endl;
		return;
	}

	list<Order> FilteredOrders = OrderProvider::GetInstance().ListByAccountID(this->ID());
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		if ((*i).ShippingDate().Month != _Month || (*i).Status() != COMPLETED)
		{
			FilteredOrders.erase(i);
			if (FilteredOrders.size() == 0) break;
			else i = FilteredOrders.begin();
		}
	}

	cout << "Overview for " << Date::Month_String(_Month) << " " << to_string(_Year) << ":" << endl;
	if (FilteredOrders.size() == 0)
	{
		cout << "There are no orders for this month." << endl;
		return;
	}
	cout << endl;

	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		cout << "Order " << (*i).ID() << " : Shipping fee = " << (*i).ShippingFee() << endl;
	}

	// Total $
	int64_t TotalShippingFee = 0;
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		TotalShippingFee += (*i).ShippingFee();
	}
	cout << "Total order count = " << FilteredOrders.size() << endl;
	cout << "Total shipping fee = " << TotalShippingFee << endl;
}

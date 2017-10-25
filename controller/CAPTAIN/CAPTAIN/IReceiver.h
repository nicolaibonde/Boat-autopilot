#pragma once
class IReceiver
{
public:
	virtual ~IReceiver(){};
	virtual void ReceiveToNav() = 0;
	virtual void ReceiveActiveParam() = 0;
};


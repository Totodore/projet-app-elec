#pragma once

class BaseWorker
{
public:
	virtual void init() = 0;
	virtual void loop() = 0;
};
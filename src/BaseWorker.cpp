#ifndef BASE_WORKER_H
#define BASE_WORKER_H

class BaseWorker
{
public:
	virtual void init() = 0;
	virtual void loop() = 0;
};
#endif
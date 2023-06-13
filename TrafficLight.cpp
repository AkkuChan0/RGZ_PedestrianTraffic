#include <random>
#include "TrafficLight.h"


Traffic::Traffic()
{
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = INVALID_HANDLE_VALUE;
}

void Traffic::randomTraffic()
{
	MutexLocker guard(hMutex);
	if (isGreen)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 3);
		int newPedestrians = dis(gen);
		peoples += newPedestrians;
	}
}

void Traffic::addPeople()
{
	MutexLocker guard(hMutex);
	++peoples;
}

void Traffic::deleteTraffic()
{
	MutexLocker guard(hMutex);
	peoples = 0;
}

int Traffic::checkTraffic()
{
	MutexLocker guard(hMutex);
	return peoples;
}

void Traffic::changeGreen(bool status)
{
	MutexLocker guard(hMutex);
	isGreen = status;
}

void Traffic::startWork()
{
	if (hThread != INVALID_HANDLE_VALUE) return;
	hThread = CreateThread(NULL, 0, trafficThread, this, 0, NULL);
}

void Traffic::stopWork()
{
	if (hThread == INVALID_HANDLE_VALUE) return;

	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		//прождали пять минут, не остановился, "приболел" видимо, пристрелим, чтобы не мучался
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;
}


DWORD WINAPI Traffic::trafficThread(LPVOID ptr)
{
	Traffic* _this = (Traffic*)ptr;
	if (!_this) return 1; //забыли нам дать объект, не будем работать

	for (;;)
	{
		_this->randomTraffic();
		Sleep(10000);
	}

	return 0;
}
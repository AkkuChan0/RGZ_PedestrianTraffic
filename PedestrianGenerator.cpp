#include "PedestrianGenerator.h"


Pedestrian::Pedestrian()
{
	hMutex = CreateMutex(NULL, FALSE, NULL);

	timer = 0;

	hThread = INVALID_HANDLE_VALUE;
}

Pedestrian::~Pedestrian()
{
	CloseHandle(hMutex);

	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		TerminateThread(hThread, 1);
	}
	stopWork();
}


void Pedestrian::changePedestrian()
{
	MutexLocker guard(hMutex);
	if (peoples.checkTraffic() >= 3 && isGreen == false)
	{
		isGreen = true;
		peoples.deleteTraffic();
		timer = 20;
		while (timer != 0)
		{
			--timer;
			Sleep(1000);
		}

		isGreen = false;
		timer = 60;
		while (timer != 0)
		{
			--timer;
			Sleep(1000);
		}
	}
}


void Pedestrian::startWork()
{
	if (hThread != INVALID_HANDLE_VALUE) return;
	peoples.startWork();
	hThread = CreateThread(NULL, 0, pedestrianThread, this, 0, NULL);
}


void Pedestrian::stopWork()
{
	if (hThread == INVALID_HANDLE_VALUE) return; //не запущен
	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		//прождали пять минут, не остановился, "приболел" видимо, пристрелим, чтобы не мучался
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	peoples.stopWork();
	hThread = INVALID_HANDLE_VALUE;
}


void Pedestrian::addPeople()
{
	if (!isGreen)
		peoples.addPeople();
}

int Pedestrian::checkTime()
{
	MutexLocker guard(hMutex);
	return timer;
}

int Pedestrian::checkPeople()
{
	MutexLocker guard(hMutex);
	return peoples.checkTraffic();
}

std::string Pedestrian::checkStatus()
{
	MutexLocker guard(hMutex);
	if (isGreen)
		return "Green";
	else
		return "Red";
}

void Pedestrian::changeTime(int time)
{
	MutexLocker guard(hMutex);
	timer = time;
}

void Pedestrian::changeStatus(bool status)
{
	MutexLocker guard(hMutex);
	isGreen = status;
}


DWORD WINAPI Pedestrian::pedestrianThread(LPVOID ptr)
{
	Pedestrian* _this = (Pedestrian*)ptr;

	for (;;)
	{
		if (_this->peoples.checkTraffic() >= 3 && _this->isGreen == false)
		{
			Sleep(1000);
			_this->changeStatus(true);
			_this->peoples.changeGreen(false);
			_this->peoples.deleteTraffic();
			_this->changeTime(20);
			while (_this->timer != 0)
			{
				--_this->timer;
				Sleep(1000);
			}

			_this->changeStatus(false);
			_this->peoples.changeGreen(true);
			_this->changeTime(60);
			while (_this->timer != 0)
			{
				--_this->timer;
				Sleep(1000);
			}
		}
	}

	return 0;
}
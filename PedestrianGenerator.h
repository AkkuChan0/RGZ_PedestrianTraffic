#pragma once

#include <string>
#include "TrafficLight.h"


class Pedestrian
{
	int timer;
	bool isGreen = false;
	Traffic peoples;

	HANDLE hMutex;
	HANDLE hThread;

public:

	Pedestrian();
	~Pedestrian();
	void changePedestrian();
	void addPeople();
	int checkTime();
	int checkPeople();
	void changeTime(int time);
	void changeStatus(bool status);
	std::string checkStatus();

	void startWork();
	void stopWork();

private:

	static DWORD WINAPI pedestrianThread(LPVOID ptr);
};


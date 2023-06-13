#pragma once

#include <windows.h>


class MutexLocker
{
	HANDLE m_Mutex;

public:
	MutexLocker(HANDLE mutex) : m_Mutex(mutex)
	{
		WaitForSingleObject(m_Mutex, INFINITE);
	}

	~MutexLocker()
	{
		ReleaseMutex(m_Mutex);
	}
};

class Traffic
{
	int peoples;
	bool isGreen = true;

	HANDLE hMutex;
	HANDLE hThread;

public:

	Traffic();

	void randomTraffic();
	void addPeople();
	void deleteTraffic();
	int checkTraffic();
	void changeGreen(bool status);

	void startWork();
	void stopWork();

private:

	static DWORD WINAPI trafficThread(LPVOID ptr);
};
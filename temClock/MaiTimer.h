// Aug 14:
// Added DWORD casting, removed 't3' as it seems useless

#pragma once
#include <Windows.h>

class MaiTimer
{
	LARGE_INTEGER t1, t2, freq;
	bool running, paused;
	DWORD elapsed_ms_before_paused;

	DWORD GetElapsedMsTillNow();

public:
	MaiTimer();
	DWORD GetElapsedMs();
	bool IsPaused(); 
	bool IsRunning();
	void Start();
	void Stop();
	void Reset();
	void Restart();
	
	void Pause();
	void Resume();

};


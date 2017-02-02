#pragma once
#include <Windows.h>
#include <limits.h>

typedef __int64		i64_t;
typedef int			i32_t;

class MaiTimer
{
	LARGE_INTEGER t1, t2, freq;
	bool running, paused;
	i64_t elapsed_ms_before_paused;

	i64_t GetElapsedMsTillNow();

public:
	MaiTimer();
	i64_t GetElapsedMs();
	i32_t GetElapsedMs_i32();
	bool IsPaused(); 
	bool IsRunning();
	void Start();
	void Stop();
	void Reset();
	void Restart();
	
	void Pause();
	void Resume();

};


#include "MaiTimer.h"

MaiTimer::MaiTimer()
{
	Reset();
}

bool MaiTimer::IsPaused()
{
	return paused;
}

bool MaiTimer::IsRunning()
{
	return running;
}

i64_t MaiTimer::GetElapsedMsTillNow()
{
	if (t1.QuadPart == 0) { return 0; }
	if (running) { QueryPerformanceCounter(&t2); }
	return (t2.QuadPart - t1.QuadPart) * 1000 / freq.QuadPart;
}

i64_t MaiTimer::GetElapsedMs()
{
	i64_t elapsed = elapsed_ms_before_paused;
	if (!paused) { elapsed += GetElapsedMsTillNow(); }
	return elapsed;
}

i32_t MaiTimer::GetElapsedMs_i32()
{
	i64_t ret = GetElapsedMs();
	return ret > INT_MAX ? -1 : (i32_t)ret;
}

void MaiTimer::Start()
{
	if (!running) { Restart(); }
}

void MaiTimer::Stop()
{
	running = false;
	QueryPerformanceCounter(&t2);
}

void MaiTimer::Restart()
{
	Reset();
	running = true;
	QueryPerformanceCounter(&t1);
}

void MaiTimer::Reset()
{
	ZeroMemory(&t1, sizeof(t1));
	ZeroMemory(&t2, sizeof(t2));
	ZeroMemory(&freq, sizeof(freq));
	QueryPerformanceFrequency(&freq);
	running = false;
	paused = false;
	elapsed_ms_before_paused = 0;
}

void MaiTimer::Pause()
{
	if (!paused)
	{
		paused = true;
		elapsed_ms_before_paused += GetElapsedMsTillNow();
		ZeroMemory(&t1, sizeof(t1));
		ZeroMemory(&t2, sizeof(t2));
	}
}

void MaiTimer::Resume()
{
	if (paused)
	{
		paused = false;
		QueryPerformanceCounter(&t1);
	}
}

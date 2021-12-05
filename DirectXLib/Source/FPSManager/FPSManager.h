#pragma once
#include <Windows.h>

namespace lib {
	class FPSManager {
	private:
		bool m_Is_use;
		float m_Min_frame = 1.0f / 60;
		float m_Frame = 0;
		float m_Fps = 0;

		LARGE_INTEGER m_Time_start;
		LARGE_INTEGER m_Time_end;
		LARGE_INTEGER m_Time_freq;
	public:
		FPSManager(int fps);
		void update();
		void draw();
		void drawLog();
	};
}
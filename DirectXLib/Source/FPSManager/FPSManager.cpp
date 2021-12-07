#include "FPSManager.h"
#include <iostream>
#include <sstream>
#pragma warning(disable : 4996)
namespace lib {
	FPSManager::FPSManager(int fps) {
		m_Min_frame = 1.0f / fps;
		m_Frame = 0;
		m_Fps = 0;
		m_Is_use = true;
		if (QueryPerformanceFrequency(&m_Time_freq) == FALSE)m_Is_use = false;
		if (m_Is_use) {
			QueryPerformanceCounter(&m_Time_start);
			//QueryPerformanceCounter(&m_Time_freq);
		}
	}
	void FPSManager::update() {
		if (!m_Is_use)return;
		// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
		QueryPerformanceCounter(&m_Time_end);
		m_Frame = static_cast<float>(m_Time_end.QuadPart - m_Time_start.QuadPart) / static_cast<float>(m_Time_freq.QuadPart);
		if (m_Frame < m_Min_frame) { // 時間に余裕がある
			DWORD sleepTime = static_cast<DWORD>((m_Min_frame - m_Frame) * 1000);//ミリ秒に変換
			timeBeginPeriod(1);//分解能を上げる
			Sleep(sleepTime); //寝る
			timeEndPeriod(1); //戻す
			return;
		}
		if (m_Frame > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
			m_Fps = (m_Fps * 0.99f) + (0.01f / m_Frame); // 平均fpsを計算
		}
		m_Time_start = m_Time_end; // 入れ替え
	}
	void FPSManager::draw() {
		if (!m_Is_use)return;
	}
	void FPSManager::drawLog() {
		if (!m_Is_use)return;
		std::stringstream stream;
		stream << m_Fps << " FPS" << std::endl;
		char* fp = new char[40];
		sprintf(fp, "\nFPS: %lf", m_Fps);
		// カウンタ付けて10回に1回出力、とかにしないと見づらいかもね
		OutputDebugString(fp);
	}
}
#pragma once
#include "../FCT/headers.h"
using namespace FCT;
class App
{
public:
	App() {

	}
	~App() {
		m_abcFont->release();
		m_cnFont->release();
		m_emjFont->release();
		delete m_il;
		m_wnd->release();
		delete m_tp;
		m_ctx->release();
	}
	void init() {

		m_abcFont = m_rt.createFont();
		m_cnFont = m_rt.createFont();
		m_emjFont = m_rt.createFont();
		m_abcFont->create("C:\\Windows\\Fonts\\times.ttf");
		m_cnFont->create("C:\\Windows\\Fonts\\simsun.ttc");

		m_emjFont->create("C://Windows//Fonts//seguiemj.ttf");
		m_abcFont->translateAlpha();
		m_abcFont->translateNums();
		m_cnFont->translateChinese();
		m_cnFont->translateCommonChineseSymbols();
		m_emjFont->translagtEmoji();

		m_il = m_rt.createImageLoader();
		m_wnd = m_rt.createWindow(800, 600, "MaBlock");
		m_ctx = m_rt.createContext(m_wnd);

		m_tp = new TextPipeline(m_ctx);
		//m_tp->addFont(m_abcFont);
		m_tp->addFont(m_cnFont);
		m_tp->addFont(m_emjFont);

		reviewport();

		m_wnd->getCallBack()->addResizeCallback([this](FCT::Window* wnd, int w, int h)
			{ m_needViewPort = true; });

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void run() {
		while (m_wnd->isRunning()) {
			if (m_needViewPort)
				reviewport();
			logicTick();
			renderTick();
			AutoTicker("frame");
		}
	}
	void logicTick() {
		AutoTimer timer("logicTick");
		std::string fpsText = "FPS:" + std::to_string(static_cast<int>(GetTps("frame")));
		std::string logicTickDurationText = "logicTickDuration:" + std::to_string(GetDuration("logicTick") * 1000);
		std::string renderTickDurationText = "renderTickDuration:" + std::to_string(GetDuration("renderTick") * 1000);
		m_tp->drawText(renderTickDurationText, 0, 40);
		m_tp->drawText(logicTickDurationText, 0, 20);
		m_tp->drawText(fpsText, 0, 0);
	}
	void renderTick() {
		AutoTimer timer("renderTick");
		renderDebug();
	}
	void reviewport()
	{
		float x = 0, y = 0, w = m_wnd->getWidth(), h = m_wnd->getHeight();
		if (h / w > 0.75)
		{
			x = 0;
			y = (h - w * 3 / 4) / 2;
			h = w * 3 / 4;
		}
		else
		{
			x = (w - h * 4 / 3) / 2;
			y = 0;
			w = h * 4 / 3;
		}
		m_ctx->viewport(x, y, w, h);
	}
	void renderDebug() {
		m_ctx->clear(0, 0, 0);
		m_tp->flush();
		m_wnd->swapBuffers();
	}
private:
	Runtime m_rt;
	Font* m_abcFont;
	Font* m_cnFont;
	Font* m_emjFont;
	Window* m_wnd;
	Context* m_ctx;
	ImageLoader* m_il;
	TimeCounter m_logicCounter;
	TimeCounter m_renderCounter;
	TextPipeline* m_tp;
	bool m_needViewPort;
};


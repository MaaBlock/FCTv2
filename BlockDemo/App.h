﻿#pragma once
#include "../FCT/headers.h"
#include "Chunk.h"
#include "World.h"
#include "Camera.h"
#include "Player.h"
using namespace FCT;

class SimulationFilterCallback : public physx::PxSimulationFilterCallback
{
public:
	physx::PxFilterFlags pairFound(physx::PxU64 pairID,
								   physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
								   const physx::PxActor *a0, const physx::PxShape *s0,
								   physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
								   const physx::PxActor *a1, const physx::PxShape *s1,
								   physx::PxPairFlags &pairFlags) override
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		return physx::PxFilterFlags();
	}

	void pairLost(physx::PxU64 pairID,
				  physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
				  physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
				  bool objectRemoved) {}

	bool statusChange(physx::PxU64 &pairID, physx::PxPairFlags &pairFlags, physx::PxFilterFlags &filterFlags)
	{
		return false;
	}
};
class App
{
private:
	Runtime m_rt;
	Font *m_abcFont;
	Font *m_cnFont;
	Font *m_emjFont;
	Window *m_wnd;
	Context *m_ctx;
	ImageLoader *m_il;
	TimeCounter m_logicCounter;
	TimeCounter m_renderCounter;
	TextPipeline *m_tp;
	Pipeline *m_pipeline;
	VertexFactory *m_vf;
	Player *m_player;
	World *m_world;
	Camera *m_camera;
	PhysicsSystem *m_phySys;
	physx::PxScene *m_scene;
	bool m_needViewPort;
	char m_keyState[512];
	bool m_mouseCanMove = false;

	bool m_leftMousePressed = false;
	bool m_rightMousePressed = false;
	int m_mouseDx = 0, m_mouseDy = 0;
	SimulationCallback m_callback;
	SimulationFilterCallback *m_filterCallback;

public:
	App()
	{
	}
	~App()
	{
		m_abcFont->release();
		m_cnFont->release();
		m_emjFont->release();
		delete m_il;
		m_wnd->release();
		delete m_tp;
		m_ctx->release();
	}
	float m_groundCheckTimer = 0.0f;
	void init()
	{

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
		m_tp->addFont(m_cnFont);
		m_tp->addFont(m_emjFont);

		m_vf = new VertexFactory();
		m_vf->addAttribute(PipelineAttributeType::Position3f, "position");
		m_vf->addAttribute(PipelineAttributeType::Color4f, "color");
		m_vf->addAttribute(FCT::PipelineAttributeType::TexCoord2f, "TexCoord");
		m_pipeline = new Pipeline(m_ctx, m_vf);
		reviewport();

		Block::Init(m_ctx, m_vf, m_il);
		m_wnd->getCallBack()->addResizeCallback([this](FCT::Window *wnd, int w, int h)
												{ m_needViewPort = true; });

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_phySys = m_rt.createPhysicsSystem();
		m_filterCallback = new SimulationFilterCallback;
		m_scene = m_phySys->createBasicScene(m_filterCallback);
		m_world = new World(m_pipeline, m_phySys, m_scene);
		m_camera = new Camera(FCT::Vec3(0.0f, 0.5f + 1.6f + 3, 0.0f));
		m_wnd->getCallBack()->addKeyDownCallback([this](FCT::Window *wnd, int key)
												 { m_keyState[key] = true; });
		m_wnd->getCallBack()->addKeyUpCallback([this](FCT::Window *wnd, int key)
											   {
				if (key == FCT::KC_ESCAPE)
					m_mouseCanMove = !m_mouseCanMove;
				m_keyState[key] = false; });

		m_wnd->getCallBack()->addMouseMoveCallback([this](FCT::Window *wnd, int x, int y)
												   {
				if (!m_mouseCanMove) {
					m_mouseDx = x - wnd->getWidth() / 2;
					m_mouseDy = wnd->getHeight() / 2 - y;
					wnd->setCursorPos(wnd->getWidth() / 2, wnd->getHeight() / 2);
				} });

		m_wnd->getCallBack()->addRButtonUpCallback([this](Window *wnd, int x, int y)
												   { m_rightMousePressed = true; });
		m_wnd->getCallBack()->addLButtonUpCallback([this](Window *wnd, int x, int y)
												   { m_leftMousePressed = true; });

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		m_camera->addPipeline(m_pipeline);
		m_player = new Player(m_world, m_camera, m_phySys, m_scene);
		m_callback.bindToScene(m_scene);
		m_callback.addContactCallback([this](const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
									  {
    bool playerContact = false;
    bool groundContact = false;

    for (physx::PxU32 i = 0; i < nbPairs; i++)
    {
        const physx::PxContactPair& cp = pairs[i];
        if (cp.contactCount > 0)
        {
            std::vector<physx::PxContactPairPoint> contacts(cp.contactCount);
            cp.extractContacts(&contacts[0], cp.contactCount);

            for (physx::PxU32 j = 0; j < cp.contactCount; j++)
            {
                const physx::PxContactPairPoint& contact = contacts[j];
                physx::PxVec3 normal = contact.normal;

                if (pairHeader.actors[0] == m_player->getActor() || pairHeader.actors[1] == m_player->getActor())
                {
                    playerContact = true;
                    if (normal.y > 0.5f)
                    {
                        groundContact = true;
                        break;
                    }
                }
            }
        }
    }
    if (groundContact)
    {
        m_player->setOnGround(true);
    } });
		m_scene->simulate(0);
	}
	void run()
	{
		while (m_wnd->isRunning())
		{
			AutoTicker("frame");
			if (m_needViewPort)
				reviewport();
			logicTick();
			renderTick();
		}
	}
	void logicTick()
	{
		float deltaTime = GetTickDuration("frame");
		m_scene->fetchResults(true);
		AutoTimer timer("logicTick");
		std::string fpsText = "FPS:" + std::to_string(static_cast<int>(GetTps("frame")));
		std::string logicTickDurationText = "logicTickDuration:" + std::to_string(GetDuration("logicTick") * 1000);
		std::string renderTickDurationText = "renderTickDuration:" + std::to_string(GetDuration("renderTick") * 1000);
		std::string frameDurationText = "frameDuration:" + std::to_string(GetTickDuration("frame") * 1000);
		std::string posText = "currentPos:(" + std::to_string(m_camera->position.x) + "," + std::to_string(m_camera->position.y) + "," + std::to_string(m_camera->position.z) + ")";

		m_tp->drawText(posText, 0, 80);
		m_tp->drawText(frameDurationText, 0, 60);
		m_tp->drawText(renderTickDurationText, 0, 40);
		m_tp->drawText(logicTickDurationText, 0, 20);
		m_tp->drawText(fpsText, 0, 0);
		Vec3 rayDirection = m_camera->getRayDirection();
		Vec3 normal;
		std::pair<bool, Vec3> ret = m_world->raycast(m_camera->position, rayDirection, 5.0f);
		ret.first = m_world->MousePickFace(m_camera->position, rayDirection, ret.second, normal);
		auto hit = ret.first;
		auto hitPosition = m_world->GetBlockPositionFromHit(ret.second, normal);
		if (hit)
		{
			m_world->selectBlock(hitPosition, normal);
			if (m_leftMousePressed)
			{
				m_world->destroyBlock(hitPosition);
				m_leftMousePressed = false;
			}
			if (m_rightMousePressed)
			{
				//Vec3 normal = m_world->getNormal(hitPosition, m_camera->position);
				m_world->placeBlock(m_world->GetAdjacentBlockPosition(ret.second,normal));
				m_rightMousePressed = false;
			}
		}
		else {
			m_world->unselect();
		}
		m_player->processKeyboard(m_keyState, deltaTime, *m_world);
		m_player->processMouseMovement(m_mouseDx, m_mouseDy);
		/*
		m_camera->processKeyboard(m_keyState, deltaTime, *m_world);
		m_camera->processMouseMovement(m_mouseDx, m_mouseDy);*/
		m_mouseDx = 0;
		m_mouseDy = 0;
		m_camera->updata();
		m_world->updata();
		m_scene->simulate(deltaTime);
	}
	void renderTick()
	{
		AutoTimer timer("renderTick");
		m_ctx->clear(0.2f, 0.3f, 0.3f);
		m_pipeline->begin();
		m_world->render(m_pipeline, m_camera->position);
		m_pipeline->end();
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
	void renderDebug()
	{
		m_tp->flush();
		m_wnd->swapBuffers();
	}
};

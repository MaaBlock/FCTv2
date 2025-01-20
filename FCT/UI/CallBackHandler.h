#pragma once
#include "./EventHandler.h"
#include <vector>
#include <functional>
#include <unordered_map>

namespace FCT {
    class CallBackEventHandler : public EventHandler {
    public:
        using ResizeCallBack = std::function<void(Window* wnd, int width, int height)>;
        using CallbackId = size_t;

        void onResize(Window* wnd, int width, int height) override {
            for (const auto& pair : m_resizeCallbacks) {
                pair.second(wnd, width, height);
            }
        }

        CallbackId addResizeCallback(const ResizeCallBack& cb) {
            CallbackId id = m_nextId++;
            m_resizeCallbacks[id] = cb;
            return id;
        }

        void removeResizeCallback(CallbackId id) {
            m_resizeCallbacks.erase(id);
        }

        void clearResizeCallbacks() {
            m_resizeCallbacks.clear();
        }

        void invokeResizeCallbacks(Window* wnd, int width, int height) {
            onResize(wnd, width, height);
        }

    private:
        std::unordered_map<CallbackId, ResizeCallBack> m_resizeCallbacks;
        CallbackId m_nextId = 0;
    };
}
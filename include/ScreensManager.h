#pragma once

#include "GameObject.h"

class ScreensManager: public GameObject
{
    public:
        void SwapScreens();
        void Update(FixedPoint dt) override;

    private:
        ScreensManager(std::string name);

        enum class State { Idle, FadingOut, FadingIn };

        State m_State = State::Idle;
        int m_Brightness = 0;
        bool m_MainOnTop = true;

        friend class GameObject;
};
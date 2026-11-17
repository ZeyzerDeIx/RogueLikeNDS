#include "ScreensManager.h"
#include "pch.h"

ScreensManager::ScreensManager(std::string name): GameObject(std::move(name)) {}

void ScreensManager::SwapScreens()
{
    if (m_State != State::Idle) return;

    m_State = State::FadingOut;
    m_Brightness = 0;
    m_MainOnTop = !m_MainOnTop;
}

void ScreensManager::Update(FixedPoint dt)
{
    if (m_State == State::Idle) return;

    if (m_State == State::FadingOut)
    {
        setBrightness(3, --m_Brightness);

        if (m_Brightness <= -16)
        {
            lcdSwap();
            m_State = State::FadingIn;
        }
    }
    else if (m_State == State::FadingIn)
    {
        setBrightness(3, ++m_Brightness);

        if (m_Brightness >= 0)
        {
            m_Brightness = 0;
            m_State = State::Idle;
        }
    }
}
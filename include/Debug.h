#pragma once
class Entity;

namespace Debug
{
    void init();
    void clearConsole();
    void logFps();
    void logEntityInfo(Entity& entity);
    void beginProfile();
    void endProfile();
    void logProfile();
}
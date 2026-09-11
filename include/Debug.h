#pragma once
class Entity;

namespace Debug
{
    void Init();
    void ClearConsole();
    void LogFps();
    void LogEntityInfo(Entity& entity);
    void BeginProfile();
    void EndProfile();
    void LogProfile();
}
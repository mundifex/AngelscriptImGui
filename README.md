AngelscriptImGui
================
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.md)

Requires [ImGui](https://github.com/benui-dev/UnrealImGui) plugin

Example Usage
-------------
Within a CheatManager script subclass:
```cpp
class UMyCheatManager : UCheatManager
{
    UFUNCTION(BlueprintOverride)
    void InitCheatManager()
    {
        ImGui::AddWorldEarlyDebugDelegate(this, n"OnImGui");
    }

    UFUNCTION()
    void OnImGui()
    {
      // only show if ImGui is capturing or sharing input
      if (ImGuiProps::IsInputEnabled() == false && ImGuiProps::IsKeyboardInputShared() == false)
      {
        return;
      }
      
      ImGui::BeginMainMenuBar();
      // ...
      ImGui::EndMainMenuBar();
    }
}
```

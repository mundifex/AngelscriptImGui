AngelscriptImGui
================
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.md)

Requires [ImGui](https://github.com/benui-dev/UnrealImGui) plugin

Provides script namespace `ImGui` containing Angeslcript binds to native ImGui functions, e.g. `ImGui::Button("Click Me")`

Example Usage
-------------
Create file `DefaultImGui.ini` in project's Config folder with desired keybinds:
```
[/Script/ImGui.ImGuiSettings]
ToggleInput=(Key=F10)
```

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
        // only show if ImGui is capturing input
        if (ImGuiProps::IsInputEnabled() == false)
        {
            return;
        }
        
        ImGui::BeginMainMenuBar();
        // ...
        ImGui::EndMainMenuBar();
    }
}
```

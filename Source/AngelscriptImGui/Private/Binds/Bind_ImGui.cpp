// Copyright 2024 Ben Donatelli

#include "AngelscriptBinds.h"
#include "AngelscriptManager.h"

#include "ImGuiModule.h"
#include "ImGuiCommon.h"

// NOTE: all enums are int8 in AS so we can't bind full range of values in ImGuiWindowFlags within native
//  instead, we use integer type for Flags parameters and mirror desired enum values in script namespaces
// https://discord.com/channels/551756549962465299/551756549962465303/1179730807435296880

const FAngelscriptBinds::FBind Bind_ImGui(FAngelscriptBinds::EOrder::Normal, []
{
	FAngelscriptBinds::FNamespace ns("ImGui");

	FAngelscriptBinds::BindGlobalFunction("void AddWorldEarlyDebugDelegate(UObject Object, FName Function)",
	[](UObject* Object, FName Function)
	{
#if !UE_BUILD_SHIPPING
		if (Object->FindFunction(Function) == nullptr)
		{
			FAngelscriptManager::Throw("Could not find function in object with this name. Is it declared UFUNCTION()?");
			return;
		}
#endif // !UE_BUILD_SHIPPING

		FImGuiDelegates::OnWorldEarlyDebug().AddUFunction(Object, Function);
	});

	// Commands
	FAngelscriptBinds::BindGlobalFunction("void ToggleKeyboardInputSharing()",
	[]()
	{
		FImGuiModuleProperties& ImGuiProperties = FImGuiModule::Get().GetProperties();
		ImGuiProperties.ToggleKeyboardInputSharing();
	});

	// Windows
	FAngelscriptBinds::BindGlobalFunction("bool Begin(const FString& Name)",
	[](const FString& Name)
	{
		return ImGui::Begin(TCHAR_TO_ANSI(*Name));
	});
	FAngelscriptBinds::BindGlobalFunction("bool Begin(const FString& Name, bool& bOpen, int Flags = 0)",
	[](const FString& Name, bool& bOpen, int Flags = 0)
	{
		return ImGui::Begin(TCHAR_TO_ANSI(*Name), &bOpen, Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("void End()", FUNC_TRIVIAL(ImGui::End));

	// Child Windows
	FAngelscriptBinds::BindGlobalFunction("void BeginChild(const FString& Id)",
	[](const FString& Id)
	{
		return ImGui::BeginChild(TCHAR_TO_ANSI(*Id));
	});
	FAngelscriptBinds::BindGlobalFunction("void EndChild()", FUNC_TRIVIAL(ImGui::EndChild));

	// Cursor / Layout
	FAngelscriptBinds::BindGlobalFunction("void Separator()", FUNC_TRIVIAL(ImGui::Separator));
	FAngelscriptBinds::BindGlobalFunction("void SameLine()",
	[]()
	{
		// can't use FUNC_TRIVIAL because it passes junk values as function arguments
		ImGui::SameLine();
	});
	FAngelscriptBinds::BindGlobalFunction("void NewLine()", FUNC_TRIVIAL(ImGui::NewLine));

	// ID stack/scopes
	FAngelscriptBinds::BindGlobalFunction("void PushID(const FString& Id)",
	[](const FString& Id)
	{
		ImGui::PushID(TCHAR_TO_ANSI(*Id));
	});
	FAngelscriptBinds::BindGlobalFunction("void PushID(int Id)",
	[](int Id)
	{
		ImGui::PushID(Id);
	});
	FAngelscriptBinds::BindGlobalFunction("void PopID()", FUNC_TRIVIAL(ImGui::PopID));

	// Widgets: Text
	FAngelscriptBinds::BindGlobalFunction("void Text(const FString& Text)",
	[](const FString& Text)
	{
		ImGui::Text(TCHAR_TO_ANSI(*Text));
	});
	FAngelscriptBinds::BindGlobalFunction("void TextColored(const FLinearColor& Color, const FString& Text)",
	[](const FLinearColor& Color, const FString& Text)
	{
		const ImVec4 ImColor = ImVec4(Color.R, Color.G, Color.B, Color.A);
		ImGui::TextColored(ImColor, TCHAR_TO_ANSI(*Text));
	});

	// Widgets: Main
	FAngelscriptBinds::BindGlobalFunction("bool Button(const FString& Label, const FVector2D& Size = FVector2D())",
	[](const FString& Label, const FVector2D& Size = FVector2D())
	{
		return ImGui::Button(TCHAR_TO_ANSI(*Label), ImVec2(Size.X, Size.Y));
	});
	FAngelscriptBinds::BindGlobalFunction("bool Checkbox(const FString& Label, bool& bIsChecked)",
	[](const FString& Label, bool& bIsChecked)
	{
		return ImGui::Checkbox(TCHAR_TO_ANSI(*Label), &bIsChecked);
	});

	// Widgets: Combo Box
	FAngelscriptBinds::BindGlobalFunction("bool BeginCombo(const FString& Label, const FString& PreviewValue, int Flags = 0)",
	[](const FString& Label, const FString& PreviewValue, int Flags = 0)
	{
		return ImGui::BeginCombo(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(*PreviewValue), Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("void EndCombo()", FUNC_TRIVIAL(ImGui::EndCombo));

	// Widgets: Input with Keyboard
	FAngelscriptBinds::BindGlobalFunction("bool InputText(const FString& Label, FString& Text, int Flags = 0)",
	[](FString& Label, FString& Text, int Flags = 0)
	{
		const size_t BufferSize = 128;
		ANSICHAR AnsiBuffer[BufferSize];
		FCStringAnsi::Strcpy(AnsiBuffer, BufferSize, TCHAR_TO_ANSI(*Text));

		if (ImGui::InputText(TCHAR_TO_ANSI(*Label), AnsiBuffer, BufferSize, Flags))
		{
			Text = ANSI_TO_TCHAR(AnsiBuffer);
			return true;
		}
		return false;
	});
	FAngelscriptBinds::BindGlobalFunction("bool InputInt(const FString& Label, int& Value, int Step = 1, int StepFast = 100, int Flags = 0)",
	[](const FString& Label, int& Value, int Step, int StepFast, int Flags)
	{
		return ImGui::InputInt(TCHAR_TO_ANSI(*Label), &Value, Step, StepFast, Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("bool InputFloat(const FString& Label, float32& Value, float32 Step = 0.f, float32 StepFast = 0.f, const FString& Format = \"%.3f\", int Flags = 0)",
	[](const FString& Label, float& Value, float Step, float StepFast, const FString& Format, int Flags)
	{
		return ImGui::InputFloat(TCHAR_TO_ANSI(*Label), &Value, Step, StepFast, TCHAR_TO_ANSI(*Format), Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("bool InputFloat(const FString& Label, double& Value, double Step = 0.0, double StepFast = 0.0, const FString& Format = \"%.6f\", int Flags = 0)",
	[](const FString& Label, double& Value, double Step, double StepFast, const FString& Format, int Flags)
	{
		return ImGui::InputDouble(TCHAR_TO_ANSI(*Label), &Value, Step, StepFast, TCHAR_TO_ANSI(*Format), Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("bool InputDouble(const FString& Label, double& Value, double Step = 0.0, double StepFast = 0.0, const FString& Format = \"%.6f\", int Flags = 0)",
	[](const FString& Label, double& Value, double Step, double StepFast, const FString& Format, int Flags)
	{
		return ImGui::InputDouble(TCHAR_TO_ANSI(*Label), &Value, Step, StepFast, TCHAR_TO_ANSI(*Format), Flags);
	});

	// Widgets: Color Editor/Picker
	FAngelscriptBinds::BindGlobalFunction("bool ColorEdit4(const FString& Label, FLinearColor& Color, int Flags = 0)",
	[](const FString& Label, FLinearColor& Color, int Flags = 0)
	{
		float ImColor[4] = { Color.R, Color.G, Color.B, Color.A };
		if (ImGui::ColorEdit4(TCHAR_TO_ANSI(*Label), ImColor, Flags))
		{
			Color = FLinearColor(ImColor[0], ImColor[1], ImColor[2], ImColor[3]);
			return true;
		}
		return false;
	});

	// Widgets: Selectables
	FAngelscriptBinds::BindGlobalFunction("bool Selectable(const FString& Label, bool& bSelected, int Flags = 0)",
	[](const FString& Label, bool& bSelected, int Flags = 0)
	{
		return ImGui::Selectable(TCHAR_TO_ANSI(*Label), &bSelected, Flags);
	});

	// Widgets: Data Plotting
	FAngelscriptBinds::BindGlobalFunction("void PlotLines(const FString& Label, const TArray<float32>& Values)",
	[](const FString& Label, const TArray<float>& Values)
	{
		ImGui::PlotLines(TCHAR_TO_ANSI(*Label), Values.GetData(), Values.Num());
	});

	// Widgets: Menus
	FAngelscriptBinds::BindGlobalFunction("bool BeginMenuBar()", FUNC_TRIVIAL(ImGui::BeginMenuBar));
	FAngelscriptBinds::BindGlobalFunction("void EndMenuBar()", FUNC_TRIVIAL(ImGui::EndMenuBar));
	FAngelscriptBinds::BindGlobalFunction("bool BeginMainMenuBar()", FUNC_TRIVIAL(ImGui::BeginMainMenuBar));
	FAngelscriptBinds::BindGlobalFunction("void EndMainMenuBar()", FUNC_TRIVIAL(ImGui::EndMainMenuBar));
	FAngelscriptBinds::BindGlobalFunction("bool BeginMenu(const FString& Label, bool bEnabled = true)",
	[](const FString& Label, bool bEnabled = true)
	{
		return ImGui::BeginMenu(TCHAR_TO_ANSI(*Label), bEnabled);
	});
	FAngelscriptBinds::BindGlobalFunction("void EndMenu()", FUNC_TRIVIAL(ImGui::EndMenu));
	FAngelscriptBinds::BindGlobalFunction("bool MenuItem(const FString& Label)",
	[](const FString& Label)
	{
		return ImGui::MenuItem(TCHAR_TO_ANSI(*Label));
	});

	// Tables
	FAngelscriptBinds::BindGlobalFunction("bool BeginTable(const FString& Id, int Column, int Flags = 0)",
	[](const FString& Id, int Column, int Flags = 0)
	{
		return ImGui::BeginTable(TCHAR_TO_ANSI(*Id), Column, Flags);
	});
	FAngelscriptBinds::BindGlobalFunction("void EndTable()", FUNC_TRIVIAL(ImGui::EndTable));
	FAngelscriptBinds::BindGlobalFunction("void TableNextColumn()", FUNC_TRIVIAL(ImGui::TableNextColumn));

	// Tables: Headers & Columns
	FAngelscriptBinds::BindGlobalFunction("void TableSetupColumn(const FString& Label, int Flags = 0, float32 InitWidthOrHeight = 0.f)",
	[](const FString& Label, int Flags, float InitWidthOrHeight)
	{
		ImGui::TableSetupColumn(TCHAR_TO_ANSI(*Label), Flags, InitWidthOrHeight);
	});
	FAngelscriptBinds::BindGlobalFunction("void TableHeadersRow()", FUNC_TRIVIAL(ImGui::TableHeadersRow));
});

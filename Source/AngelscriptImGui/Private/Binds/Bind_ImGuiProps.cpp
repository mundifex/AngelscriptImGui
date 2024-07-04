// Copyright 2024 Ben Donatelli

#include "AngelscriptBinds.h"

#include "ImGuiModule.h"

const FAngelscriptBinds::FBind Bind_ImGuiProps(FAngelscriptBinds::EOrder::Normal, []
{
	FAngelscriptBinds::FNamespace ns("ImGuiProps");

	FAngelscriptBinds::BindGlobalFunction("bool IsInputEnabled()",
	[]()
	{
		const FImGuiModuleProperties& ImGuiProps = FImGuiModule::Get().GetProperties();
		return ImGuiProps.IsInputEnabled();
	});

	FAngelscriptBinds::BindGlobalFunction("bool IsKeyboardInputShared()",
	[]()
	{
		const FImGuiModuleProperties& ImGuiProps = FImGuiModule::Get().GetProperties();
		return ImGuiProps.IsKeyboardInputShared();
	});
});

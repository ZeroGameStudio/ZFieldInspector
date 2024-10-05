// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZFieldInspectorEditorModule : public IModuleInterface
{
public:
	static FORCEINLINE IZFieldInspectorEditorModule& Get()
	{
		static IZFieldInspectorEditorModule& GSingleton = FModuleManager::LoadModuleChecked<IZFieldInspectorEditorModule>("ZFieldInspectorEditor");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZFieldInspectorEditor");
	}
};

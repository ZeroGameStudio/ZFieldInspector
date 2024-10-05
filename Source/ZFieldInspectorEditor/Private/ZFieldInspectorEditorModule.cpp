// Copyright Zero Games. All Rights Reserved.

#include "ZFieldInspectorEditorModule.h"

#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Framework/Docking/WorkspaceItem.h"

#define LOCTEXT_NAMESPACE "ZFieldInspectorEditorModule"

namespace ZFieldInspector::ZFieldInspectorEditorModule_Private
{
	static const FName TabId1 { "FieldInspector1" };
	static const FName TabId2 { "FieldInspector2" };
}

class FZFieldInspectorEditorModule : public IZFieldInspectorEditorModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<FWorkspaceItem> FieldInspectorsGroup;
};

IMPLEMENT_MODULE(FZFieldInspectorEditorModule, ZFieldInspectorEditor)

void FZFieldInspectorEditorModule::StartupModule()
{
	const TSharedRef<FWorkspaceItem> debugCategory = WorkspaceMenu::GetMenuStructure().GetDeveloperToolsDebugCategory();
	const auto icon = FSlateIcon { FAppStyle::GetAppStyleSetName(), "Modules" };
	
	FieldInspectorsGroup = debugCategory->AddGroup(LOCTEXT("GroupName", "Field Inspectors"), icon);
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		ZFieldInspector::ZFieldInspectorEditorModule_Private::TabId1, FOnSpawnTab::CreateRaw(this, &FZFieldInspectorEditorModule::SpawnTab))
		.SetDisplayName(LOCTEXT("TabTitle1", "Field Inspector 1"))
		.SetTooltipText(LOCTEXT("TooltipText1", "Open the field inspector tab."))
		.SetIcon(icon)
		.SetGroup(FieldInspectorsGroup.ToSharedRef());
		
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		ZFieldInspector::ZFieldInspectorEditorModule_Private::TabId2, FOnSpawnTab::CreateRaw(this, &FZFieldInspectorEditorModule::SpawnTab))
		.SetDisplayName(LOCTEXT("TabTitle2", "Field Inspector 2"))
		.SetTooltipText(LOCTEXT("TooltipText2", "Open a second field inspector tab."))
		.SetIcon(icon)
		.SetGroup(FieldInspectorsGroup.ToSharedRef());
}

void FZFieldInspectorEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ZFieldInspector::ZFieldInspectorEditorModule_Private::TabId1);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ZFieldInspector::ZFieldInspectorEditorModule_Private::TabId2);
	
	const TSharedRef<FWorkspaceItem> debugCategory = WorkspaceMenu::GetMenuStructure().GetDeveloperToolsDebugCategory();
	debugCategory->RemoveItem(FieldInspectorsGroup.ToSharedRef());
}

TSharedRef<SDockTab> FZFieldInspectorEditorModule::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return
		SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBorder)
			.Padding(2)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(STextBlock).Text(LOCTEXT("Test", "I'm field inspector!"))
			]
		];
}

#undef LOCTEXT_NAMESPACE



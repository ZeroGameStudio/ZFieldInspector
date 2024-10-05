// Copyright Zero Games. All Rights Reserved.


#include "ZPackageFieldTreeItem.h"

#include "ZUFieldTreeItem.h"

namespace ZFieldInspector::ZPackageFieldTreeItem_Private
{
	int32 GetUFieldPriority(const UField* field)
	{
		if (const auto cls = Cast<UClass>(field))
		{
			return !cls->HasAnyClassFlags(CLASS_Deprecated) ? 1 : MAX_int32;
		}

		if (field->IsA<UScriptStruct>())
		{
			return 2;
		}

		if (field->IsA<UEnum>())
		{
			return 3;
		}

		if (field->IsA<UDelegateFunction>())
		{
			return 4;
		}

		return MAX_int32;
	}
}

ZFieldInspector::FZPackageFieldTreeItem::FZPackageFieldTreeItem(UPackage* package)
	: Package(package)
{

}

ZFieldInspector::IZFieldTreeItem::TargetType ZFieldInspector::FZPackageFieldTreeItem::GetTarget() const
{
	TargetType target;
	target.Set<UPackage*>(Package.Get());
	return target;
}

void ZFieldInspector::FZPackageFieldTreeItem::ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action)
{
	SetupChildren();
	
	for (const auto& child : *Children)
	{
		action(child.ToSharedRef());
	}
}

void ZFieldInspector::FZPackageFieldTreeItem::SetupChildren()
{
	if (Children)
	{
		return;
	}

	Children = TArray<TSharedPtr<IZFieldTreeItem>>{};
	TArray<UObject*> subobjects;
	GetObjectsWithPackage(Package.Get(), subobjects, false);
	for (const auto& subobject : subobjects)
	{
		TSharedPtr<IZFieldTreeItem> child;
		if (const auto field = Cast<UField>(subobject))
		{
			child = MakeShared<FZUFieldTreeItem>(field);
		}

		if (child)
		{
			Children->Emplace(child);
		}
	}

	Children->StableSort([](const TSharedPtr<IZFieldTreeItem>& lhs, const TSharedPtr<IZFieldTreeItem>& rhs)
	{
		const int32 lhsPriority = ZPackageFieldTreeItem_Private::GetUFieldPriority(lhs->GetTarget().Get<UField*>());
		const int32 rhsPriority = ZPackageFieldTreeItem_Private::GetUFieldPriority(rhs->GetTarget().Get<UField*>());
		
		return lhsPriority != rhsPriority ? lhsPriority < rhsPriority : lhs->GetName() < rhs->GetName();
	});
}



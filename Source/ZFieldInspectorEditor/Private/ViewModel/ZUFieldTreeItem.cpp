// Copyright Zero Games. All Rights Reserved.


#include "ZUFieldTreeItem.h"

#include "ZFFieldTreeItem.h"

ZFieldInspector::FZUFieldTreeItem::FZUFieldTreeItem(UField* field)
	: Field(field)
{

}

ZFieldInspector::IZFieldTreeItem::TargetType ZFieldInspector::FZUFieldTreeItem::GetTarget() const
{
	TargetType target;
	target.Set<UField*>(Field.Get());
	return target;
}

void ZFieldInspector::FZUFieldTreeItem::ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action)
{
	SetupChildren();
	
	for (const auto& child : *Children)
	{
		action(child.ToSharedRef());
	}
}

void ZFieldInspector::FZUFieldTreeItem::SetupChildren()
{
	if (Children)
	{
		return;
	}

	Children = TArray<TSharedPtr<IZFieldTreeItem>>{};
	if (const auto strct = Cast<UStruct>(Field))
	{
		{
			UField* currentChild = strct->Children;
			while (currentChild)
			{
				Children->Emplace(MakeShared<FZUFieldTreeItem>(currentChild));
				currentChild = currentChild->Next;
			}

			Children->StableSort([](const TSharedPtr<IZFieldTreeItem>& lhs, const TSharedPtr<IZFieldTreeItem>& rhs)
			{
				const int32 lhsPriority = lhs->GetTarget().Get<UField*>()->IsA<UDelegateFunction>() ? 1 : 2;
				const int32 rhsPriority = rhs->GetTarget().Get<UField*>()->IsA<UDelegateFunction>() ? 1 : 2;
				return lhsPriority < rhsPriority;
			});
		}

		{
			FField* currentChild = strct->ChildProperties;
			while (currentChild)
			{
				Children->Emplace(MakeShared<FZFFieldTreeItem>(currentChild));
				currentChild = currentChild->Next;
			}
		}
	}
}



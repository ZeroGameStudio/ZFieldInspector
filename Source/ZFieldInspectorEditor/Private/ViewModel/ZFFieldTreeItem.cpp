// Copyright Zero Games. All Rights Reserved.


#include "ZFFieldTreeItem.h"

ZFieldInspector::FZFFieldTreeItem::FZFFieldTreeItem(FField* field)
	: GCRoot(field->GetOwnerUField())
	, Field(field)
{
	check(!GCRoot.IsExplicitlyNull());
}

ZFieldInspector::IZFieldTreeItem::TargetType ZFieldInspector::FZFFieldTreeItem::GetTarget() const
{
	TargetType target;
	target.Set<FField*>(GCRoot.IsValid(true) ? Field : nullptr);
	return target;
}

void ZFieldInspector::FZFFieldTreeItem::ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action)
{
}



// Copyright Zero Games. All Rights Reserved.


#include "IZFieldTreeItem.h"


FName ZFieldInspector::IZFieldTreeItem::GetFName() const
{
	return Visit([](const auto& value){ return GetFNameSafe(value); }, GetTarget());
}

FString ZFieldInspector::IZFieldTreeItem::GetName() const
{
	return GetFName().ToString();
}

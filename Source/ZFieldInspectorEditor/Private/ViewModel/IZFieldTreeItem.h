// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZFieldInspector
{
	struct IZFieldTreeItem
	{
		using TargetType = TVariant<UPackage*, UField*, FField*>;
		virtual ~IZFieldTreeItem(){}
		virtual TargetType GetTarget() const = 0;
		virtual void ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action) = 0;

	public:
		FName GetFName() const;
		FString GetName() const;
	};
}



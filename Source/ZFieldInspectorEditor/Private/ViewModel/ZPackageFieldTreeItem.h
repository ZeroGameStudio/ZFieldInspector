// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZFieldTreeItem.h"

namespace ZFieldInspector
{
	struct FZPackageFieldTreeItem : public IZFieldTreeItem
	{
		
		explicit FZPackageFieldTreeItem(UPackage* package);
		
		virtual TargetType GetTarget() const override;
		virtual void ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action) override;

	private:
		void SetupChildren();

	private:
		TWeakObjectPtr<UPackage> Package;
		TOptional<TArray<TSharedPtr<IZFieldTreeItem>>> Children;
		
	};
}



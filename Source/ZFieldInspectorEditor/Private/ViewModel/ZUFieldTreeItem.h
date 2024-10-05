// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZFieldTreeItem.h"

namespace ZFieldInspector
{
	struct FZUFieldTreeItem : public IZFieldTreeItem
	{
		
		explicit FZUFieldTreeItem(UField* field);
		
		virtual TargetType GetTarget() const override;
		virtual void ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action) override;

	private:
		void SetupChildren();

	private:
		TWeakObjectPtr<UField> Field;
		TOptional<TArray<TSharedPtr<IZFieldTreeItem>>> Children;
		
	};
}



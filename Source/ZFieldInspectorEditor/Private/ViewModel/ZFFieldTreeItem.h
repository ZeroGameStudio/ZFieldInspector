// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZFieldTreeItem.h"

namespace ZFieldInspector
{
	struct FZFFieldTreeItem : public IZFieldTreeItem
	{
		
		explicit FZFFieldTreeItem(FField* field);
		
		virtual TargetType GetTarget() const override;
		virtual void ForeachChildren(TFunctionRef<void(TSharedRef<IZFieldTreeItem>)> action) override;

	private:
		TWeakObjectPtr<UField> GCRoot;
		FField* Field;
		
	};
}



// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Widgets/Views/STableRow.h"

namespace ZFieldInspector
{
	struct IZFieldTreeItem;

	class SZFieldTreeRow : public STableRow<TSharedPtr<IZFieldTreeItem>>
	{

		using Super = STableRow;

	public:
		SLATE_BEGIN_ARGS(SZFieldTreeRow){}
			SLATE_ARGUMENT(TSharedPtr<IZFieldTreeItem>, Item)
		SLATE_END_ARGS()

		void Construct(const FArguments& args, TSharedRef<STableViewBase> owner);
		
	};
}



// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Widgets/Views/STreeView.h"

namespace ZFieldInspector
{
	struct IZFieldTreeItem;

	class SZFieldTreeView : public STreeView<TSharedPtr<IZFieldTreeItem>>
	{
		
	};
}



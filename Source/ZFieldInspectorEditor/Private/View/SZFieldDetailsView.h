// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ViewModel/IZFieldTreeItem.h"
#include "Widgets/SUserWidget.h"

namespace ZFieldInspector
{
	class SZFieldDetailsView : public SUserWidget
	{

	public:
		SLATE_USER_ARGS(SZFieldDetailsView){}
		SLATE_END_ARGS()

		virtual void Construct(const FArguments& args) = 0;

	public:
		virtual void SetTarget(const IZFieldTreeItem::TargetType& target) = 0;
		
	};
}



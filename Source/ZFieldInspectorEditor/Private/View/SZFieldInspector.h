// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SUserWidget.h"

namespace ZFieldInspector
{
	class SZFieldInspector : public SUserWidget
	{
	
	public:
		SLATE_USER_ARGS(SZFieldInspector){}
		SLATE_END_ARGS()

		virtual void Construct(const FArguments& args, bool reversed) = 0;
	
	};
}



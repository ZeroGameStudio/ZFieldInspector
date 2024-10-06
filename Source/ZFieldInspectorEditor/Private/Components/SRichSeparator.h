// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

namespace ZFieldInspector
{
	/**
	 * An enhanced separator that can hold an arbitrary user widget like this:
	 * --------------------- <User Widget> ---------------------
	 */
	class SRichSeparator : public SCompoundWidget
	{

	public:
		SLATE_BEGIN_ARGS(SRichSeparator)
			: _SeparatorImage(FCoreStyle::Get().GetBrush("Separator"))
			, _Orientation(Orient_Horizontal)
			, _Thickness(3.0f)
			, _Padding(5.f)
			, _ColorAndOpacity(FLinearColor::White){}
			SLATE_ARGUMENT(const FSlateBrush*, SeparatorImage)
			SLATE_ARGUMENT(EOrientation, Orientation)
			SLATE_ARGUMENT(float, Thickness)
			SLATE_ARGUMENT(double, Padding)
			SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
			SLATE_DEFAULT_SLOT(FArguments, UserWidget)
		SLATE_END_ARGS()

		void Construct(const FArguments& args);
		
	};
}



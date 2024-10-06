// Copyright Zero Games. All Rights Reserved.


#include "SRichSeparator.h"

void ZFieldInspector::SRichSeparator::Construct(const FArguments& args)
{
	if (args._UserWidget.Widget == SNullWidget::NullWidget)
	{
		// If no user widget then fallback to SSeparator.
		ChildSlot
		[
			SNew(SSeparator)
			.SeparatorImage(args._SeparatorImage)
			.Orientation(args._Orientation)
			.Thickness(args._Thickness)
			.ColorAndOpacity(args._ColorAndOpacity)
		];
	}
	else if (args._Orientation == Orient_Vertical)
	{
		// Use vertical box if vertical.
		ChildSlot
		[
			SNew(SVerticalBox)
		
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SSeparator)
				.SeparatorImage(args._SeparatorImage)
				.Orientation(args._Orientation)
				.Thickness(args._Thickness)
				.ColorAndOpacity(args._ColorAndOpacity)
			]

			+ SVerticalBox::Slot()
			[
				SNew(SSpacer)
				.Size(FVector2D { 0.0, args._Padding })
			]
		
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				args._UserWidget.Widget
			]

			+ SVerticalBox::Slot()
			[
				SNew(SSpacer)
				.Size(FVector2D { 0.0, args._Padding })
			]
				
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Fill)
			[
				SNew(SSeparator)
				.SeparatorImage(args._SeparatorImage)
				.Orientation(args._Orientation)
				.Thickness(args._Thickness)
				.ColorAndOpacity(args._ColorAndOpacity)	
			]
		];
	}
	else
	{
		// Otherwise use horizontal box.
		ChildSlot
		[
			SNew(SHorizontalBox)
		
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SSeparator)
				.SeparatorImage(args._SeparatorImage)
				.Orientation(args._Orientation)
				.Thickness(args._Thickness)
				.ColorAndOpacity(args._ColorAndOpacity)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D { args._Padding, 0.0 })
			]
		
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				args._UserWidget.Widget
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D { args._Padding, 0.0 })
			]
				
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SSeparator)
				.SeparatorImage(args._SeparatorImage)
				.Orientation(args._Orientation)
				.Thickness(args._Thickness)
				.ColorAndOpacity(args._ColorAndOpacity)	
			]
		];
	}
}



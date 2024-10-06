// Copyright Zero Games. All Rights Reserved.


#include "SZFieldDetailsView.h"

#include "Components/SRichSeparator.h"

namespace ZFieldInspector::ZFieldDetailsView_Private
{
	class SZFieldDetailsViewImpl : public SZFieldDetailsView
	{

	public:
		virtual void Construct(const FArguments& args) override;

	public:
		virtual void SetTarget(const IZFieldTreeItem::TargetType& target) override;

	private:
		TSharedRef<SWidget> RefreshView(UPackage* package);
		TSharedRef<SWidget> RefreshView(UField* field);
		TSharedRef<SWidget> RefreshView(FField* field);
	
	private:
		TSharedPtr<SVerticalBox> Box;
		FSlateBrush SeparatorBrush;

	};

	void SZFieldDetailsViewImpl::Construct(const FArguments& args)
	{
		SUserWidget::Construct(SUserWidget::FArguments{}
		[
			SAssignNew(Box, SVerticalBox)
		]);
	}

	void SZFieldDetailsViewImpl::SetTarget(const IZFieldTreeItem::TargetType& target)
	{
		Box->ClearChildren();
		if (Visit([](const auto& value){ return !value; }, target))
		{
			return;
		}
		
		Box->AddSlot()
		.AutoHeight()
		[
			Visit([this](auto& value){ return RefreshView(value); }, target)
		];

		Box->AddSlot()
		.AutoHeight()
		[
			SNew(SRichSeparator)
			.SeparatorImage(&SeparatorBrush)
			.ColorAndOpacity(FLinearColor { 0.75f, 0.75f, 0.75f })
			.Thickness(1.f)
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("Test", "Test", "I AM SEPARATOR"))
			]
		];
	}
	
	TSharedRef<SWidget> SZFieldDetailsViewImpl::RefreshView(UPackage* package)
	{
		return SNew(STextBlock).Text(FText::FromName(GetFNameSafe(package)));
	}

	TSharedRef<SWidget> SZFieldDetailsViewImpl::RefreshView(UField* field)
	{
		return SNew(STextBlock).Text(FText::FromName(GetFNameSafe(field)));
	}

	TSharedRef<SWidget> SZFieldDetailsViewImpl::RefreshView(FField* field)
	{
		return SNew(STextBlock).Text(FText::FromName(GetFNameSafe(field)));
	}
}

TSharedRef<ZFieldInspector::SZFieldDetailsView> ZFieldInspector::SZFieldDetailsView::New()
{
	return MakeShared<ZFieldDetailsView_Private::SZFieldDetailsViewImpl>(); 
}



// Copyright Zero Games. All Rights Reserved.


#include "SZFieldInspector.h"

#include "SZFieldDetailsView.h"
#include "SZFieldTreeRow.h"
#include "SZFieldTreeView.h"
#include "ViewModel/ZPackageFieldTreeItem.h"
#include "Widgets/Input/SSearchBox.h"

#define LOCTEXT_NAMESPACE "SZFieldInspector"

namespace ZFieldInspector::ZFieldInspector_Private
{
	class SZFieldInspectorImpl : public SZFieldInspector
	{

		using ThisClass = SZFieldInspectorImpl;

	public:
		virtual void Construct(const FArguments& args, bool reversed) override;

	private:
		void RebuildItems();

	private:
		void HandleGetChildren(TSharedPtr<IZFieldTreeItem> parent, TArray<TSharedPtr<IZFieldTreeItem>>& children);
		TSharedRef<ITableRow> HandleGenerateRow(TSharedPtr<IZFieldTreeItem> item, const TSharedRef<STableViewBase>& TableViewBase);
		void HandleSelectionChanged(TSharedPtr<IZFieldTreeItem> item, ESelectInfo::Type selectInfo);
		void HandleExpansionChanged(TSharedPtr<IZFieldTreeItem> item, bool expanded) const;
		FReply HandlePathQuery();
		
	private:
		TSharedPtr<SSearchBox> SearchBox;
		TSharedPtr<SZFieldTreeView> TreeView;
		TSharedPtr<SZFieldDetailsView> DetailsView;
		TSharedPtr<SEditableTextBox> PathText;

	private:
		TArray<TSharedPtr<IZFieldTreeItem>> Items;
		
	};

	void SZFieldInspectorImpl::Construct(const FArguments& args, bool reversed)
	{
		RebuildItems();

		static constexpr float GTreeWidth = 0.33f;
		static constexpr float GDetailsWidth = 1 - GTreeWidth;

		TSharedRef<SWidget> vb =
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(SearchBox, SSearchBox)
				.HintText(LOCTEXT("SearchBoxHint", "Search compiled-in fields by name"))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("CompiledInOnlyHint", "There are only compiled-in fields, use path query if you want to inspect dynamic fields."))
				.AutoWrapText(true)
			]

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(TreeView, SZFieldTreeView)
				.TreeItemsSource(&Items)
				.OnGetChildren(this, &ThisClass::HandleGetChildren)
				.OnGenerateRow(this, &ThisClass::HandleGenerateRow)
				.SelectionMode(ESelectionMode::Single)
				.OnSelectionChanged(this, &ThisClass::HandleSelectionChanged)
				.OnExpansionChanged(this, &ThisClass::HandleExpansionChanged)
				.HighlightParentNodesForSelection(true)
				.AllowInvisibleItemSelection(true)
			];

		TSharedRef<SWidget> details = SAssignNew(DetailsView, SZFieldDetailsView);
		
		SUserWidget::Construct(SUserWidget::FArguments{}
		[
			SNew(SBorder)
			.Padding(2)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					
					+ SHorizontalBox::Slot()
					[
						SAssignNew(PathText, SEditableTextBox)
						.HintText(LOCTEXT("PathQueryHint", "Query field by object path (Only supports UField)"))
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Right)
					[
						SNew(SButton)
						.Text(LOCTEXT("Query", "Query"))
						.OnClicked(this, &ThisClass::HandlePathQuery)
					]
				]

				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SSplitter)
                    .Orientation(Orient_Horizontal)
                    
                    + SSplitter::Slot()
                    .Value(reversed ? GDetailsWidth : GTreeWidth)
                    [
                    	reversed ? details : vb
                    ]
                    
                    + SSplitter::Slot()
                    .Value(reversed ? GTreeWidth : GDetailsWidth)
                    [
                    	reversed ? vb : details
                    ]
				]
			]
		]);
	}

	void SZFieldInspectorImpl::RebuildItems()
	{
		for (TObjectIterator<UPackage> it; it; ++it)
		{
			UPackage* package = *it;
			if (package->HasAllPackagesFlags(PKG_CompiledIn))
			{
				Items.Emplace(MakeShared<FZPackageFieldTreeItem>(package));
			}
		}

		Items.StableSort([](const TSharedPtr<IZFieldTreeItem>& lhs, const TSharedPtr<IZFieldTreeItem>& rhs)
		{
			return lhs->GetName() < rhs->GetName();
		});
	}

	void SZFieldInspectorImpl::HandleGetChildren(TSharedPtr<IZFieldTreeItem> parent, TArray<TSharedPtr<IZFieldTreeItem>>& children)
	{
		parent->ForeachChildren([&children](TSharedRef<IZFieldTreeItem> child)
		{
			children.Emplace(child);
		});
	}

	TSharedRef<ITableRow> SZFieldInspectorImpl::HandleGenerateRow(TSharedPtr<IZFieldTreeItem> item, const TSharedRef<STableViewBase>& TableViewBase)
	{
		return SNew(SZFieldTreeRow, TreeView.ToSharedRef()).Item(item);
	}

	void SZFieldInspectorImpl::HandleSelectionChanged(TSharedPtr<IZFieldTreeItem> item, ESelectInfo::Type selectInfo)
	{
		DetailsView->SetTarget(item ? item->GetTarget() : IZFieldTreeItem::TargetType{});
	}

	void SZFieldInspectorImpl::HandleExpansionChanged(TSharedPtr<IZFieldTreeItem> item, bool expanded) const
	{
		TreeView->SetItemExpansion(item, expanded);
	}

	FReply SZFieldInspectorImpl::HandlePathQuery()
	{
		TOptional<FText> errorText;
		FString path = PathText->GetText().ToString();
		const auto object = LoadObject<UObject>(nullptr, *path);
		if (!object)
		{
			errorText = LOCTEXT("PathQueryResultNotFound", "Field not found.");
		}

		const auto field = Cast<UField>(object);
		if (!field && !errorText)
		{
			errorText = FText::Format(LOCTEXT("PathQueryResultTypeMismatch", "Result type is expected to be UField, but {0}."), FText::FromName(GetFNameSafe(object->GetClass())));
		}

		if (!errorText)
		{
			TreeView->ClearSelection();
			IZFieldTreeItem::TargetType target;
			target.Set<UField*>(field);
			DetailsView->SetTarget(target);
		}
		else
		{
			FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok, *errorText);
		}
		
		return FReply::Handled();
	}
}

TSharedRef<ZFieldInspector::SZFieldInspector> ZFieldInspector::SZFieldInspector::New()
{
	return MakeShared<ZFieldInspector_Private::SZFieldInspectorImpl>(); 
}

#undef LOCTEXT_NAMESPACE



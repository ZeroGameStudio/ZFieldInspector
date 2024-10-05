// Copyright Zero Games. All Rights Reserved.


#include "SZFieldTreeRow.h"

#include "ViewModel/IZFieldTreeItem.h"

#define LOCTEXT_NAMESPACE "SZFieldTreeRow"

namespace ZFieldInspector::ZFieldTreeRow_Private
{
	struct FZDisplayTextInfo
	{
		FText Text;
		FSlateColor Color;
	};
	
	FZDisplayTextInfo GetDisplayTextInfo(const IZFieldTreeItem::TargetType& target)
	{
		static const FTextFormat GFormatWithAttr = LOCTEXT("DisplayText", "{0} ({1}) {2}");
		static const FTextFormat GFormatWithoutAttr = LOCTEXT("DisplayText", "{0} ({1})");

		static const FColor GDeprecatedColor = FColor { 127, 127, 127 };
		
		FZDisplayTextInfo info;

		bool deprecated = false;
		FString type;
		FString name;
		TArray<FString> attrs;
		FColor color;
		if (target.IsType<UPackage*>())
		{
			const auto package = target.Get<UPackage*>();
			
			type = "Package";
			package->GetName().Split("/", nullptr, &name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			color = FColor::White;
		}
		else if (target.IsType<UField*>())
		{
			const auto field = target.Get<UField*>();

			if (const auto cls = Cast<UClass>(field))
			{
				type = "Class";
				
				if (cls->HasAllClassFlags(CLASS_Interface))
				{
					attrs.Emplace("Interface");
				}
				else if (cls->HasAllClassFlags(CLASS_Abstract))
				{
					attrs.Emplace("Abstract");
				}
				
				color = FColor::Orange;

				if (cls->HasAllClassFlags(CLASS_Deprecated))
				{
					deprecated = true;
				}
			}
			else if (const auto strct = Cast<UScriptStruct>(field))
			{
				type = "Struct";
				color = FColor::Turquoise;
			}
			else if (const auto enm = Cast<UEnum>(field))
			{
				type = "Enum";
				
				if (enm->HasAnyEnumFlags(EEnumFlags::Flags))
				{
					attrs.Emplace("Flags");
				}
				
				color = FColor::Emerald;
			}
			else if (const auto function = Cast<UFunction>(field))
			{
				if (const auto delegate = Cast<UDelegateFunction>(function))
				{
					type = "Delegate";
					
					if (delegate->HasAllFunctionFlags(FUNC_MulticastDelegate))
					{
						attrs.Emplace("Multicast");
					}
					if (delegate->IsA<USparseDelegateFunction>())
					{
						attrs.Emplace("Sparse");
					}

					color = FColor { 255, 149, 230 };
				}
				else
				{
					type = "Function";
					
					if (function->HasAllFunctionFlags(FUNC_Static))
					{
						attrs.Emplace("Static");
					}
					if (function->HasAllFunctionFlags(FUNC_BlueprintEvent))
					{
						attrs.Emplace("Virtual");
					}
					if (function->HasAllFunctionFlags(FUNC_Net))
					{
						attrs.Emplace("RPC");
					}

					color = FColor::Purple;
				}

				if (const UClass* outer = function->GetOwnerClass())
				{
					if (outer->HasAllClassFlags(CLASS_Deprecated))
					{
						deprecated = true;
					}
				}
			}
			else
			{
				ensureAlways(false);
				type = "Unknown";
			}

			name = field->GetName();

			static const FString GDelegatePostfix = "__DelegateSignature";
			if (field->IsA<UDelegateFunction>() && name.EndsWith(GDelegatePostfix))
			{
				name.LeftChopInline(GDelegatePostfix.Len());
			}
		}
		else
		{
			const auto field = target.Get<FField*>();
			const auto property = CastFieldChecked<FProperty>(field);
			if (const auto function = Cast<UFunction>(property->GetOwnerUField()))
			{
				if (property->HasAllPropertyFlags(CPF_Parm))
				{
					const bool ret = property->HasAllPropertyFlags(CPF_ReturnParm);
					
					type = ret ? "Return" : "Parameter";
					
					if (!ret)
					{
						const bool cnst = property->HasAnyPropertyFlags(CPF_ConstParm);
						if (property->HasAllPropertyFlags(CPF_ReferenceParm) && !cnst)
						{
							attrs.Emplace("InOut");
						}
						else if (property->HasAllPropertyFlags(CPF_OutParm) && !cnst)
						{
							attrs.Emplace("Out");
						}
						else
						{
							attrs.Emplace("In");
						}
					}
					
					color = ret ? FColor::Green : FColor::Cyan;
				}
				else
				{
					type = "Local";
					color = FColor::Silver;
				}

				if (const UClass* outer = function->GetOwnerClass())
				{
					if (outer->HasAllClassFlags(CLASS_Deprecated))
					{
						deprecated = true;
					}
				}
			}
			else
			{
				type = "Property";
				
				if (property->HasAllPropertyFlags(CPF_Net))
				{
					attrs.Emplace("Replicated");
				}
				
				color = FColor { 159, 153, 255 };
			}

			name = property->GetName();

			if (property->HasAllPropertyFlags(CPF_Deprecated))
			{
				deprecated = true;
			}
			else if (const UClass* outer = property->GetOwnerClass())
			{
				if (outer->HasAllClassFlags(CLASS_Deprecated))
				{
					deprecated = true;
				}
			}
		}

		if (deprecated)
		{
			name.InsertAt(0, "DEPRECATED - ");
			color = GDeprecatedColor;
		}

		if (attrs.IsEmpty())
		{
			info.Text = FText::Format(GFormatWithoutAttr, FText::FromString(name), FText::FromString(type));
		}
		else
		{
			info.Text = FText::Format(GFormatWithAttr, FText::FromString(name), FText::FromString(type), FText::FromString(FString::JoinBy(attrs, TEXT(" "), [](const FString& attr){ return FString::Printf(TEXT("[%s]"), *attr); })));
		}

		// FSlateColor(const FColor&) directly reinterpret FColor to FLinearColor, which leads to wrong color, so we cast to FLinearColor manually.
		info.Color = FLinearColor { color };

		return info;
	}
}

void ZFieldInspector::SZFieldTreeRow::Construct(const FArguments& args, TSharedRef<STableViewBase> owner)
{
	IZFieldTreeItem::TargetType target = args._Item->GetTarget();
	ZFieldTreeRow_Private::FZDisplayTextInfo info = ZFieldTreeRow_Private::GetDisplayTextInfo(target);
	
	Super::Construct(Super::FArguments{}
	[
		SNew(STextBlock)
		.Text(info.Text)
		.ColorAndOpacity(info.Color)
	], owner);
}



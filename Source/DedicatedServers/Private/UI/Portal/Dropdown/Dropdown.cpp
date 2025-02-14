// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dropdown/Dropdown.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UDropdown::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Expander->OnClicked.AddDynamic(this, &UDropdown::ToggleDropDown);
	Button_Expander->OnHovered.AddDynamic(this, &UDropdown::Hover);
	Button_Expander->OnUnhovered.AddDynamic(this, &UDropdown::Unhover);
}


void UDropdown::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetStyleTransparent();
	Collapse();
	Unhover();
}

void UDropdown::SetStyleTransparent()
{
	FButtonStyle Style;

	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f,0.f,0.f,0.f ));

	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;

	Button_Expander->SetStyle(Style);
}

void UDropdown::Hover()
{
	TextBlock_ButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UDropdown::Unhover()
{
	TextBlock_ButtonText->SetColorAndOpacity(UnHoveredTextColor);
}

void UDropdown::Expand()
{
	WidgetSwitcher->SetActiveWidget(ExpandedWidget);
	Image_Triangle->SetBrush(TriangleUp);
	bIsExpanded = true;
}

void UDropdown::Collapse()
{
	WidgetSwitcher->SetActiveWidget(CollapsedWidget);
	Image_Triangle->SetBrush(TriangleDown);
	bIsExpanded = false;
}

void UDropdown::ToggleDropDown()
{
	if (bIsExpanded)
	{
		Collapse();
	}
	else
	{
		Expand();
	}
}

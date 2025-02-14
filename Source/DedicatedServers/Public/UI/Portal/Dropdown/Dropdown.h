// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dropdown.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class UImage;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDropdown : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> ExpandedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> CollapsedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Expander;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor UnHoveredTextColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush TriangleUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush TriangleDown;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Triangle;

protected:

	
	virtual void NativeConstruct() override;
	void SetStyleTransparent();
	virtual void NativePreConstruct() override;

private:
	
	UFUNCTION()
	void Hover();
	
	UFUNCTION()
	void Unhover();
	UFUNCTION()
	void ToggleDropDown();

	bool bIsExpanded = false;

	void Expand();

	void Collapse();

	

};

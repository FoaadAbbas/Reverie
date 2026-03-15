// LiveSimulator - Childhood Segment
// Player controller for the kindergarten childhood segment

#include "ChildhoodPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AChildhoodPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add all default input mapping contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (int32 i = 0; i < DefaultMappingContexts.Num(); ++i)
		{
			if (DefaultMappingContexts[i])
			{
				// Skip mobile-excluded contexts when not using touch
				if (!ShouldUseTouchControls() && MobileExcludedMappingContexts.Contains(DefaultMappingContexts[i]))
				{
					continue;
				}
				Subsystem->AddMappingContext(DefaultMappingContexts[i], i);
			}
		}
	}

	// Spawn mobile controls widget if needed
	if (ShouldUseTouchControls() && MobileControlsWidgetClass)
	{
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
		if (MobileControlsWidget)
		{
			MobileControlsWidget->AddToViewport();
		}
	}
}

void AChildhoodPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

bool AChildhoodPlayerController::ShouldUseTouchControls() const
{
	if (bForceTouchControls)
	{
		return true;
	}

	// Check for actual mobile platform
	FString PlatformName = UGameplayStatics::GetPlatformName();
	return PlatformName == TEXT("IOS") || PlatformName == TEXT("Android");
}

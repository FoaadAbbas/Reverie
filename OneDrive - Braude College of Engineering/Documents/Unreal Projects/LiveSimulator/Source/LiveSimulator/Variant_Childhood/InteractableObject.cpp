// LiveSimulator - Childhood Segment
// Base class for interactable objects (toys, drawing boards, swings, etc.)

#include "InteractableObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the visual mesh
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;

	// Create the interaction trigger sphere
	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetupAttachment(RootComponent);
	InteractionTrigger->SetSphereRadius(120.0f);
	InteractionTrigger->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionTrigger->SetGenerateOverlapEvents(true);

	bIsInteractable = true;
	bOnCooldown = false;

	InteractionPrompt = FText::FromString(TEXT("Play"));
}

void AInteractableObject::Interact(AActor* Interactor)
{
	if (!IsInteractable() || !Interactor)
	{
		return;
	}

	// Notify Blueprint
	OnInteract(Interactor);

	// Start cooldown
	if (InteractionCooldown > 0.0f)
	{
		bOnCooldown = true;
		GetWorldTimerManager().SetTimer(
			CooldownTimer,
			this,
			&AInteractableObject::ResetCooldown,
			InteractionCooldown,
			false
		);
	}
}

void AInteractableObject::ResetCooldown()
{
	bOnCooldown = false;
	OnCooldownComplete();
}

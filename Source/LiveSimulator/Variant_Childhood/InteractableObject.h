// LiveSimulator - Childhood Segment
// Base class for interactable objects (toys, drawing boards, swings, etc.)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 *  Base class for interactable objects in the kindergarten.
 *  Toys, drawing boards, swings, books, and other childhood activities.
 *  Has a detection sphere, interaction prompt, and Blueprint-implementable events.
 */
UCLASS()
class AInteractableObject : public AActor
{
	GENERATED_BODY()

public:

	AInteractableObject();

protected:

	/** Visual mesh of the object */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ObjectMesh;

	/** Collision sphere for interaction range detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* InteractionTrigger;

	/** Prompt text displayed when the player is in range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionPrompt;

	/** Whether this object can currently be interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsInteractable;

	/** Cooldown between interactions (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = 0, ClampMax = 30, Units = "s"))
	float InteractionCooldown = 3.0f;

	/** Whether the object is currently on cooldown */
	bool bOnCooldown;

	/** Timer for interaction cooldown */
	FTimerHandle CooldownTimer;

public:

	/** Called by the player character to interact with this object */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	/** Returns the interaction prompt text */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const { return InteractionPrompt; }

	/** Returns whether the object can be interacted with */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsInteractable() const { return bIsInteractable && !bOnCooldown; }

protected:

	/** Blueprint event: called when interaction occurs */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteract(AActor* Interactor);

	/** Blueprint event: called when interaction cooldown ends */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnCooldownComplete();

	/** Resets the cooldown */
	void ResetCooldown();
};

// LiveSimulator - Childhood Segment
// Player character for the kindergarten childhood segment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ChildhoodCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class USphereComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogChildhoodCharacter, Log, All);

/**
 *  A child character for the kindergarten segment.
 *  Simple third-person movement with an interaction system
 *  for playing with toys, talking to friends, and engaging in activities.
 */
UCLASS(abstract)
class AChildhoodCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Interaction detection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionSphere;

protected:

	// ========== Input Actions ==========

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	// ========== Movement Settings ==========

	/** Walk speed for the child character (slower than adult) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 50, ClampMax = 400, Units = "cm/s"))
	float ChildWalkSpeed = 200.0f;

	// ========== Interaction System ==========

	/** Radius for detecting interactable objects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = 50, ClampMax = 500, Units = "cm"))
	float InteractionRadius = 150.0f;

	/** Currently targeted interactable actor (closest in range) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractTarget;

public:

	AChildhoodCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called when interact button is pressed */
	void InteractPressed();

	/** Finds the closest interactable actor in range */
	void UpdateInteractionTarget();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump released */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** Handles interact pressed */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoInteract();

	/** Blueprint event called when an interaction occurs */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractWithTarget(AActor* Target);

	/** Blueprint event called when the interaction target changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionTargetChanged(AActor* NewTarget);

public:

	/** Returns CameraBoom subobject */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

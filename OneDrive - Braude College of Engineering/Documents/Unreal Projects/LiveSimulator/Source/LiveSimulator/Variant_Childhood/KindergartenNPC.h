// LiveSimulator - Childhood Segment
// NPC character for kindergarten friends and teacher

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KindergartenNPC.generated.h"

class UBehaviorTree;
class UBlackboardData;

/**
 * Type of NPC in the kindergarten
 */
UENUM(BlueprintType)
enum class ENPCType : uint8
{
	Friend		UMETA(DisplayName = "Friend"),
	Teacher		UMETA(DisplayName = "Teacher")
};

/**
 * NPC activity state
 */
UENUM(BlueprintType)
enum class ENPCActivity : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Wandering	UMETA(DisplayName = "Wandering"),
	Playing		UMETA(DisplayName = "Playing"),
	Learning	UMETA(DisplayName = "Learning"),
	Talking		UMETA(DisplayName = "Talking"),
	Interacting	UMETA(DisplayName = "Interacting With Player")
};

/**
 *  NPC character for the kindergarten segment.
 *  Represents friends and the teacher. Has simple AI behavior:
 *  wander around, play animations, and respond to player interactions.
 */
UCLASS()
class AKindergartenNPC : public ACharacter
{
	GENERATED_BODY()

public:

	AKindergartenNPC();

protected:

	// ========== NPC Identity ==========

	/** Type of this NPC (friend or teacher) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Identity")
	ENPCType NPCType;

	/** Display name of this NPC */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Identity")
	FText NPCName;

	// ========== AI Behavior ==========

	/** Behavior tree for this NPC's AI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AI")
	UBehaviorTree* BehaviorTree;

	/** Blackboard data for this NPC's AI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AI")
	UBlackboardData* BlackboardData;

	/** Current activity of this NPC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|State")
	ENPCActivity CurrentActivity;

	/** Wander radius around the NPC's spawn point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AI", meta = (ClampMin = 100, ClampMax = 2000, Units = "cm"))
	float WanderRadius = 500.0f;

	/** Walk speed for wandering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Movement", meta = (ClampMin = 50, ClampMax = 300, Units = "cm/s"))
	float WanderSpeed = 120.0f;

	/** Spawn location (used as wander center) */
	FVector SpawnLocation;

	// ========== Interaction ==========

	/** Whether this NPC is currently available for interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Interaction")
	bool bCanInteract;

	/** Interaction prompt text shown to the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Interaction")
	FText InteractionPrompt;

	/** Animation montage to play during "play together" interaction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Interaction")
	UAnimMontage* PlayTogetherMontage;

	/** Animation montage for idle activity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Animations")
	UAnimMontage* IdleMontage;

public:

	/** Called when the player interacts with this NPC */
	UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
	void OnPlayerInteract(AActor* Player);

	/** Sets the NPC's current activity */
	UFUNCTION(BlueprintCallable, Category = "NPC|State")
	void SetActivity(ENPCActivity NewActivity);

	/** Returns whether this NPC can be interacted with right now */
	UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
	bool CanPlayerInteract() const { return bCanInteract; }

	/** Returns the NPC type */
	UFUNCTION(BlueprintCallable, Category = "NPC|Identity")
	ENPCType GetNPCType() const { return NPCType; }

protected:

	virtual void BeginPlay() override;

	/** Blueprint event: called when the player starts interacting */
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Interaction")
	void OnInteractionStarted(AActor* Player);

	/** Blueprint event: called when the NPC's activity changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC|State")
	void OnActivityChanged(ENPCActivity NewActivity);
};

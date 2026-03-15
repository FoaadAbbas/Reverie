// LiveSimulator - Childhood Segment
// NPC character for kindergarten friends and teacher

#include "KindergartenNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AKindergartenNPC::AKindergartenNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set default NPC type
	NPCType = ENPCType::Friend;
	CurrentActivity = ENPCActivity::Idle;
	bCanInteract = true;

	// Configure NPC movement (slower child-like movement)
	GetCharacterMovement()->MaxWalkSpeed = WanderSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// AI controlled
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	InteractionPrompt = FText::FromString(TEXT("Play Together"));
}

void AKindergartenNPC::BeginPlay()
{
	Super::BeginPlay();

	// Store spawn location as wander center
	SpawnLocation = GetActorLocation();

	// Apply walk speed
	GetCharacterMovement()->MaxWalkSpeed = WanderSpeed;

	// Run the behavior tree if assigned
	AAIController* AICtrl = Cast<AAIController>(GetController());
	if (AICtrl && BehaviorTree)
	{
		AICtrl->RunBehaviorTree(BehaviorTree);

		// Set wander parameters on blackboard
		UBlackboardComponent* BB = AICtrl->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsVector(TEXT("SpawnLocation"), SpawnLocation);
			BB->SetValueAsFloat(TEXT("WanderRadius"), WanderRadius);
		}
	}
}

void AKindergartenNPC::OnPlayerInteract(AActor* Player)
{
	if (!bCanInteract || !Player)
	{
		return;
	}

	// Change to interacting state
	SetActivity(ENPCActivity::Interacting);
	bCanInteract = false;

	// Face the player
	FVector DirectionToPlayer = Player->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Z = 0.0f;
	if (!DirectionToPlayer.IsNearlyZero())
	{
		SetActorRotation(DirectionToPlayer.Rotation());
	}

	// Play "play together" animation if available
	if (PlayTogetherMontage)
	{
		PlayAnimMontage(PlayTogetherMontage);
	}

	// Notify Blueprint
	OnInteractionStarted(Player);

	// Re-enable interaction after a delay (handled via anim notify or timer)
	FTimerHandle ResetTimer;
	GetWorldTimerManager().SetTimer(ResetTimer, [this]()
	{
		bCanInteract = true;
		SetActivity(ENPCActivity::Idle);
	}, 5.0f, false);
}

void AKindergartenNPC::SetActivity(ENPCActivity NewActivity)
{
	if (CurrentActivity != NewActivity)
	{
		CurrentActivity = NewActivity;
		OnActivityChanged(NewActivity);
	}
}

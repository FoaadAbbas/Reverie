// LiveSimulator - Childhood Segment
// Player character for the kindergarten childhood segment

#include "ChildhoodCharacter.h"
#include "InteractableObject.h"
#include "KindergartenNPC.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogChildhoodCharacter);

AChildhoodCharacter::AChildhoodCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to match camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = ChildWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 40.0f, 60.0f);

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create interaction sphere
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionRadius);
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionSphere->SetGenerateOverlapEvents(true);

	CurrentInteractTarget = nullptr;
}

void AChildhoodCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Apply child walk speed
	GetCharacterMovement()->MaxWalkSpeed = ChildWalkSpeed;
}

void AChildhoodCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Continuously check for closest interactable
	UpdateInteractionTarget();
}

void AChildhoodCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AChildhoodCharacter::Move);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChildhoodCharacter::Look);
		}
		if (MouseLookAction)
		{
			EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AChildhoodCharacter::Look);
		}

		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AChildhoodCharacter::DoJumpStart);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AChildhoodCharacter::DoJumpEnd);
		}

		// Interacting
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AChildhoodCharacter::InteractPressed);
		}
	}
}

void AChildhoodCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AChildhoodCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AChildhoodCharacter::DoMove(float Right, float Forward)
{
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AChildhoodCharacter::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void AChildhoodCharacter::DoJumpStart()
{
	Jump();
}

void AChildhoodCharacter::DoJumpEnd()
{
	StopJumping();
}

void AChildhoodCharacter::InteractPressed()
{
	DoInteract();
}

void AChildhoodCharacter::DoInteract()
{
	if (CurrentInteractTarget)
	{
		UE_LOG(LogChildhoodCharacter, Log, TEXT("Interacting with: %s"), *CurrentInteractTarget->GetName());

		// Try to call Interact on InteractableObject
		AInteractableObject* Interactable = Cast<AInteractableObject>(CurrentInteractTarget);
		if (Interactable)
		{
			Interactable->Interact(this);
		}

		// Notify Blueprint
		OnInteractWithTarget(CurrentInteractTarget);
	}
}

void AChildhoodCharacter::UpdateInteractionTarget()
{
	TArray<AActor*> OverlappingActors;
	InteractionSphere->GetOverlappingActors(OverlappingActors);

	AActor* ClosestTarget = nullptr;
	float ClosestDistance = MAX_FLT;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			// Check if it's an interactable object or a kindergarten NPC
			AInteractableObject* AsInteractable = Cast<AInteractableObject>(Actor);
			AKindergartenNPC* AsNPC = Cast<AKindergartenNPC>(Actor);

			if (AsInteractable || AsNPC)
			{
				float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestTarget = Actor;
				}
			}
		}
	}

	if (ClosestTarget != CurrentInteractTarget)
	{
		CurrentInteractTarget = ClosestTarget;
		OnInteractionTargetChanged(CurrentInteractTarget);
	}
}

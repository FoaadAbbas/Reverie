// LiveSimulator - Childhood Segment
// Ghost doctor actor that appears as a coma hint in the background

#include "DoctorGhostActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ADoctorGhostActor::ADoctorGhostActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the ghost mesh
	GhostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GhostMesh"));
	RootComponent = GhostMesh;

	// No collision — purely visual
	GhostMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GhostMesh->SetCastShadow(false);

	CurrentState = EGhostState::FadingIn;
	StateTimer = 0.0f;
	CurrentOpacity = 0.0f;
}

void ADoctorGhostActor::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material for opacity control
	if (GhostMesh && GhostMesh->GetMaterial(0))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMesh->GetMaterial(0), this);
		GhostMesh->SetMaterial(0, DynamicMaterial);
		UpdateOpacity(0.0f);
	}

	CurrentState = EGhostState::FadingIn;
	StateTimer = 0.0f;

	OnGhostAppear();
}

void ADoctorGhostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateTimer += DeltaTime;

	switch (CurrentState)
	{
	case EGhostState::FadingIn:
	{
		float Alpha = FMath::Clamp(StateTimer / FMath::Max(FadeInDuration, 0.01f), 0.0f, 1.0f);
		UpdateOpacity(FMath::Lerp(0.0f, MaxOpacity, Alpha));

		if (StateTimer >= FadeInDuration)
		{
			CurrentState = EGhostState::Visible;
			StateTimer = 0.0f;
			UpdateOpacity(MaxOpacity);
		}
		break;
	}

	case EGhostState::Visible:
	{
		if (StateTimer >= VisibleDuration)
		{
			CurrentState = EGhostState::FadingOut;
			StateTimer = 0.0f;
			OnGhostDisappear();
		}
		break;
	}

	case EGhostState::FadingOut:
	{
		float Alpha = FMath::Clamp(StateTimer / FMath::Max(FadeOutDuration, 0.01f), 0.0f, 1.0f);
		UpdateOpacity(FMath::Lerp(MaxOpacity, 0.0f, Alpha));

		if (StateTimer >= FadeOutDuration)
		{
			CurrentState = EGhostState::Done;
			Destroy();
		}
		break;
	}

	case EGhostState::Done:
		break;
	}
}

void ADoctorGhostActor::UpdateOpacity(float NewOpacity)
{
	CurrentOpacity = NewOpacity;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(OpacityParameterName, CurrentOpacity);
	}
}

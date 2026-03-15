// LiveSimulator - Childhood Segment
// Medical audio manager for random coma hint sounds

#include "MedicalAudioManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AMedicalAudioManager::AMedicalAudioManager()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;

	// Don't auto-activate — we control playback manually
	AudioComponent->bAutoActivate = false;

	bIsActive = false;
}

void AMedicalAudioManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMedicalAudioManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopAudioCycle();
	Super::EndPlay(EndPlayReason);
}

void AMedicalAudioManager::StartAudioCycle()
{
	if (!bIsActive)
	{
		bIsActive = true;
		ScheduleNextSound();
	}
}

void AMedicalAudioManager::StopAudioCycle()
{
	bIsActive = false;
	GetWorldTimerManager().ClearTimer(SoundTimer);
}

void AMedicalAudioManager::SetIntensity(float NewIntensity)
{
	Intensity = FMath::Clamp(NewIntensity, 0.0f, 1.0f);
}

void AMedicalAudioManager::PlayRandomSound()
{
	if (!bIsActive || MedicalSounds.Num() == 0)
	{
		if (bIsActive)
		{
			ScheduleNextSound();
		}
		return;
	}

	// Pick a random sound
	int32 SoundIndex = FMath::RandRange(0, MedicalSounds.Num() - 1);
	USoundBase* SelectedSound = MedicalSounds[SoundIndex];

	if (SelectedSound)
	{
		// Calculate volume based on base volume and intensity
		float FinalVolume = SoundVolume * FMath::Lerp(0.5f, 1.0f, Intensity);

		// Play the sound at the manager's location (2D-like if placed at origin, or spatial)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SelectedSound,
			GetActorLocation(),
			FinalVolume,
			1.0f  // Pitch
		);

		// Notify Blueprint
		OnMedicalSoundPlayed(SelectedSound);
	}

	// Schedule the next sound
	ScheduleNextSound();
}

void AMedicalAudioManager::ScheduleNextSound()
{
	if (!bIsActive)
	{
		return;
	}

	// Higher intensity = shorter intervals
	float IntensityScale = FMath::Lerp(1.0f, 0.4f, Intensity);
	float AdjustedMin = MinSoundInterval * IntensityScale;
	float AdjustedMax = MaxSoundInterval * IntensityScale;

	float NextInterval = FMath::RandRange(AdjustedMin, AdjustedMax);

	GetWorldTimerManager().SetTimer(
		SoundTimer,
		this,
		&AMedicalAudioManager::PlayRandomSound,
		NextInterval,
		false
	);
}

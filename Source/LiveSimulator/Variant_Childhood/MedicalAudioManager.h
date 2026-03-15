// LiveSimulator - Childhood Segment
// Medical audio manager for random coma hint sounds

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedicalAudioManager.generated.h"

class UAudioComponent;

/**
 *  Manages random playback of medical equipment sounds as coma hints.
 *  Plays sounds like heart monitor beeps, ventilator hissing, IV drip clicks,
 *  and defibrillator charges at random intervals to create an eerie atmosphere
 *  suggesting the player is actually in a hospital.
 */
UCLASS()
class AMedicalAudioManager : public AActor
{
	GENERATED_BODY()

public:

	AMedicalAudioManager();

protected:

	/** Audio component for playing medical sounds */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	/** Array of medical sound effects to randomly pick from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical Audio|Sounds")
	TArray<USoundBase*> MedicalSounds;

	/** Minimum interval between sound plays (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical Audio|Timing", meta = (ClampMin = 5, ClampMax = 120, Units = "s"))
	float MinSoundInterval = 15.0f;

	/** Maximum interval between sound plays (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical Audio|Timing", meta = (ClampMin = 10, ClampMax = 300, Units = "s"))
	float MaxSoundInterval = 60.0f;

	/** Volume multiplier for medical sounds (keep low for subtlety) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical Audio|Volume", meta = (ClampMin = 0.01, ClampMax = 1.0))
	float SoundVolume = 0.15f;

	/** 
	 * Intensity multiplier (0.0 - 1.0). Controls both volume scaling and interval reduction.
	 * In the childhood segment this should be very low (~0.2).
	 * Increase in later segments to make coma hints more obvious.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical Audio|Intensity", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float Intensity = 0.2f;

	/** Timer for scheduling next sound */
	FTimerHandle SoundTimer;

	/** Whether the audio cycle is currently active */
	bool bIsActive;

	/** Plays a random medical sound */
	void PlayRandomSound();

	/** Schedules the next random sound */
	void ScheduleNextSound();

public:

	/** Starts the random medical audio cycle */
	UFUNCTION(BlueprintCallable, Category = "Medical Audio")
	void StartAudioCycle();

	/** Stops the random medical audio cycle */
	UFUNCTION(BlueprintCallable, Category = "Medical Audio")
	void StopAudioCycle();

	/** Sets the intensity (affects volume and frequency) */
	UFUNCTION(BlueprintCallable, Category = "Medical Audio")
	void SetIntensity(float NewIntensity);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Blueprint event: called when a medical sound plays */
	UFUNCTION(BlueprintImplementableEvent, Category = "Medical Audio")
	void OnMedicalSoundPlayed(USoundBase* Sound);
};

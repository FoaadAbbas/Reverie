// LiveSimulator - Childhood Segment
// GameMode for the kindergarten childhood segment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChildhoodGameMode.generated.h"

class ADoctorGhostActor;
class AMedicalAudioManager;

/**
 * Day phase enum representing different parts of the kindergarten day
 */
UENUM(BlueprintType)
enum class EKindergartenDayPhase : uint8
{
	Arrival		UMETA(DisplayName = "Arrival"),
	FreePlay	UMETA(DisplayName = "Free Play"),
	Learning	UMETA(DisplayName = "Learning"),
	Recess		UMETA(DisplayName = "Recess"),
	Departure	UMETA(DisplayName = "Departure")
};

/**
 *  GameMode for the childhood/kindergarten segment.
 *  Manages the day cycle (arrival -> play -> learn -> recess -> departure),
 *  spawns ghost doctor apparitions at random intervals,
 *  and triggers random medical equipment sounds as coma hints.
 */
UCLASS()
class AChildhoodGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AChildhoodGameMode();

protected:

	// ========== Day Phase System ==========

	/** Current phase of the kindergarten day */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Day Cycle")
	EKindergartenDayPhase CurrentPhase;

	/** Duration of each day phase in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle", meta = (ClampMin = 5, ClampMax = 300, Units = "s"))
	float PhaseDuration = 60.0f;

	/** Timer for advancing through day phases */
	FTimerHandle DayPhaseTimer;

	/** Advances to the next day phase */
	void AdvanceDayPhase();

	/** Called when a new day phase begins */
	UFUNCTION(BlueprintImplementableEvent, Category = "Day Cycle")
	void OnDayPhaseChanged(EKindergartenDayPhase NewPhase);

	// ========== Doctor Ghost Spawning ==========

	/** Class to spawn for ghost doctor apparitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coma Hints|Doctor Ghost")
	TSubclassOf<ADoctorGhostActor> DoctorGhostClass;

	/** Spawn points where doctor ghosts can appear (set in the level) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coma Hints|Doctor Ghost")
	TArray<AActor*> DoctorSpawnPoints;

	/** Minimum interval between doctor ghost spawn attempts (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coma Hints|Doctor Ghost", meta = (ClampMin = 10, ClampMax = 120, Units = "s"))
	float MinDoctorSpawnInterval = 30.0f;

	/** Maximum interval between doctor ghost spawn attempts (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coma Hints|Doctor Ghost", meta = (ClampMin = 15, ClampMax = 300, Units = "s"))
	float MaxDoctorSpawnInterval = 90.0f;

	/** Timer for spawning doctor ghosts */
	FTimerHandle DoctorSpawnTimer;

	/** Spawns a doctor ghost at a random spawn point */
	void SpawnDoctorGhost();

	/** Schedules the next doctor ghost spawn */
	void ScheduleNextDoctorSpawn();

	// ========== Medical Audio ==========

	/** Reference to the Medical Audio Manager actor in the level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coma Hints|Medical Audio")
	AMedicalAudioManager* MedicalAudioManager;

protected:

	/** Initialization */
	virtual void BeginPlay() override;

	/** Cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

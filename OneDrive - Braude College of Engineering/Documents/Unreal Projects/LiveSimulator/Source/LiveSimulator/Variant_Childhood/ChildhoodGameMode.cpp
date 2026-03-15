// LiveSimulator - Childhood Segment
// GameMode for the kindergarten childhood segment

#include "ChildhoodGameMode.h"
#include "DoctorGhostActor.h"
#include "MedicalAudioManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

AChildhoodGameMode::AChildhoodGameMode()
{
	CurrentPhase = EKindergartenDayPhase::Arrival;
}

void AChildhoodGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start day cycle
	CurrentPhase = EKindergartenDayPhase::Arrival;
	OnDayPhaseChanged(CurrentPhase);
	GetWorldTimerManager().SetTimer(DayPhaseTimer, this, &AChildhoodGameMode::AdvanceDayPhase, PhaseDuration, false);

	// Start doctor ghost spawn cycle
	ScheduleNextDoctorSpawn();

	// Start medical audio if manager is assigned
	if (MedicalAudioManager)
	{
		MedicalAudioManager->StartAudioCycle();
	}
}

void AChildhoodGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(DayPhaseTimer);
	GetWorldTimerManager().ClearTimer(DoctorSpawnTimer);

	Super::EndPlay(EndPlayReason);
}

void AChildhoodGameMode::AdvanceDayPhase()
{
	// Progress through the day phases
	switch (CurrentPhase)
	{
	case EKindergartenDayPhase::Arrival:
		CurrentPhase = EKindergartenDayPhase::FreePlay;
		break;
	case EKindergartenDayPhase::FreePlay:
		CurrentPhase = EKindergartenDayPhase::Learning;
		break;
	case EKindergartenDayPhase::Learning:
		CurrentPhase = EKindergartenDayPhase::Recess;
		break;
	case EKindergartenDayPhase::Recess:
		CurrentPhase = EKindergartenDayPhase::Departure;
		break;
	case EKindergartenDayPhase::Departure:
		// Day complete — could loop or trigger end-of-segment
		return;
	}

	// Notify Blueprints of the new phase
	OnDayPhaseChanged(CurrentPhase);

	// Schedule next phase (unless it's Departure which ends the cycle)
	if (CurrentPhase != EKindergartenDayPhase::Departure)
	{
		GetWorldTimerManager().SetTimer(DayPhaseTimer, this, &AChildhoodGameMode::AdvanceDayPhase, PhaseDuration, false);
	}
}

void AChildhoodGameMode::SpawnDoctorGhost()
{
	if (!DoctorGhostClass || DoctorSpawnPoints.Num() == 0)
	{
		// Reschedule even if we can't spawn right now
		ScheduleNextDoctorSpawn();
		return;
	}

	// Pick a random spawn point
	int32 SpawnIndex = FMath::RandRange(0, DoctorSpawnPoints.Num() - 1);
	AActor* SpawnPoint = DoctorSpawnPoints[SpawnIndex];

	if (SpawnPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ADoctorGhostActor>(
			DoctorGhostClass,
			SpawnPoint->GetActorLocation(),
			SpawnPoint->GetActorRotation(),
			SpawnParams
		);
	}

	// Schedule the next spawn
	ScheduleNextDoctorSpawn();
}

void AChildhoodGameMode::ScheduleNextDoctorSpawn()
{
	float NextInterval = FMath::RandRange(MinDoctorSpawnInterval, MaxDoctorSpawnInterval);
	GetWorldTimerManager().SetTimer(DoctorSpawnTimer, this, &AChildhoodGameMode::SpawnDoctorGhost, NextInterval, false);
}

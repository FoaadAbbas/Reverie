// LiveSimulator - Childhood Segment
// Ghost doctor actor that appears as a coma hint in the background

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoctorGhostActor.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

/**
 *  A ghostly doctor apparition that randomly appears in the background
 *  as a subtle coma hint. Fades in, stays visible briefly, then fades out
 *  and self-destructs. Designed to create an unsettling feeling
 *  that something isn't quite right about this "normal" childhood.
 */
UCLASS()
class ADoctorGhostActor : public AActor
{
	GENERATED_BODY()

public:

	ADoctorGhostActor();

protected:

	/** The mesh representing the doctor silhouette */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GhostMesh;

	/** Duration of the fade-in effect (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Appearance", meta = (ClampMin = 0.5, ClampMax = 5.0, Units = "s"))
	float FadeInDuration = 1.5f;

	/** Duration the ghost stays fully visible (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Appearance", meta = (ClampMin = 1.0, ClampMax = 15.0, Units = "s"))
	float VisibleDuration = 3.0f;

	/** Duration of the fade-out effect (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Appearance", meta = (ClampMin = 0.5, ClampMax = 5.0, Units = "s"))
	float FadeOutDuration = 2.0f;

	/** Maximum opacity when fully visible (0 = invisible, 1 = full opaque) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Appearance", meta = (ClampMin = 0.05, ClampMax = 0.8))
	float MaxOpacity = 0.3f;

	/** Material parameter name for opacity control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Appearance")
	FName OpacityParameterName = TEXT("Opacity");

	/** Dynamic material instance for opacity control */
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

private:

	/** Current ghost state */
	enum class EGhostState : uint8
	{
		FadingIn,
		Visible,
		FadingOut,
		Done
	};

	EGhostState CurrentState;
	float StateTimer;
	float CurrentOpacity;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Updates the material opacity */
	void UpdateOpacity(float NewOpacity);

	/** Blueprint event: called when the ghost becomes visible */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ghost")
	void OnGhostAppear();

	/** Blueprint event: called when the ghost starts fading out */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ghost")
	void OnGhostDisappear();
};

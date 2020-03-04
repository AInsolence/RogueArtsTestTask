// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ConeActor.generated.h"

class USphereComponent;

UCLASS()
class ROGUEARTSTEST_API AConeActor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Actor components
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent = nullptr;
	// Events
	UFUNCTION()
	void OnClickedThis(AActor* TouchedActor, FKey ButtonPressed);
	UFUNCTION()
	void OnBeginHoverThis(AActor* TouchedActor);
	UFUNCTION()
	void OnEndHoverThis(AActor* TouchedActor);
	// Allow only one actor be selected at one time
	static AConeActor* SelectedActor;
	// Actors cash to avoid multiple calls of FindObject<>()
	static TArray<AConeActor*> ActorsOnMap;
	// Reset selection if another actor selected
	void ResetSelection();
	// Transform functions
	void ScaleActor(float ScaleRange);
	void RotateActor(bool RotateByClockWise);

private:
	// dynamic material which indicates hover/clicked events
	UPROPERTY(EditAnywhere, Category = "OnHoverDetection")
	UMaterialInstanceDynamic * OnHoverMaterialInstance = nullptr;
	bool bIsActorSelected = false;
	void ChangeMeshColorTo(FLinearColor Color);
	float DistanceFromCameraToActor = 0.0;
};

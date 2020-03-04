// Fill out your copyright notice in the Description page of Project Settings.


#include "ConeActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/UserWidget.h"

AConeActor* AConeActor::SelectedActor = nullptr;
TArray<AConeActor*> AConeActor::ActorsOnMap;

// Sets default values
AConeActor::AConeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// set static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	// set mesh component
	auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	if (Mesh.Object)
	{
		MeshComponent->SetStaticMesh(Mesh.Object);
	}
	SetRootComponent(MeshComponent);
	// set mesh physics properties
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetRelativeScale3D(FVector(0.5f));
	// set constraints to avoid actor rotation
	MeshComponent->BodyInstance.bLockXRotation = true;
	MeshComponent->BodyInstance.bLockYRotation = true;
	MeshComponent->BodyInstance.bLockZRotation = true;
}

// Called when the game starts or when spawned
void AConeActor::BeginPlay()
{
	Super::BeginPlay();
	// add events to appropriate delegate
	OnBeginCursorOver.AddDynamic(this, &AConeActor::OnBeginHoverThis);
	OnEndCursorOver.AddDynamic(this, &AConeActor::OnEndHoverThis);
	OnClicked.AddDynamic(this, &AConeActor::OnClickedThis);
	// create dynamic material
	OnHoverMaterialInstance = MeshComponent->CreateDynamicMaterialInstance(0);
	// Add to cash list of all actors on map
	ActorsOnMap.Add(this);
}

void AConeActor::OnClickedThis(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!bIsActorSelected)
	{
		bIsActorSelected = true;
		ChangeMeshColorTo(FLinearColor::Blue);
		// calculate to manipulate actor without changing position
		DistanceFromCameraToActor = this->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (SelectedActor != nullptr)
		{
			SelectedActor->ResetSelection();
		}
		SelectedActor = this;
	}
	else
	{
		bIsActorSelected = false;
		ChangeMeshColorTo(FLinearColor::White);
		SelectedActor = nullptr;
	}
}

void AConeActor::OnBeginHoverThis(AActor* TouchedActor)
{
	if (!bIsActorSelected)
	{
		ChangeMeshColorTo(FLinearColor::Gray);
	}
}

void AConeActor::OnEndHoverThis(AActor* TouchedActor)
{
	if (!bIsActorSelected)
	{
		ChangeMeshColorTo(FLinearColor::White);
	}
}

void AConeActor::ResetSelection()
{
	bIsActorSelected = false;
	ChangeMeshColorTo(FLinearColor::White);
}

void AConeActor::ScaleActor(float ScaleRange)
{
	if (bIsActorSelected)
	{
		this->SetActorRelativeScale3D(this->GetActorScale3D() * ScaleRange);
	}
}

void AConeActor::RotateActor(bool RotateByClockWise)
{
	if (bIsActorSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("ROTATE"));
		if (RotateByClockWise)
		{
			this->SetActorRotation(this->GetActorRotation() + FRotator(0, 0, 10));
		}
		else
		{
			this->SetActorRotation(this->GetActorRotation() - FRotator(0, 0, 10));
		}
		
	}
}

void AConeActor::ChangeMeshColorTo(FLinearColor Color)
{
	if (OnHoverMaterialInstance)
	{
		FLinearColor CurrentMeshColor;
		OnHoverMaterialInstance->GetVectorParameterValue(FName("BaseColor"), CurrentMeshColor);
		OnHoverMaterialInstance->SetVectorParameterValue(FName("BaseColor"), Color);
	}
}

// Called every frame
void AConeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsActorSelected)
	{// Take actor with mouse
		FVector WorlPosition, WorldDirection;
		GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorlPosition, WorldDirection);
		this->SetActorLocation(WorlPosition + WorldDirection*DistanceFromCameraToActor);
	}
	// constraint to restrict strong hit impact for actor
	MeshComponent->SetAllPhysicsLinearVelocity(FVector(0.f));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::ScaleSelectedActor(float ScaleRange)
{
		for (auto Actor : AConeActor::ActorsOnMap)
		{
			Actor->ScaleActor(ScaleRange);
		}
}

void AMyPlayerController::RotateActor(bool RotateByClockWise)
{
	for (auto Actor : AConeActor::ActorsOnMap)
	{
		Actor->RotateActor(RotateByClockWise);
	}
}

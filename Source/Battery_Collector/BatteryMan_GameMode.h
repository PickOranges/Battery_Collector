// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BatteryMan_GameMode.generated.h"

/**
 * GameMode will decide which character is our default character.
 */
UCLASS()
class BATTERY_COLLECTOR_API ABatteryMan_GameMode : public AGameMode
{
	GENERATED_BODY()

	ABatteryMan_GameMode(); // ctor

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	/// <summary>
	/// We want to spawn our battery chargers.
	/// How to use it in UE4?
	/// Go to the gamemode blueprint->battery man game mode->recharger->select pickable item BP.
	/// Then pass the min max of x and y into gamemode blueprint.(Those params can be obtained by measuring the limit of the wall)
	/// </summary>
	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> PlayerRecharge;

	float Spawn_Z = 500.f;  // the spawn height

	UPROPERTY(EditAnywhere)
		float Spawn_X_Min;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Max;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Min;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Max;

	void SpawnPlayerRecharge();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Blueprint/UserWidget.h"

#include "BatteryMan.generated.h"

UCLASS() 
class BATTERY_COLLECTOR_API ABatteryMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABatteryMan();

	// The violet capital text means this is a macro.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	bool bDead;  // Use it to check whether we can move our character or not. If he/she is dead, then not.
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	// Define those data member for pickable item.
	// Note: these data member could be seen in the BatteryMan blueprint.
	// Note: this is the use of blueprint, ie. like a GUI for c++ code.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float Power;
	UPROPERTY(EditAnywhere) float Power_Threshold;

	// We only use the arg OtherComp, but we have to declare like this. Because the other params are decorations.
	// ??? check the book see design pattern delegation and decoration ???
	// OthercComp: the component we have to collect in the game, ie. the pickable item.
	UFUNCTION() // This means the function below is a delegate.
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;
	UUserWidget* Player_Power_Widget;

	void RestartGame();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

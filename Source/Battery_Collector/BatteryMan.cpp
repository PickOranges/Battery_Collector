// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryMan.h"

// Sets default values
ABatteryMan::ABatteryMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get the reference of the capsule. This means we will take the params of capsule from UE4 to here,
	// and we will use code to set and change those params.
	GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Similarly to above. Basically we will take all params from the blueprint we created, then init and set them here with code,
	// instead of managing them with blueprints.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create camera boom.
	// Note: google it to see what is a camera boom.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create camera and attach it to camera boom.
	FollowCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}


// Called when the game starts or when spawned
void ABatteryMan::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABatteryMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input.
// Use: to bind the input devices(eg. mouse, keyboard) onto our character or camera.
// You can check it from UE4: Edit->ProjectSetting->Engine->Input->Bindings

void ABatteryMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ABatteryMan::MoveForward(float Axis)
{
}

void ABatteryMan::MoveRight(float Axis)
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryMan.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

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

	bDead = false;
	Power = 100.f;
}


// Called when the game starts or when spawned
// ??? Is this the main logic loop of the whole game ???
// ??? Will it be updated periodically, eg. update all status and call binded function every 10 seconds  ???
void ABatteryMan::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABatteryMan::OnBeginOverlap);

	// Check whether the progress bar(GUI) is bounded onto the battery man character.
	// See the UE4 blueprint for the details.
	// Create and add the power bar into scene.
	if (Player_Power_Widget_Class!=nullptr) {
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}

}

// Called every frame
// ??? Is this a clock ???
// ??? Is it triggered periodically ???
void ABatteryMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Power -= DeltaTime*Power_Threshold;
	if (Power <= 0.f) {
		if (!bDead) {
			bDead = true;

			// This will make our character fall on the floor. 
			GetMesh()->SetSimulatePhysics(true); 


			// Restart by calling the function Restart() after 3 seconds, and do NOT repeat it(false).
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &ABatteryMan::RestartGame, 3.f, false);  
		}
	}

}

// Called to bind functionality to input.
// Use: to bind the input devices(eg. mouse, keyboard) onto our character or camera.
// You can check it from UE4: Edit->ProjectSetting->Engine->Input->Bindings

void ABatteryMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind charater to our mouse.
	// Bind BatteryMan z-movement(i.e. "turn") to mouse x-axis. 
	// This means when we move mouse left and right, the character will rotate aroud z-axis.
	// Note: the last param is a function pointer.(Like callback function?)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); 
	// Bind BatteryMan y-movement(i.e. "lookup") to mouse y-axis. 
	// This means when we move mouse up and down, the lookup direction will change.
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind character jump to keyboard space.
	// Note: when press, jump, release, stop jumping.(Here comes the logic stuff. )
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement to the axis.
	// Check the project setting, movements will be bound onto WS(up&down)AD(left&right).
	PlayerInputComponent->BindAxis("MoveForward",this, &ABatteryMan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABatteryMan::MoveRight);
}


// These two functions are also bound to function above.
void ABatteryMan::MoveForward(float Axis)
{
	// Use rotation matrix(around z-axis) to determine the direction of x- and y-axis.
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawDirection(0, Rotation.Yaw, 0);  // ??? isn't yaw the z-direction ???
														  // ??? Which kind of Euler angle does UE4 use ???
														  // ??? x-z-x ???
		// Get the forward direction.
		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void ABatteryMan::MoveRight(float Axis)
{
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawDirection(0, Rotation.Yaw, 0);  

		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}


// Note: you can NOT give it a random name, because this name will override some function inherited from parent class.
// Otherwise it will NOT work normally.
void ABatteryMan::RestartGame()
{
	// Load the level inside "this".
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

// For the pickable item.
// This function will be bound on member function BeginPlay(). See above.
// The reason we bind it, is because this is the way to trigger this function when we collide with a pickable item.
void ABatteryMan::OnBeginOverlap(UPrimitiveComponent* HitComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Recharge")) {
		//UE_LOG(LogTemp, Warning, TEXT("Collide with"));
		Power += 10.f;
		if (Power > 100.f) Power = 100.f;

		OtherActor->Destroy(); // Destroy the pickable items from the scene after collision(i.e. we picked it up).
	}


}


DEFINE_LOG_CATEGORY_STATIC(TestLog, Log, All);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiThreadTest, "MyTest.PublicTest.MultiThreadTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

	bool FMultiThreadTest::RunTest(const FString& Parameters)
{
	UE_LOG(TestLog, Log, TEXT("Hello"));
	return true;
}
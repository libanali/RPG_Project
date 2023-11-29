// Fill out your copyright notice in the Description page of Project Settings.


#include "Another_Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAnother_Character::AAnother_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.1f;



	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bSoftLockOnActive = false;
	bAutoActivateSoftLock = false;

	SoftLockOnMaxDistance = 800.0f;

	//MinArmLength = 200.0f;
	//MaxArmLength = 350.0f;
	//ArmLengthInterpSpeed = 5.0f;

	bPlayerControlsCamera = true;
	//bShouldUpdateCameraRotation = false;

}

void AAnother_Character::MoveForward(float Axis)
{

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);


}

void AAnother_Character::MoveRight(float Axis)
{



	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);


}

void AAnother_Character::ToggleSoftLockOn()
{


	if (bSoftLockOnActive)

	{

		bSoftLockOnActive = false;

	}

	else

	{

		AEnemy* NearestEnemy = GetNearestEnemy();

		if (NearestEnemy)

		{

			float DistanceToEnemy = FVector::Dist(GetActorLocation(), NearestEnemy->GetActorLocation());

			if (DistanceToEnemy <= SoftLockOnMaxDistance)

			{

				bSoftLockOnActive = true;

				GEngine->AddOnScreenDebugMessage(1, 2.3f, FColor::Blue, TEXT("Soft Lock-On Active"));


			}

		}


	}


}

AEnemy* AAnother_Character::GetNearestEnemy()
{
	TArray<AActor*> FoundActors;

	//FName Enemy;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);

	AEnemy* NearestEnemy = nullptr;
	float NearestDistance = MAX_FLT - 1.0f;

	FVector PlayerLocation = GetActorLocation();


	for (AActor* Actor : FoundActors)

	{

		AEnemy* EnemyActor = Cast<AEnemy>(Actor);

		if (EnemyActor)

		{
			float Distance = FVector::DistSquared(EnemyActor->GetActorLocation(), PlayerLocation);

			if (Distance < NearestDistance)
			{

				NearestDistance = Distance;
				NearestEnemy = EnemyActor;

			}

		}

	}

	return NearestEnemy;
}

// Called when the game starts or when spawned
void AAnother_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnother_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bSoftLockOnActive && bAutoActivateSoftLock)

	{

		AEnemy* NearestEnemy = GetNearestEnemy();

		if (NearestEnemy)
		{

			float DistanceToEnemy = FVector::Dist(GetActorLocation(), NearestEnemy->GetActorLocation());

			if (DistanceToEnemy <= SoftLockOnMaxDistance)
			{

				bSoftLockOnActive = true;

				GEngine->AddOnScreenDebugMessage(1, 2.3f, FColor::Blue, TEXT("Soft Lock-On Active"));


				FVector CameratoTarget = NearestEnemy->GetActorLocation() - CameraBoom->GetComponentLocation();
				CameraBoom->TargetArmLength = CameratoTarget.Size();


			}

			else if (bSoftLockOnActive)
			{
				bSoftLockOnActive = false;
			}
		}
	}

}

// Called to bind functionality to input
void AAnother_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);




	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//	PlayerInputComponent->BindAction("HardLockOn", IE_Pressed, this, &ARPG_Character::ToggleHardLockOn);



	PlayerInputComponent->BindAxis("MoveForward", this, &AAnother_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAnother_Character::MoveRight);




}


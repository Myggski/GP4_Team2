// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "PlayerPawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	PlayerCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Player Collider"));
	MoveComp = CreateDefaultSubobject<UPlayerPawnMovementComponent>(TEXT("MovementComponent"));
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	
	RootComponent = PlayerCollider;
	
	PlayerCollider->SetCollisionResponseToAllChannels(ECR_Block);
	PlayerMesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (CrouchingCurve)
	{
		StandingHalfLenght = PlayerCollider->GetUnscaledCapsuleHalfHeight();
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("CrouchUpdate"));
		TimeLine.AddInterpFloat(CrouchingCurve, ProgressUpdate);
	}
}

void APlayerPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MoveComp->SetPawn(this);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLine.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &APlayerPawn::HandleMoveForward);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &APlayerPawn::HandleMoveRight);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerPawn::HandleJump);
	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerPawn::HandleCrouch);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &APlayerPawn::PitchCamera);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &APlayerPawn::YawCamera);
}

void APlayerPawn::HandleJump()
{
	if (MoveComp->OnGround())
	{
		MoveComp->OnJump.Broadcast();
	}
}

void APlayerPawn::HandleCrouch()
{
	if (!MoveComp->RequestCrouch)
	{
		TimeLine.Play();
		MoveComp->RequestCrouch = true;
		MoveComp->GroundCheckRayLenght = CrouchinghafLenght + 10;
	}
	else
	{
		TimeLine.Reverse();
		MoveComp->RequestCrouch = false;
		MoveComp->GroundCheckRayLenght = StandingHalfLenght + 10;
	}
}

void APlayerPawn::CrouchUpdate(float Alpha)
{
	float HalfHeight = FMath::Lerp(StandingHalfLenght, CrouchinghafLenght, Alpha);
	PlayerCollider->SetCapsuleHalfHeight(HalfHeight);
}

void APlayerPawn::HandleMoveForward(float Value)
{
	MoveComp->PlayerInput.X = PlayerInput.X = FMath::Clamp(Value, -1.0f, 1.0f);
}

void APlayerPawn::HandleMoveRight(float Value)
{
	MoveComp->PlayerInput.Y = PlayerInput.Y = FMath::Clamp(Value, -1.0f, 1.0f);
}

void APlayerPawn::PitchCamera(float Value)
{
	MoveComp->CameraInput.Y = CameraInput.Y = Value;
}

void APlayerPawn::YawCamera(float Value)
{
	MoveComp->CameraInput.X = CameraInput.X = Value;
}





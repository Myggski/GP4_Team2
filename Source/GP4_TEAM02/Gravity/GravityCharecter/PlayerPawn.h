// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class UCurveFloat;

class UPlayerPawnMovementComponent;
UCLASS()
class  GP4_TEAM02_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* PlayerCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PlayerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerPawnMovementComponent* MoveComp;
	
	//Input variables
	FVector PlayerInput;
	FVector CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	
	bool IsGrounded = false;

	void HandleMoveForward(float Value);
	void HandleMoveRight(float Value);
	void HandleJump();
	void HandleCrouch();
	void PitchCamera(float Value);
	void YawCamera(float Value);

	UFUNCTION()
	void CrouchUpdate(float Alpha);
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CrouchingCurve = nullptr;
	UPROPERTY()
	FTimeline TimeLine;
	UPROPERTY()
	float StandingHalfLenght = 90;
	UPROPERTY()
	float CrouchinghafLenght = 40;
};

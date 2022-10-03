// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerPawn.h"

UPlayerPawnMovementComponent::UPlayerPawnMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OnJump.AddDynamic(this, &UPlayerPawnMovementComponent::Jump);
	//MinGroundDotProduct = FMath::Cos(FMath::DegreesToRadians(MaxGroundAngle));
}

void UPlayerPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	AdjustVelocity(DeltaTime);
	UpdateState(DeltaTime);
	InHouse();
	RotateTowardsGravity();
}

void UPlayerPawnMovementComponent::Gravity()
{
	AddForce(GravityDirection.GetSafeNormal() * GravityForce);
}

void UPlayerPawnMovementComponent::RotateTowardsGravity()
{
	const FRotator Rot = FRotationMatrix::MakeFromZX(-GravityDirection, Owner->GetActorForwardVector()).Rotator();
	Owner->SetActorRotation(FMath::Lerp(Owner->GetActorRotation(), Rot, GravityAlignmentSpeed));
}

void UPlayerPawnMovementComponent::AdjustVelocity(float DeltaTime)
{
	InputForward =  Owner->GetActorForwardVector() * PlayerInput.X;
	Inputright = Owner->GetActorRightVector() * PlayerInput.Y;
	DesiredVelocity = (InputForward + Inputright).GetSafeNormal() * MaxMovementSpeed;

	if (OnGround())
	{
		const float MaxSpeedChange = MaxAcceleration * DeltaTime;
		Velocity = FVectorMoveTowards(Velocity, DesiredVelocity, MaxSpeedChange);
	}
	else
	{
		const float MaxSpeedChange = AirStrafeForce * DeltaTime;
		Velocity = FVectorMoveTowards(Velocity, DesiredVelocity * AirSpeedModifier, MaxSpeedChange);
	}
	
	Gravity();
	CollisionCheck(DeltaTime);
	
	if (Velocity.Size() > 100 && OnGround())
	{
		PlayStepSound.Broadcast();
	}
}

void UPlayerPawnMovementComponent::UpdateState(float DeltaTime)
{
	StepsSinceLastOnGround += DeltaTime;
	StepsSinceLastJump += DeltaTime;
	
	if (OnGround() || SnapToGround())
	{
		StepsSinceLastOnGround = 0;
	}
}

void UPlayerPawnMovementComponent::Jump()
{
	AddImpulse(-GravityDirection.GetSafeNormal() * JumpStrength);
	StepsSinceLastJump = 0;
}

void UPlayerPawnMovementComponent::CameraControlls()
{
	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	FRotator NewRotation = Owner->GetActorRotation();
	float dotResult = FVector::DotProduct(FVector(0,0,1), Owner->GetActorUpVector());
	
	if (dotResult > 0)
	{
		NewRotation.Yaw += CameraInput.X;
	}
	else
	{
		NewRotation.Yaw -= CameraInput.X;
	}
	
	Owner->SetActorRotation(NewRotation);
}

//TODO slide along surface and use built in.
void UPlayerPawnMovementComponent::CollisionCheck(float DeltaTime)
{
	float RemainingTime = DeltaTime;
	int Iterations = 0;
	
	while (RemainingTime > 0.f && ++Iterations < 10)
	{
		FHitResult Hit;
		Owner->AddActorWorldOffset(Velocity * RemainingTime, true, &Hit);
		
		if (Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				Owner->AddActorWorldOffset(Hit.Normal * (Hit.PenetrationDepth + 0.1f));
				if (Hit.GetComponent()->IsSimulatingPhysics() && !StandingOnPhysicsObject)
				{
					Hit.GetComponent()->AddImpulse(Velocity.GetSafeNormal() * CollitionForceAddedToPhysicsObjects);
				}
			}
			else
			{
				Velocity = FVector::VectorPlaneProject(Velocity, Hit.Normal);
				RemainingTime -= RemainingTime * Hit.Time;
					if (Hit.GetComponent()->IsSimulatingPhysics() && !StandingOnPhysicsObject)
					{
						Hit.GetComponent()->AddImpulse(Velocity.GetSafeNormal() * CollitionForceAddedToPhysicsObjects);
					}
			}
		}
		else
		{
			break;
		}
	}
}

bool UPlayerPawnMovementComponent::SnapToGround()
{
	if (StepsSinceLastOnGround > 1 || StepsSinceLastJump <= 1)
	{
		return false;
	}
	
	FHitResult OutHit;
	FVector RayStartLocation = Owner->GetActorLocation();
	FVector GroundProbeDistancec = RayStartLocation + GravityDirection.GetSafeNormal() * GroundProbeDistance;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);
	
	if (GetWorld()->LineTraceSingleByChannel(OutHit, RayStartLocation, GroundProbeDistancec, ECC_Visibility, CollisionParams))
	{
		return false;	
	}
	
	float Dot;
	Dot = FVector::DotProduct(Velocity, OutHit.Normal);
	float speed = Velocity.Size();
	if (Dot > 0)
	{
		Velocity = (Velocity - OutHit.Normal * Dot).GetSafeNormal() *  speed;
	}
	return true;
}

bool UPlayerPawnMovementComponent::OnGround()
{
	FHitResult OutHit;
	FVector RayStartLocation = Owner->GetActorLocation();
	FVector EndRay = RayStartLocation + GravityDirection.GetSafeNormal() * GroundCheckRayLenght;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, RayStartLocation, EndRay, ECC_Visibility, CollisionParams))
	{
		StandingOnPhysicsComponent(OutHit);
		return true;
	}
	return  false;
}

bool UPlayerPawnMovementComponent::InHouse()
{
	FHitResult OutHit;
	FVector RayStartLocation = Owner->GetActorLocation();
	FVector EndRay = RayStartLocation + GravityDirection.GetSafeNormal() * 300;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, RayStartLocation, EndRay, ECC_GameTraceChannel2, CollisionParams))
	{
		GravityDirection = -OutHit.Normal;
		return true;
	}
	return false;
}

FVector UPlayerPawnMovementComponent::FVectorMoveTowards(FVector current, FVector target, float maxDistanceDelta)
{
	FVector a = target - current;
	float magnitude = a.Size();
	if (magnitude <= maxDistanceDelta || magnitude == 0.f)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;
}

void UPlayerPawnMovementComponent::AddForce(const FVector& Force)
{
	Velocity += Force * GetWorld()->GetDeltaSeconds();
}

void UPlayerPawnMovementComponent::AddImpulse(const FVector& Impulse)
{
	Velocity += Impulse;
}

void UPlayerPawnMovementComponent::SetToDefaultGravity()
{
	GravityDirection = DefaultGravityDirection;
	GravityForce = DefaultGravityForce;
}

void UPlayerPawnMovementComponent::StandingOnPhysicsComponent(FHitResult Hit)
{
	if (Hit.GetComponent()->IsSimulatingPhysics())
		StandingOnPhysicsObject = true;
	else
		StandingOnPhysicsObject = false;
}

void UPlayerPawnMovementComponent::SetPawn(APawn* Pawn)
{
	Owner = Pawn;
}
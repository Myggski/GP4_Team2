#pragma once
#include "PlayerPawn.h"
#include "Components/ActorComponent.h"
#include  "PlayerPawnMovementComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayStepSound);

UCLASS(Meta = (BlueprintSpawnableComponent))
class UPlayerPawnMovementComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnJump OnJump;

	UPROPERTY(BlueprintAssignable)
	FPlayStepSound PlayStepSound;
	
	UPlayerPawnMovementComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	void AdjustVelocity(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void Jump();
	void CameraControlls();
	void CollisionCheck(float DeltaTime);
	void Gravity();
	UFUNCTION(BlueprintCallable)
	void SetToDefaultGravity();
	UFUNCTION(BlueprintCallable)
	void AddForce(const FVector& Force);
	UFUNCTION(BlueprintCallable)
	void AddImpulse(const FVector& Impulse);
	void SetPawn(APawn* Pawn);
	void StandingOnPhysicsComponent(FHitResult Hit);
	void RotateTowardsGravity();
	void UpdateState(float DeltaTime);
	
	bool SnapToGround();
	FVector FVectorMoveTowards(FVector current, FVector target, float maxDistanceDelta);

	bool OnGround();
	UFUNCTION(BlueprintCallable)
	bool InHouse();
	bool RequestCrouch;
	
	UPROPERTY()
	APawn* Owner;

	UPROPERTY(VisibleAnywhere);
	FVector Velocity;
	FVector DesiredVelocity;
	FVector InputForward;
	FVector Inputright;
	FVector Displacement;
	FVector PlayerInput;
	FVector CameraInput;
	FVector PlayerUpVector;
	FVector PlayerForwardVector;
	
	//Movement
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxMovementSpeed = 400;
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxAcceleration = 1400;
	UPROPERTY(EditAnywhere, Category="Movement")
	float JumpStrength = 600;
	UPROPERTY(EditAnywhere, Category="Movement", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float AirSpeedModifier = 0.5f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float AirStrafeForce = 600;
	
	//Collision
	UPROPERTY(EditAnywhere, Category="Collision")
	float GroundCheckRayLenght = 100;
	UPROPERTY(EditAnywhere, Category="Collision")
	float GroundProbeDistance = 120;
	float StepsSinceLastOnGround;
	float StepsSinceLastJump;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 90))
	float MaxGroundAngle;
	float MinGroundDotProduct;
	UPROPERTY(EditAnywhere, Category="Collision")
	float CollitionForceAddedToPhysicsObjects = 2000;
	bool StandingOnPhysicsObject = false;
	//Gravity
	UPROPERTY(BlueprintReadOnly, Category="Gravity")
	FVector DefaultGravityDirection = FVector(0, 0, -1);
	UPROPERTY(BlueprintReadOnly, Category="Gravity")
	float DefaultGravityForce = 980;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gravity")
	FVector GravityDirection = FVector(0, 0, -1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	float GravityForce = 980;
	UPROPERTY(EditAnywhere, Category="Gravity", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float GravityAlignmentSpeed = 0.2;
};


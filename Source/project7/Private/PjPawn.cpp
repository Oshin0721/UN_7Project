#include "PjPawn.h"
#include "PjPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


APjPawn::APjPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ? 충돌 활성화
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));

	Yaw = 0.0f;
	Pitch = 0.0f;

	bUseControllerRotationYaw = false;
}

void APjPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APjPlayerController* PlayerController = Cast<APjPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APjPawn::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APjPawn::Look
				);
			}
		}
	}

}

void APjPawn::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (MoveInput.IsNearlyZero()) return;

	const FRotator YawRotation = GetActorRotation();

	const FVector Forward = -FRotationMatrix(YawRotation + FRotator(0.f, -90.f, 0.f)).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation + FRotator(0.f, -90.f, 0.f)).GetUnitAxis(EAxis::Y);

	const FVector MoveDir = Forward * MoveInput.Y + Right * MoveInput.X;

	AddActorWorldOffset(MoveDir * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void APjPawn::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("Look X: %f, Y: %f"), LookInput.X, LookInput.Y);

	if (LookInput.IsNearlyZero()) return;

	FRotator CurrentRotation = GetActorRotation();
	Yaw = CurrentRotation.Yaw + LookInput.X;
	Pitch = FMath::Clamp(Pitch + LookInput.Y, -80.f, 80.f);

	SetActorRotation(FRotator(0.f, Yaw, 0.f));
	SpringArmComp->SetRelativeRotation(FRotator(Pitch, 0.f, 0.f));
}
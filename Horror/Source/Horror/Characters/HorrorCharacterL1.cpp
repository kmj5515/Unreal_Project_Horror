// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorCharacterL1.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../Interfaces/Interaction.h"
#include "InputActionValue.h"

// Sets default values
AHorrorCharacterL1::AHorrorCharacterL1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	CameraComponent->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = true;

}

// Called when the game starts or when spawned
void AHorrorCharacterL1::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext != nullptr)
				{
					InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

// Called every frame
void AHorrorCharacterL1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHorrorCharacterL1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (LookAction != nullptr)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorCharacterL1::Look);
		}
		if (MoveAction != nullptr)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorCharacterL1::Move);
		}
		if (JumpAction != nullptr)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHorrorCharacterL1::StartJump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHorrorCharacterL1::StopJump);
		}
		if (InteractAction != nullptr)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHorrorCharacterL1::Interact);
		}
	}
}

void AHorrorCharacterL1::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(-LookAxisValue.Y);
}

void AHorrorCharacterL1::Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxisValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, ControllerRotation.Yaw, 0.0f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveAxisValue.Y);
		AddMovementInput(RightDirection, MoveAxisValue.X);
	}
}

void AHorrorCharacterL1::StartJump(const FInputActionValue& Value)
{
	Jump();
}

void AHorrorCharacterL1::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void AHorrorCharacterL1::Interact(const FInputActionValue& Value)
{
	if (CameraComponent == nullptr)
	{
		return;
	}

	const FVector TraceStart = CameraComponent->GetComponentLocation();
	const FVector TraceEnd = TraceStart + (CameraComponent->GetForwardVector() * InteractionTraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(InteractionTrace), false, this);

	const bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (!bHasHit)
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		return;
	}

	if (HitActor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
	{
		IInteraction::Execute_Interact(HitActor, this);
	}
}


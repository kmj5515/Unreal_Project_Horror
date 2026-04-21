// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorCharacterL1.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

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
}


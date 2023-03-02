#include "FirstLevelPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"


AFirstLevelPlayer::AFirstLevelPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFirstLevelPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc)
	{
		auto localPlayer = pc->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_Input, 0);
		}
	}

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
}

void AFirstLevelPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirstLevelPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (inputSystem)
	{
		inputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::Turn);
		inputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::Move);
	}
}

void AFirstLevelPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddControllerYawInput(Axis.X * 0.5f);
	AddControllerPitchInput(-Axis.Y * 0.5f);
}

void AFirstLevelPlayer::Move(const FInputActionValue& Values)
{
	FVector2D axis = Values.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *axis.ToString());
	mainCharacter->AddMovementInput(FVector(1, 0, 0), axis.X);
	mainCharacter->AddMovementInput(FVector(0, 1, 0), axis.Y);

	mainCharacter->SetActorRelativeRotation(FVector(axis.X, axis.Y, 0).Rotation());

	//curRot = FVector(axis.X, axis.Y, 0).Rotation();
	//FRotator lerpRot = FMath::Lerp(prevRot, curRot, 0.2f);
	//mainCharacter->SetActorRelativeRotation(curRot);
	//prevRot = curRot;
}
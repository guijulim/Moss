// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnim.h"
#include "Maincharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Potal.h"


void UMainCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//�÷��̾��� �̵��ӵ��� ������ speed�� �Ҵ�
	auto ownerPawn = TryGetPawnOwner();
	//2. �÷��̾�� ĳ����
	auto player = Cast<AMainCharacter>(ownerPawn);
	if (player)
	{
		//3. �̵��ӵ� �ʿ�
		FVector velocity = player->GetVelocity();
		//4. �÷��̾��� ���� ���Ͱ� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		//5. speed�� �� �Ҵ��ϱ�
		speed = FVector::DotProduct(forwardVector, velocity);
		//6. �¿� �ӵ� �Ҵ��ϱ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		//�÷��̾ ���߿� �ִ��� 
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UMainCharacterAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

void UMainCharacterAnim::PlayFinishAnim()
{
	Montage_Play(finishAnimMontage);
}

void UMainCharacterAnim::PlayMagicAnim()
{
	Montage_Play(magicAnimMontage);
}

void UMainCharacterAnim::PlayDeathAnim()
{
	Montage_Play(DeathMontage);
}

void UMainCharacterAnim::AnimNotify_FinishEnd()
{
	APotal* portal = Cast<APotal>(UGameplayStatics::GetActorOfClass(GetWorld(), APotal::StaticClass()));
	if (portal)
	{
		portal->PlayGateAnimation();
		portal->isPlayEndingAnimation = false;
		//FTimerHandle timer;
		//FTimerDelegate timerDelegate;
		//timerDelegate.BindLambda([&]() -> void {
		//	portal->isPlayEndingAnimation = false;
		//});

		//GetWorld()->GetTimerManager().SetTimer(timer, timerDelegate, 0.5, false);
	}
}

void UMainCharacterAnim::AnimNotify_MagicEnd()
{
	isMagicEnd=true;
}

void UMainCharacterAnim::AnimNotify_DeathEnd()
{
	isDeathEnd = true;
	UGameplayStatics::OpenLevel(this, TEXT("Three"));

}

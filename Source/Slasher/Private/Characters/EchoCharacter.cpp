#include "Characters/EchoCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

AEchoCharacter::AEchoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");


}

void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AEchoCharacter::MoveForward(float Value)
{
	if (ActionState == EActionState::EAS_Attacking)
	{
		return;
	}
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AEchoCharacter::Turn(float Value)
{
	if (Controller && (Value != 0))
	{
		AddControllerYawInput(Value);
	}
}

void AEchoCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AEchoCharacter::MoveRight(float Value)
{
	if (ActionState == EActionState::EAS_Attacking)
	{
		return;
	}
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void AEchoCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void AEchoCharacter::Attack()
{
	bool bCanAttack = ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
	if (bCanAttack)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}

}

void AEchoCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage, 1.5);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();
		switch (Selection)
		{
			case 0:
				SectionName = FName("Attack1");
				break;
			case 1:
				SectionName = FName("Attack2");
				break;
			case 2:
				SectionName = FName("Attack3");
				break;
			default:
				break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}
void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEchoCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEchoCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEchoCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEchoCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &AEchoCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AEchoCharacter::Attack);

}


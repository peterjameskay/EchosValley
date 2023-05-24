#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "EchoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class SLASHER_API AEchoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEchoCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Callbacks for input
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveRight(float Value);
	void EKeyPressed();
	void Attack();

	// Play Montage Functions
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};

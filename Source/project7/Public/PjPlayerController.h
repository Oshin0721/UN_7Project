#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PjPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECT7_API APjPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APjPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

protected:
	virtual void BeginPlay() override;
};

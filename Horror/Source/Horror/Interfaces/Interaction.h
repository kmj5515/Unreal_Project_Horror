#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interaction.generated.h"

UINTERFACE(BlueprintType)
class HORROR_API UInteraction : public UInterface
{
	GENERATED_BODY()
};

class HORROR_API IInteraction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APawn* InteractorPawn);
};

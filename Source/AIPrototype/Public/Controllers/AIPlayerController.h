#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIPlayerController.generated.h"

/**
 * @brief Represents unique non-human player. Logically it's the most authoritative entity regarding AI player, just like APlayerController for human player.
 */
UCLASS()
class AIPROTOTYPE_API AAIPlayerController : public AAIController
{
	GENERATED_BODY()
};

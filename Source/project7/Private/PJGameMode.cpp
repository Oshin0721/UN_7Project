#include "PJGameMode.h"
#include "PjPawn.h"
#include "PjPlayerController.h"

APJGameMode::APJGameMode()
{
	DefaultPawnClass = APjPawn::StaticClass();
	PlayerControllerClass = APjPlayerController::StaticClass();
}


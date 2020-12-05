// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "assignment_2GameMode.h"
#include "assignment_2Character.h"
#include "UObject/ConstructorHelpers.h"

Aassignment_2GameMode::Aassignment_2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

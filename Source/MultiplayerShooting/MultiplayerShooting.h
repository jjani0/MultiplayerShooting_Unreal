// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ENUM_TO_INT32(Value) (int32)Value
// index: weapon index, count: total num of weapons
#define GET_CIRCULAR_ARRAY_INDEX(Index, Count) (Index % Count + Count) % Count
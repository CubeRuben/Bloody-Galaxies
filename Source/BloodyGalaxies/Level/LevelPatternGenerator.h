#pragma once

#include "CoreMinimal.h"
#include "RoomEnums.h"

struct BLOODYGALAXIES_API FOtherData
{
	FOtherData(float x, float y, float zRotation);

	FVector Offset;

	FRotator Rotation;
};

struct BLOODYGALAXIES_API FRoomData
{
	FRoomData(ERoomType type, float zRotation);

	FRoomData SetOffset(int x, int y);

	ERoomType Type;

	FVector Offset;

	FRotator Rotation;

	TArray<FOtherData> DoorsData;
};

struct BLOODYGALAXIES_API FLevelData
{
	TArray<FRoomData> RoomsData;

	TArray<FOtherData> CorridorsData;
};

class BLOODYGALAXIES_API FLevelPatternGenerator
{

	struct Cell
	{
		Cell();

		bool top, right, bottom, left;

		FRoomData GetRoomData(float x, float y);

		TArray<FOtherData> GetDoorsData();

		TArray<FOtherData> GetCorridorsData(float x, float y);
	};

	FIntPoint Size;

public:

	struct Border
	{
		Border();
		Border(const Border& other);
		Border(int x, int y, int ox, int oy);

		FIntPoint first, second;

		bool IsToRight();
		bool IsToBottom();

		friend bool operator==(const Border& left, const Border& right) { return left.first == right.first && left.second == right.second; }

		Border& operator=(Border other)
		{
			first = other.first;
			second = other.second;

			return *this;
		}
	};

	FLevelPatternGenerator(int x, int y);

	FLevelData Generate(int seed, float minDensity);
};

typedef FLevelPatternGenerator::Border FLevelBorder;
#include "LevelPatternGenerator.h"

#include "Room.h"

FLevelPatternGenerator::Border::Border()
{
	first = FIntPoint(-1, -1);
	second = FIntPoint(-1, -1);
}

FLevelPatternGenerator::Border::Border(const Border& other)
{
	first = other.first;
	second = other.second;
}

FLevelPatternGenerator::Border::Border(int x, int y, int ox, int oy)
{
	first = FIntPoint(x, y);
	second = FIntPoint(ox, oy);
}

bool FLevelPatternGenerator::Border::IsToRight()
{
	return first.X - second.X < 0;
}

bool FLevelPatternGenerator::Border::IsToBottom()
{
	return first.Y - second.Y < 0;
}

FRoomData::FRoomData(enum ERoomType type, float zRotation)
{
	Type = type;
	Rotation = FRotator(0, zRotation, 0);
}

FRoomData FRoomData::SetOffset(int x, int y)
{
	Offset = FVector(x, y, 0);

	return *this;
}

FOtherData::FOtherData(float x, float y, float zRotation)
{
	Offset = FVector(x, y, 0);
	Rotation = FRotator(0, zRotation, 0);
}

FLevelPatternGenerator::Cell::Cell()
{
	top = true;
	right = true;
	bottom = true;
	left = true;
}

FRoomData FLevelPatternGenerator::Cell::GetRoomData(float x, float y)
{
	static FRoomData roomsData[] = {
		FRoomData(ERoomType::XIntersection, 45),	//	0	
		FRoomData(ERoomType::DeadEnd, 0),			//	1
		FRoomData(ERoomType::DeadEnd, 90),			//	2
		FRoomData(ERoomType::Curve, 0),				//	3
		FRoomData(ERoomType::DeadEnd, 180),			//	4
		FRoomData(ERoomType::Straight, 0),			//	5
		FRoomData(ERoomType::Curve, 90),			//	6
		FRoomData(ERoomType::TIntersection, 90),	//	7
		FRoomData(ERoomType::DeadEnd, 270),			//	8
		FRoomData(ERoomType::Curve, 270),			//	9
		FRoomData(ERoomType::Straight, 90),			//	10
		FRoomData(ERoomType::TIntersection, 0),		//	11
		FRoomData(ERoomType::Curve, 180),			//	12
		FRoomData(ERoomType::TIntersection, 270),	//	13
		FRoomData(ERoomType::TIntersection, 180),	//	14
		FRoomData(ERoomType::XIntersection, 0)		//	15
	};

	return roomsData[(int)top + (int)right * 2 + (int)bottom * 4 + (int)left * 8].SetOffset(x, y);
}

TArray<FOtherData> FLevelPatternGenerator::Cell::GetDoorsData()
{
	TArray<FOtherData> roomsData;

	if (top)
	{
		roomsData.Add(FOtherData(0, -0.5, 180));
	}

	if (bottom)
	{
		roomsData.Add(FOtherData(0, 0.5, 0));
	}

	if (left)
	{
		roomsData.Add(FOtherData(-0.5, 0, 90));
	}

	if (right)
	{
		roomsData.Add(FOtherData(0.5, 0, 270));
	}

	return roomsData;
}

TArray<FOtherData> FLevelPatternGenerator::Cell::GetCorridorsData(float x, float y) 
{
	TArray<FOtherData> corridorsData;

	if (top)
	{
		corridorsData.Add(FOtherData(x, y - 0.5, 90));
	}

	if (bottom)
	{
		corridorsData.Add(FOtherData(x, y + 0.5, 90));
	}

	if (left)
	{
		corridorsData.Add(FOtherData(x - 0.5, y, 0));
	}

	if (right)
	{
		corridorsData.Add(FOtherData(x + 0.5, y, 0));
	}

	return corridorsData;
}

FLevelPatternGenerator::FLevelPatternGenerator(int x, int y)
{
	Size = FIntPoint(x, y);
}

FLevelData FLevelPatternGenerator::Generate(int seed, float minDensity)
{
	FRandomStream randomStream(seed);

	int iterations = 0;

retry:

	TArray<Border> borders;
	TArray<TSet<FInt32Point>> table;

	for (int x = 0; x < Size.X; x++)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			table.Add(TSet<FIntPoint>{ FIntPoint(x, y) });

			if (x < Size.X - 1)
			{
				borders.Add(Border(x, y, x + 1, y));
			}

			if (y < Size.Y - 1)
			{
				borders.Add(Border(x, y, x, y + 1));
			}
		}
	}

	for (int i = 0; i < borders.Num(); i++)
	{
		int index = randomStream.RandRange(0, borders.Num() - 1);

		if (i != index)
		{
			borders.Swap(i, index);
		}
	}

	int bordersIterationsNum = 60 * (Size.X * (Size.Y - 1) + Size.Y * (Size.X - 1)) / 112;

	for (int i = 0; i < bordersIterationsNum; i++)
	{
		Border border = borders[borders.Num() - 1];

		int first = table.IndexOfByPredicate([border](const TSet<FIntPoint>& el) { return el.Contains(border.first); });
		int second = table.IndexOfByPredicate([border](const TSet<FIntPoint>& el) { return el.Contains(border.second); });

		if (first != second)
		{
			table[first].Append(table[second]);
			table.RemoveAt(second);
		}

		borders.RemoveAt(borders.Num() - 1);
	}

	TArray<Cell> cells;

	cells.SetNum(Size.X * Size.Y);

	for (int x = 0; x < Size.X; x++)
	{
		cells[x].top = false;
	}

	for (int x = 0; x < Size.X; x++)
	{
		cells[x + (Size.X - 1) * Size.Y].bottom = false;
	}

	for (int y = 0; y < Size.Y; y++)
	{
		cells[y * Size.Y].left = false;
	}

	for (int y = 0; y < Size.Y; y++)
	{
		cells[Size.X - 1 + y * Size.Y].right = false;
	}

	for (auto el : borders)
	{
		if (el.IsToRight())
		{
			cells[el.first.X + el.first.Y * Size.Y].right = false;
			cells[el.second.X + el.second.Y * Size.Y].left = false;
		}
		else if (el.IsToBottom())
		{
			cells[el.first.X + el.first.Y * Size.Y].bottom = false;
			cells[el.second.X + el.second.Y * Size.Y].top = false;
		}
	}

	int mazeIndex = 0;

	for (int i = 1; i < table.Num(); i++)
	{
		if (table[i].Num() > table[mazeIndex].Num())
		{
			mazeIndex = i;
		}
	}

	TArray<FRoomData> roomsData;
	TArray<FOtherData> corridorsData;

	for (auto el : table[mazeIndex])
	{
		roomsData.Add(cells[el.X + el.Y * Size.Y].GetRoomData(el.X, el.Y));
		roomsData.Last().DoorsData.Append(cells[el.X + el.Y * Size.Y].GetDoorsData());

		if ((el.X + el.Y) % 2 == 0)
		{
			corridorsData.Append(cells[el.X + el.Y * Size.Y].GetCorridorsData(el.X, el.Y));
		}
	}

	//Pattern verification
	if (iterations < 1000)
	{
		iterations++;

		int roomsCounter[5] = { 0, 0, 0, 0, 0 };

		for (auto el : roomsData)
		{
			roomsCounter[el.Type]++;
		}

		if (roomsData.Num() / (float)(Size.X * Size.Y) < minDensity)
		{
			goto retry;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("FZonePatternGenerator::Generate() - Took %i iterations"), iterations)

	for (int i = 0; i < roomsData.Num(); i++)
	{
		int index = randomStream.RandRange(0, roomsData.Num() - 1);

		if (i != index)
		{
			roomsData.Swap(i, index);
		}
	}

	return FLevelData{ roomsData, corridorsData };
}
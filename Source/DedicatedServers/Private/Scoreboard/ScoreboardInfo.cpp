#include "Scoreboard/ScoreboardInfo.h"

void FScoreboardInfoArray::ModifyKills(FScoreboardInfo& ScoreboardInfo, int32 Amount)
{
	if (Players.Num() > 0)
	{
		int32 Index = Players.Find(ScoreboardInfo);
		Players[Index].Kills += Amount;
		MarkItemDirty(Players[Index]);
		Players[Index].PostReplicatedChange(*this);
	}  
}

void FScoreboardInfoArray::ModifyHeadShots(FScoreboardInfo& ScoreboardInfo, int32 HeadShots)
{
	if (Players.Num() > 0)
	{
		int32 Index = Players.Find(ScoreboardInfo);
		Players[Index].HeadShots += HeadShots;
		MarkItemDirty(Players[Index]);
		Players[Index].PostReplicatedChange(*this);
	}  
}

void FScoreboardInfoArray::ModifyDeaths(FScoreboardInfo& ScoreboardInfo, int32 Amount)
{
	if (Players.Num() > 0)
	{
		int32 Index = Players.Find(ScoreboardInfo);
		Players[Index].Deaths += Amount;
		MarkItemDirty(Players[Index]);
		Players[Index].PostReplicatedChange(*this);
	}  
}

void FScoreboardInfoArray::SetUsername(const FScoreboardInfo& ScoreboardInfo, const FString& Username)
{
	if (Players.Num() > 0)
	{
		int32 Index = Players.Find(ScoreboardInfo);
		Players[Index].Username = Username;
		MarkItemDirty(Players[Index]);
		Players[Index].PostReplicatedChange(*this);
	} 
}

bool FScoreboardInfoArray::FindPlayer(const FString& Username)
{
	for (int32 PlayersIndex = 0; PlayersIndex < Players.Num(); ++PlayersIndex)
	{
		FScoreboardInfo& PlayerOnScoreboard = Players[PlayersIndex];
		if (PlayerOnScoreboard.Username == Username)
		{
			return true;
		}
	}

	return false;
}

void FScoreboardInfoArray::AddPlayer(const FScoreboardInfo& NewPlayerScoreboardInfo)
{
		int32 Index = Players.Add(NewPlayerScoreboardInfo);
		MarkItemDirty(Players[Index]);
		Players[Index].PostReplicatedAdd(*this);
}

void FScoreboardInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 PlayersIndex = 0; PlayersIndex < Players.Num(); ++PlayersIndex)
	{
		FScoreboardInfo& PlayerOnScoreboard = Players[PlayersIndex];
		if (PlayerOnScoreboard.Username == Username)
		{
			PlayerOnScoreboard.PreReplicatedRemove(*this);
			Players.RemoveAtSwap(PlayersIndex);
			MarkArrayDirty();
			break;
		}
	}
	
}


﻿#include "CardGameMode.h"

#include "CardGameLogCategory.h"
#include "CardGamePlayerState.h"

ACardGameMode::ACardGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerStateClass = ACardGamePlayerState::StaticClass();
}

void ACardGameMode::AddCardToPlayerCardPile(AController* Player, UCardGameCardPile* CardPileClass,
	UCardGameCard* CardClass)
{
	if (!IsValid(Player))
	{
		return;
	}

	ACardGamePlayerState* PlayerState = Player->GetPlayerState<ACardGamePlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	Model.AddCardToPlayerCardPile(PlayerState->GetPlayerIndex(), CardPileClass, CardClass);
}

void ACardGameMode::ShufflePlayerCardPile(AController* Player, UCardGameCardPile* CardPileClass)
{
	if (!IsValid(Player))
	{
		return;
	}

	ACardGamePlayerState* PlayerState = Player->GetPlayerState<ACardGamePlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	Model.ShufflePlayerCardPile(PlayerState->GetPlayerIndex(), CardPileClass);
}

void ACardGameMode::MoveCardBetweenPlayerPiles(AController* Player, UCardGameCardPile* From, UCardGameCardPile* To, int32 CardIndex)
{
	if (!IsValid(Player))
	{
		return;
	}

	ACardGamePlayerState* PlayerState = Player->GetPlayerState<ACardGamePlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	Model.MoveCardBetweenPlayerCardPiles(PlayerState->GetPlayerIndex(), From, To, CardIndex);
}

void ACardGameMode::SetPlayerReady(AController* Player)
{
	if (!IsValid(Player))
	{
		return;
	}

	ACardGamePlayerState* PlayerState = Player->GetPlayerState<ACardGamePlayerState>();

	if (!IsValid(PlayerState) || PlayerState->IsReady())
	{
		return;
	}

	PlayerState->SetReady();

	// Check if all players ready.
	int32 ReadyPlayers = 0;

	for (ACardGamePlayerState* P : Players)
	{
		if (IsValid(P) && P->IsReady())
		{
			++ReadyPlayers;
		}
	}

	if (ReadyPlayers >= NumPlayers)
	{
		NotifyOnPreStartGame();
	}
}

FString ACardGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                     const FString& Options, const FString& Portal)
{
	FString ErrorMessage = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	if (IsValid(NewPlayerController))
	{
		ACardGamePlayerState* NewPlayer = NewPlayerController->GetPlayerState<ACardGamePlayerState>();

		if (IsValid(NewPlayer))
		{
			// Find first available player index.
			int32 NewPlayerIndex = 0;
			bool bPlayerIndexInUse = true;
			
			while (bPlayerIndexInUse)
			{
				bPlayerIndexInUse = IsPlayerIndexInUse(NewPlayerIndex);

				if (bPlayerIndexInUse)
				{
					++NewPlayerIndex;
				}
			}

			// Set player index.
			NewPlayer->SetPlayerIndex(NewPlayerIndex);

			UE_LOG(LogCardGame, Log, TEXT("Set player index of player %s to %d."), *NewPlayer->GetName(), NewPlayerIndex);
			
			// Store player reference.
			Players.Add(NewPlayer);

			// Add player to model.
			Model.AddPlayer(NewPlayerIndex, CardPileClasses);
		}
	}

	return ErrorMessage;
}

void ACardGameMode::NotifyOnPreStartGame()
{
	UE_LOG(LogCardGame, Log, TEXT("All %d player(s) are ready."), NumPlayers);
	
	ReceiveOnPreStartGame();
}

bool ACardGameMode::IsPlayerIndexInUse(int32 PlayerIndex) const
{
	for (ACardGamePlayerState* ExistingPlayer : Players)
	{
		if (IsValid(ExistingPlayer) && ExistingPlayer->GetPlayerIndex() == PlayerIndex)
		{
			return true;
		}
	}

	return false;
}

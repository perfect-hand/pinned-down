﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CardGameModel.h"

#include "CardGameMode.generated.h"

class AController;

class UCardGameCard;
class UCardGameCardPile;
class ACardGamePlayerState;

UCLASS()
class CARDGAME_API ACardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACardGameMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void AddCardToPlayerCardPile(AController* Player, UCardGameCardPile* CardPileClass, UCardGameCard* CardClass);
	
	UFUNCTION(BlueprintCallable)
	void ShufflePlayerCardPile(AController* Player, UCardGameCardPile* CardPileClass);
	
	UFUNCTION(BlueprintCallable)
	void MoveCardBetweenPlayerPiles(AController* Player, UCardGameCardPile* From, UCardGameCardPile* To, int32 CardIndex = 0);

	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(AController* Player);

protected:
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal) override;

	virtual void NotifyOnPreStartGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnPreStartGame();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<UCardGameCardPile*> CardPileClasses;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumPlayers;
	
	UPROPERTY()
	FCardGameModel Model;
	
	UPROPERTY()
	TArray<ACardGamePlayerState*> Players;

	bool IsPlayerIndexInUse(int32 PlayerIndex) const;
};

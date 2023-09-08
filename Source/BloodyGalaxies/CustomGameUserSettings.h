#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

UCLASS(Blueprintable)
class BLOODYGALAXIES_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UCustomGameUserSettings(const FObjectInitializer& ObjectInitializer);
	
protected:

	UPROPERTY(Config)
	float MouseSensitivity;

	UPROPERTY(Config)
	float HoldTimeToShoot;

	UPROPERTY(Config)
	bool EnabledHoldToShoot;

	UPROPERTY(Config)
	float MusicAudioVolume;

	UPROPERTY(Config)
	float SoundAudioVolume;

public:

	UFUNCTION(BlueprintPure)
	float GetMouseSensitivity() { return MouseSensitivity; }

	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float value) { MouseSensitivity = value; }

	UFUNCTION(BlueprintPure)
	float GetHoldTimeToShoot() { return HoldTimeToShoot; }

	UFUNCTION(BlueprintCallable)
	void SetHoldTimeToShoot(float value) { HoldTimeToShoot = value; }

	UFUNCTION(BlueprintPure)
	float GetMusicAudioVolume() { return MusicAudioVolume; }

	UFUNCTION(BlueprintCallable)
	void SetMusicAudioVolume(float value) { MusicAudioVolume = value; }

	UFUNCTION(BlueprintPure)
	float GetSoundAudioVolume() { return SoundAudioVolume; }

	UFUNCTION(BlueprintCallable)
	void SetSoundAudioVolume(float value) { SoundAudioVolume = value; }

	UFUNCTION(BlueprintPure)
	bool GetEnabledHoldToShoot() { return EnabledHoldToShoot; }

	UFUNCTION(BlueprintCallable)
	void SetEnabledHoldToShoot(bool value) { EnabledHoldToShoot = value; }

};

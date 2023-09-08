#include "CustomGameUserSettings.h"

UCustomGameUserSettings::UCustomGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MouseSensitivity = 1;
	HoldTimeToShoot = 0.25;

	MusicAudioVolume = 1;
	SoundAudioVolume = 1;
}
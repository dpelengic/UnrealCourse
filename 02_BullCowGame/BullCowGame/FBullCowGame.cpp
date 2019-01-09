#pragma once

#include "FBullCowGame.h"
#include <map>

// To make syxtax Unreal friendly
#define TMap std::map
using Fstring = std::string;
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } // Default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

// Set Difficulty, Hidden Words here
TMap <int32, FString> HiddenWords{ { 1, "ant" },{ 2, "fish" },{ 3, "horse" },{ 4, "donkey" },{ 5, "leopard" } };

int32 FBullCowGame::GetAvailableDifficulty()
{
	return HiddenWords.size();
}

bool FBullCowGame::SetDifficulty(int32 Difficulty)
{
	if (HiddenWords.find(Difficulty) == HiddenWords.end())
	{
		return false; // Unavailable difficulty
	}

	const Fstring HIDDEN_WORD = HiddenWords[Difficulty]; // This MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;
	return true;

}

int32 FBullCowGame::GetMaxTries() const 
{  
	return MyHiddenWord.length() * 2; // Max number of tries for X word length
}

void FBullCowGame::Reset()
{
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

// Receives a VALID guess, increments turn and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	// Loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < GetHiddenWordLength(); MHWChar++)
	{
		// Compare letters against the guess
		for (int32 GChar = 0; GChar < GetHiddenWordLength(); GChar++ )
		{
			// If they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) 
			{
				if (MHWChar == GChar) // If they are in the same place	
				{ 
					BullCowCount.Bulls++;
				}
				else 
				{
					BullCowCount.Cows++;
				}
			}
		}
	}
	if (BullCowCount.Bulls == GetHiddenWordLength()) {
		bGameIsWon = true;
	}
	else 
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// Treat 0 and 1 letter words as isograms
	if (Word.length() < 2) { return true; }

	TMap<char, bool> LetterSeen; // Setup our map
	for (auto Letter : Word) // For all letters of the word
	{
		Letter = tolower(Letter); // Handle mixed case
		if (LetterSeen[Letter]) // If the letter is in the map
		{ 
			return false; // We do NOT have an isogram
		} 
		else  // Otherwise
		{ 
			LetterSeen[Letter] = true;  // Add the letter to the map
		}	
	}	
	return true; // For example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word) // For all letters of the word
	{	
		if (!islower(Letter)) // If not a lowercase letter
		{
			return false;
		}
	}
	return true;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const 
{ 
	if (!IsIsogram(Guess))// If the guess isnt an isogram
	{ 
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // If the guess isnt all lowercase
	{ 
		return EGuessStatus::Not_Lowercase;
	}
	else if ( Guess.length() != GetHiddenWordLength()) // If the guess length is wrong
	{  
		return EGuessStatus::Wrong_Length;
	}
	else 
	{
		return EGuessStatus::OK;
	}
	return EGuessStatus::OK; 
} 

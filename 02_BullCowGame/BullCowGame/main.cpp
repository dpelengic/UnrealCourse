/* This is the console executable , that makes use of the BullCow class.
This acts as the view in a MVC pattern, and is responsible for all user interaction. For game logic, see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "FBullCowGame.h"

// To make syxtax Unreal friendly
using FText = std::string;
using int32 = int;

// Function prototypes
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void AssignDifficulty();
void PrintGameSummary();

FBullCowGame BCGame; // Instantiate a new game, which we reuse across plays

// The entry point for our app
int main() 
{
	do {
		PrintIntro();
		PlayGame();
	} while (AskToPlayAgain());
	return 0;
}

void PrintIntro() 
{
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "            }   {         \\___/ " << std::endl;
	std::cout << "  HRRR . . .(> <)         (^ ^) . . . MoooOo " << std::endl;
	std::cout << "     /-------\\ /           \\ /-------\\ " << std::endl;
	std::cout << "    / | BULL |~             ~| COW  | \\ " << std::endl;
	std::cout << "   *  |-,----|               |--+++-|  * " << std::endl;
	std::cout << "      ^      ^               ^      ^ " << std::endl;
	std::cout << "Can you guess the isogram I'm thinking of?\n\n";
	return;
}

// Plays a single game to completion
void PlayGame() 
{
	AssignDifficulty();
	
	BCGame.Reset();
	
	int32 MaxTries = BCGame.GetMaxTries();

	FText Guess;
	/* Loop asking for guesses while the game
	   is NOT won and there are still tries remaining */
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();

		// Submit valid guess to the game and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << std::endl << std::endl;
	}
	PrintGameSummary();
	return;
}

// Loop until the user gets a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	std::cout << "Word length: " << BCGame.GetHiddenWordLength() << std::endl;
	do {
		// Get guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// Check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n";
			break;
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n";
			break;
		default:
			// Assume the guess is valid
			break;
		}
		std::cout << std::endl;
	} while (Status != EGuessStatus::OK); // Keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain() 
{
	std::cout << "Do you want to play again ? y/(n)";
	FText Response = "";
	std::getline(std::cin, Response);
	return (tolower(Response[0]) == 'y');
}

// This function first asks player for difficulty, and then calls SetDifficulty method
void AssignDifficulty()
{
	int32 Difficulty;
	bool IsDifficultyValid = false;
	do {
		std::stringstream Stream;
		std::cout << "Please choose difficulty: " << std::endl;
		std::cout << "Valid choices: 1 - " << BCGame.GetAvailableDifficulty() << std::endl;
		FText Response;
		// Get input string and convert it to a number
		std::getline(std::cin, Response);
		Stream << Response;
		Stream >> Difficulty;
		IsDifficultyValid = BCGame.SetDifficulty(Difficulty); // Check if difficulty is valid, and if yes, set it
	} while (!IsDifficultyValid); 
	return;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon()) 
	{
		std::cout << "Well done - YOU WIN!\n\n"; // Winner
	} 
	else 
	{
		std::cout << "Better luck next time!\n\n"; // Loser
	}
	return;
}

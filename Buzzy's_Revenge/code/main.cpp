/* 
Author: Sumedh Ravi
Class: ECE 6122
Last Date Modified: 10/24/2021

Description:

Buzzy's revenge
*/

#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include <random>       
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

const std::string titleText = "Buzzy's Revenge";
const float windowWidth = 1920;
const float windowHeight = 1080;
const float targetStartX = windowWidth - 320;
const float creatureStartY = windowHeight - 670;
const float targetWidth = 120;
const float targetHeight = 130.0;
const float powerBarStartWidth = 400;
const float powerBarHeight = 40;

// Enum to denote Target Type
enum TargetType {Creature, EvilMascot, Unicorn};
// Font element for text objects
Font font;

/* 
Prepares text sprite object.

@param textElement The text object to be modified.
@param text Text to be set to the text sprite
@param characterSize Size of the text (default is 30)
@param fillColor Color of the text(default is 30)
@param font Font object for the text to be set (default is KOMIKAP).
*/
void prepareTextElement(Text& textElement, const std::string text, float characterSize = 30, Color fillColor = Color::White, const Font& font = font)
{
	textElement.setFont(font);
	textElement.setString(text);
	textElement.setCharacterSize(characterSize);
	textElement.setFillColor(fillColor);
}

/* 
Prepares instruction text objects to be show at game start.

@param elements Vector of pointers to the text objects.
@param textStrings Vector of string objects to be shown.
@param window Reference to the window object.
*/
float showInstructionTextElements(std::vector<Text*>& elements, const std::vector<std::string>& textStrings, RenderWindow& window)
{
	// To keep track of the bottom edge of the preceding text element.
	float previousElementBottom{0.0};

	// Prepare the title Object which is the first element
	FloatRect textRect;
	prepareTextElement(*elements[0], textStrings[0],75, Color::Red);
	textRect = elements[0]->getLocalBounds();
	elements[0]->setOrigin(textRect.left + textRect.width / 2.0f,
						   	textRect.top + textRect.height / 2.0f);
	elements[0]->setPosition(windowWidth/2.0f, 200);
	window.draw(*elements[0]);
	previousElementBottom = elements[0]->getPosition().y+textRect.height/2 +80;

	// Prepare the remaining text elements
	for (int i=1; i<elements.size(); i++)
	{
		prepareTextElement(*elements[i], textStrings[i],50);
		textRect = elements[i]->getLocalBounds();
		elements[i]->setOrigin(textRect.left + textRect.width / 2.0f,
								textRect.top + textRect.height / 2.0f);
		elements[i]->setPosition(windowWidth/2.0f, previousElementBottom + 25 + textRect.height / 2);
		window.draw(*elements[i]);
		previousElementBottom = elements[i]->getPosition().y+textRect.height/2;
	}
	return previousElementBottom;
}

/* 
Prepares the target sprite objects in the game.

@param targetSprites Array of sprite objects of the targets.
@param targetTextures Array of textures to be loaded to the target sprites in the correct order.
@param imagePaths Array of image paths to load into the texture in the current order.
*/
void loadTargetSprites(Sprite (&targetSprites)[10], Texture (&targetTextures)[10],const std::string (&imagePaths)[10])
{
	for (int i = 0; i< 10; i++)
	{
		// Create Texture Object
		Texture textureTarget;
		// Load From Image Path
		textureTarget.loadFromFile(imagePaths[i]);
		targetTextures[i] = textureTarget;
		// Create Sprite Object
		Sprite spriteTarget;
		spriteTarget.setTexture(targetTextures[i]);

		// Resize image of tiger.
		if (i == 8) 
		{
			spriteTarget.setScale(targetWidth/spriteTarget.getLocalBounds().width, 
							targetHeight/spriteTarget.getLocalBounds().height);
		}
		else 
		{
			spriteTarget.setScale(1.1, 1.1);
		}
		targetSprites[i] = spriteTarget;
	}
}

/* 
Method to draw the sprite objects according to the position where they need to be placed.

@param targets Array of sprite objects of the targets.
@param positions Array of positions containing the target loaded in the position denoted by the array index.
@param window Reference to the window object.
*/
void drawTargets(Sprite (&targets)[10], int postions[10], RenderWindow& window)
{
	float startX = targetStartX;
	float startY = creatureStartY;
	FloatRect imageRect;
	for (int i = 0; i<=1; i++)
	{
		for (int j = 0; j<5; j++)
		{
			if (postions[i*5 + j] == -1) {
				continue;
			}
			imageRect = targets[postions[i*5+j]].getLocalBounds();
			targets[postions[i*5+j]].setOrigin(imageRect.left + imageRect.width / 2.0f,
									imageRect.top + imageRect.height / 2.0f);
			targets[postions[i*5+j]].setPosition(targetStartX+i*190, creatureStartY+j*145);
			window.draw(targets[postions[i*5+j]]);
		}
	}
}

/* 
Method to randomly select valid positions for the targets.

@param positions Array to set the generated positions.
@param targetTypes Array of target types of the target objects.
*/
void randomizePositions(int (&positions)[10], const TargetType (&targetTypes)[10]);

int main()
{
	
    // Create a video mode object
	VideoMode vm(windowWidth, windowHeight);

	// Create and open a window for the game
	RenderWindow window(vm, "Buzzy's Revenge");

	// Create a texture to hold the background texture
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	// Background sprite
	Sprite spriteBackground;
	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Prepare Buzzy Object
	const Vector2f beeInitialPosition = Vector2f{120, windowHeight/2};
	Vector2f beeCurrentPosition{beeInitialPosition};
	float beeOrientationAngle = -30;
	Texture textureBee;
	textureBee.loadFromFile("graphics/smallbuzzy.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	FloatRect beeRect = spriteBee.getLocalBounds();
	spriteBee.setOrigin(beeRect.left + beeRect.width / 2.0f, beeRect.top + beeRect.height / 2.0f);
	// spriteBee.setScale(1.3,1.3);
	spriteBee.setPosition(beeInitialPosition);
	spriteBee.setRotation(beeOrientationAngle);

	// Prepare Insect Object
	Texture textureInsect;
	textureInsect.loadFromFile("graphics/insect.png");
	Sprite spriteInsect;
	spriteInsect.setTexture(textureInsect);
	spriteInsect.setScale(-1.2,1.2);
	spriteInsect.setPosition(windowWidth - 300, 170);

	// Create Text Objects
	Text scoreText, livesText,messageText,titleText,escapeText,powerUpText,controlsText,createdByText,authorText,powerText;
	// Load Font
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Vector object to store start screen instructions
	std::vector<std::string> instructionText;
	instructionText.push_back("Buzzy's Revenge");
	instructionText.push_back("Press Enter to Restart Game");
	instructionText.push_back("Press Esc to exit");
	instructionText.push_back("Press Space to Powerup");
	instructionText.push_back("Press up/down arrow to aim");

	// Pointers to instruction message objects
	std::vector<Text*> textElementPointers;
	Text *titleTextPointer;
	titleTextPointer = &titleText;
	textElementPointers.push_back(titleTextPointer);
	Text *messageTextPointer;
	messageTextPointer = &messageText;
	textElementPointers.push_back(messageTextPointer);
	Text *escapeTextPointer;
	escapeTextPointer = &escapeText;
	textElementPointers.push_back(escapeTextPointer);
	Text *powerUpTextPointer;
	powerUpTextPointer = &powerUpText;
	textElementPointers.push_back(powerUpTextPointer);
	Text *controlsTextPointer;
	controlsTextPointer = &controlsText;
	textElementPointers.push_back(controlsTextPointer);

	// Lives Text Object
	prepareTextElement(livesText, "Lives ");
	livesText.setPosition(20,40);

	// Score Text Object
	const std::string scoreString = "Score: ";
	scoreText.setPosition(windowWidth-200, 40);

	//Power Text Object
	prepareTextElement(powerText, "Power");
	FloatRect powerTextRect = powerText.getLocalBounds();
	powerText.setOrigin(powerTextRect.left, powerTextRect.top + powerTextRect.height / 2.0f);
	powerText.setPosition(40, windowHeight-(powerBarHeight/2 + 50));

	// Author Text Object
	prepareTextElement(createdByText, "created by", 20);
	prepareTextElement(authorText, "Sumedh Ravi");

	/*Preparing the sprite objects for the targets*/
	Sprite targetSprites[10] = {};
	Texture targetTextures[10] = {};
	/* Targets mapping
		0: bunny, 1: chicken, 2: dog, 3: frog, 4: Georgia Bulldog
		5: sheep, 6: mouse 7: pig, 8: tiger, 9: angry unicorn
	*/
	std::string targetImagePaths[10] = {"graphics/bunny.png", "graphics/chicken.png", "graphics/dog.png", "graphics/frog.png",
								   "graphics/Georgia_Bulldogs_logo_dog_g.png", "graphics/sheep.png", "graphics/mouse.png",
								   "graphics/pig.png", "graphics/tiger.png", "graphics/angry_unicorn.png"
									};
	loadTargetSprites(targetSprites, targetTextures, targetImagePaths);
	const TargetType targetTypes[10] = {Creature, Creature, Creature, Creature, EvilMascot,
										Creature, Creature, Creature, EvilMascot, Unicorn};

	// Power Bar Container Sprite
	RectangleShape powerBarContainer;
	powerBarContainer.setSize(Vector2f(powerBarStartWidth, powerBarHeight));
	powerBarContainer.setFillColor(Color::Transparent);
	powerBarContainer.setOutlineThickness(9.0f);
	powerBarContainer.setOutlineColor(Color::Black);
	powerBarContainer.setPosition(180, windowHeight-(powerBarHeight + 50));

	// Power Bar Sprite
	RectangleShape powerBar;
	float powerWidth = 0;
	powerBar.setSize(Vector2f(powerWidth, powerBarHeight));
	powerBar.setFillColor(Color::Red);
	powerBar.setPosition(180, windowHeight - (powerBarHeight + 50));

	// Variables to control time itself
	Clock clock;
	const Time refreshInsectTime = seconds(0.5);
	Time elapsed_time{seconds(0)};

	// Variables to control the game state
	bool paused = true;
	bool acceptInput = false;
	bool canPowerUp = true;
	bool canChangeDirection = true;
	bool isPoweringUp = false;
	bool gameOver = false;
	const int maxLives = 5;
	int remainingMascots = 2;
	int livesRemaining = maxLives;
	int score = 0;
	int positions[10]{0,1,2,3,4,5,6,7,8,9};

	// Variables related to the powerBar powerUp speed
	const float powerUpMaxTime = 2;
	const float powerUpSpeed = powerBarStartWidth/powerUpMaxTime;

	/* Variables related to Buzzy's state */
	const float beeMaxRotateAngle = 80;
	// const float beeDownMaxRotateAngle = -85;
	float launchVelocity;
	float velocityX;
	float velocityY;
	const float beeRotateTime = 1;
	const float beeRotateSpeed = beeMaxRotateAngle/beeRotateTime;
	const float beeMinTime = 1.5;
	const float beeMaxTime = 3.5;
	const float beeMaxVelocity = (windowWidth - beeInitialPosition.x)/beeMinTime;
	const float beeMinVelocity = (windowWidth - beeInitialPosition.x)/beeMaxTime;
	const float beeMaxSpeedMetersPerSecond = 18;
	const float accelerationY = 9.8*beeMaxVelocity/beeMaxSpeedMetersPerSecond;
	float deltavelocityX, deltavelocityY;

	/* Variables to control insect's state */ 
	float insectSpeed{};
	Vector2f insectCurrentPosition{windowHeight-300, 180};
	float insectOrientationAngle{0};
	bool insectActive;

	/* Sprite objects to show the remaining lives. */
	std::array<Sprite, maxLives> livesSprites;
	Texture beeLifeTexture;
	beeLifeTexture.loadFromFile("graphics/buzzy_life.png");

	/* Prepare the score text to be shown */
	prepareTextElement(scoreText, scoreString+std::to_string(score));
	/*Randomize positions foe the targets */
	randomizePositions(positions, targetTypes);

	// Prepare Sound for collision
	SoundBuffer lifeLostBuffer;
	lifeLostBuffer.loadFromFile("sound/chop.wav");
	Sound lifeLostSound;
	lifeLostSound.setBuffer(lifeLostBuffer);

	// Prepare Sound for Game over
	SoundBuffer gameOverBuffer;
	gameOverBuffer.loadFromFile("sound/death.wav");
	Sound gameOverSound;
	gameOverSound.setBuffer(gameOverBuffer);

	// Prepare Sound for Points Scored
	SoundBuffer pointsScoredBuffer;
	pointsScoredBuffer.loadFromFile("sound/pickup.wav");
	Sound pointsScoredSound;
	pointsScoredSound.setBuffer(pointsScoredBuffer);

	// Prepare Sound for powering up
	SoundBuffer lifeGainedBuffer;
	lifeGainedBuffer.loadFromFile("sound/powerup.wav");
	Sound lifeGainedSound;
	lifeGainedSound.setBuffer(lifeGainedBuffer);

	/* Method to reset the bee's state */
	auto resetBee = [&beeCurrentPosition, &spriteBee, &beeInitialPosition, &beeOrientationAngle]
	{
		beeCurrentPosition = beeInitialPosition;
		spriteBee.setPosition(beeCurrentPosition);
		beeOrientationAngle = -30;
		spriteBee.setRotation(beeOrientationAngle);
	};

	/* Method to reset the insect */
	auto resetInsect = [&spriteInsect, &insectSpeed, &insectOrientationAngle, &insectCurrentPosition, &insectActive]
	{
		// Insect velocity magnitude
		srand((int)time(0) * 10);
		insectSpeed = (rand() % 50) + 100;

		// Insect starting height
		insectCurrentPosition.x = 2000;
		insectCurrentPosition.y = 170;
		spriteInsect.setPosition(insectCurrentPosition);

		// Insect orientation angle
		srand((int)time(0) * 10);
		insectOrientationAngle = (rand() % 120) - 60;
		spriteInsect.setRotation(insectOrientationAngle);
		insectActive = true;
	};

	/* Reset Targets */
	auto resetTargets = [&positions, &targetTypes, &targetSprites, &window]
	{
		int newarr[10] = {0,1,2,3,4,5,6,7,8,9}; 
		std::copy(newarr, newarr + 10, positions);
		randomizePositions(positions, targetTypes);
	};

	/* Reset Power Bar State */
	auto resetPowerBar = [&powerWidth, &powerBar]
	{
		powerWidth = 0;
		powerBar.setSize(Vector2f(powerWidth, powerBarHeight));
	};

	/* Reset the game for a new attempt */
	auto resetAttempt = [&resetBee, &livesRemaining, &resetPowerBar, &acceptInput]
	{
		resetBee();
		resetPowerBar();
		acceptInput = true;
	};

	/* Reset the game for a new level */
	auto resetLevel = [&resetBee,&resetPowerBar,&resetInsect,&livesRemaining,&remainingMascots,&resetTargets,&acceptInput]
    {
		// Reset power Bar
		resetPowerBar();
		//Restore number of lives
		remainingMascots = 2;
		//Restore bee
		resetBee();
		//Restore targets
		resetTargets();
		// Reset Insect
		resetInsect();
		// Accept Input again
		acceptInput = true;
    };

	/* Set position of lives objects according to number of remaining lives. */
	auto prepareLivesSprites = [&livesSprites, &beeLifeTexture, &maxLives] (int remainingLives)
	{
		for (int i = 0; i<maxLives; i++)
		{
			livesSprites[i].setTexture(beeLifeTexture);
			livesSprites[i].setScale(0.5,0.5);
			if (i >= remainingLives)
			{
				livesSprites[i].setPosition(livesSprites[i].getPosition().x, -300);
			}
			else 
			{
				livesSprites[i].setPosition(120+i*70, 25);	
			}
		}
	};

	/* As long as the window is open this loop keeps running. */
	while (window.isOpen())
	{
		Event event;
		/* Check for event release event. */
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased)
			{
				/* User is done powering up.*/
				if (event.key.code == Keyboard::Space && isPoweringUp)
				{
					// Do not listen for key presses anymore.
					acceptInput = false;
					isPoweringUp = false;
					// Set the launch velocity according to power given.
					launchVelocity = (powerWidth/powerBarStartWidth) * (beeMaxVelocity - beeMinVelocity) + beeMinVelocity;
					velocityX = launchVelocity * cos(beeOrientationAngle*M_PI/180.0);
					velocityY = launchVelocity * sin(beeOrientationAngle*M_PI/180.0);
				}

				/* Restart the game. */
				if (event.key.code == Keyboard::Return)
				{
					paused = false;
					isPoweringUp = false;
					gameOver = false;
					// Reset the score
					score = 0;
					prepareTextElement(scoreText, scoreString + std::to_string(score));
					// Reset level
					resetLevel();
					livesRemaining = maxLives;
				}
			}
		}

		/* Handle Player Input */
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/* To get time since the previous update. */
		Time dt = clock.restart();

		if (acceptInput) 
		{
			// Rotate bee upwards
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				beeOrientationAngle -= beeRotateSpeed*dt.asSeconds();
				beeOrientationAngle = (beeOrientationAngle < -beeMaxRotateAngle) ? -beeMaxRotateAngle : beeOrientationAngle;
				spriteBee.setRotation(beeOrientationAngle);
			}

			// Rotate bee downwards
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				beeOrientationAngle += beeRotateSpeed*dt.asSeconds();				
				beeOrientationAngle = (beeOrientationAngle > beeMaxRotateAngle) ? beeMaxRotateAngle : beeOrientationAngle;
				spriteBee.setRotation(beeOrientationAngle);
			}

			// Start powering up.
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				isPoweringUp = true;
				powerWidth += powerUpSpeed*dt.asSeconds();
				powerWidth = (powerWidth > powerBarStartWidth) ? powerBarStartWidth : powerWidth;
				// Update display of power bar.
				powerBar.setSize(Vector2f(powerWidth, powerBarHeight));
			}
		}

		if (!paused)
		{
			// Update insect if on the screen
			if (insectActive)
			{
				
				elapsed_time += dt;
				if (elapsed_time > refreshInsectTime)
				{
					// Get next randomized direction of the insect
					srand((int)time(0) * 10);
					insectOrientationAngle = (rand() % 120) - 60;
					spriteInsect.setRotation(insectOrientationAngle);	
					elapsed_time = seconds(0);
				}
				// Update insect position
				insectCurrentPosition.y += insectSpeed * sin((180+insectOrientationAngle)*M_PI/180.0)* dt.asSeconds();
				insectCurrentPosition.x += insectSpeed * cos((180+insectOrientationAngle)*M_PI/180.0)*dt.asSeconds();
				spriteInsect.setPosition(insectCurrentPosition);

				// If insect goes off the screen 
				if (spriteInsect.getPosition().x < -100)
				{
					insectActive = false;
				}
			}
		}
		
		/* When the game has started */
		if (!paused and !acceptInput and !gameOver)
		{
			/* Update the position and velocity of the bee */
			deltavelocityY = velocityY * dt.asSeconds()+0.5*accelerationY*pow(dt.asSeconds(), 2);
			beeCurrentPosition.y += deltavelocityY;
			velocityY += accelerationY*dt.asSeconds();
			beeCurrentPosition.x += velocityX*dt.asSeconds();
			spriteBee.setPosition(beeCurrentPosition);
			beeOrientationAngle = atan2(velocityY, velocityX);
			spriteBee.setRotation(beeOrientationAngle * 180.0/M_PI);

			// Check intersection with targets
			for (int i=0; i<10; i++)
			{
				if (positions[i] == -1 or acceptInput)  continue;

				if (targetSprites[positions[i]].getGlobalBounds().intersects(spriteBee.getGlobalBounds())) {
					switch (targetTypes[positions[i]])
					{
						case EvilMascot: 
						{
							// Update score if hitting a mascot.
							score += 25;
							pointsScoredSound.play();
							// Remove the column where the mascot was.
							int column = int(i/5);
							int currentIndex;
							for (int index = 0; index<5; index++)
							{
								currentIndex = column*5+index;
								if (positions[currentIndex] == -1) continue;
								targetSprites[positions[currentIndex]].setPosition(3000,
								targetSprites[positions[currentIndex]].getPosition().y);
								window.draw(targetSprites[positions[currentIndex]]);
								positions[currentIndex] = -1;
							}
							/* Update the number of remaining mascots. */
							remainingMascots -= 1;

							/* If there are no mascots remaining, reset the level. */
							if (remainingMascots == 0)
							{
								resetLevel();
							}
							/* Otherwise reset the window for another attempt.*/
							else 
							{
								resetAttempt();
							}
							break;
						}

						case Creature:
						{
							/* If a woodland creature is hit, reduce a life and reset window for another attempt. */
							livesRemaining -=1;
							lifeLostSound.play();
							resetAttempt();
							break;
						}

						case Unicorn:
						{
							/* Remove unicorn and other objects on top of it in the column should move down */
							/* Index of the unicorn object. */
							int index = i%5;
							/* Column of the unicorn object. */
							int column = i/5;
							/* Rearrange other objects in column*/
							int currentIndex = column*5+index;
							Sprite* collidedTarget = &targetSprites[positions[i]];
							while(index > 0 and positions[i-1] != -1)
							{
								currentIndex = column*5+index;
								targetSprites[positions[currentIndex-1]].setPosition(targetSprites[positions[currentIndex]].getPosition().x, 
								targetSprites[positions[currentIndex]].getPosition().y);
								positions[currentIndex] = positions[currentIndex-1];
								window.draw(targetSprites[positions[currentIndex-1]]);
								index = index - 1;
							}
							/* Remove unicorn object. */
							collidedTarget->setPosition(3000, collidedTarget->getPosition().y);
							positions[column*5+index] = -1;
							/* Add a life, to get a maximum of 5 lives. */
							livesRemaining += 1;
							lifeGainedSound.play();
							livesRemaining = livesRemaining > maxLives ? maxLives : livesRemaining;
							/* Reset window for another attempt. */
							resetAttempt();
							break;
						}
						default: break;
					}
				}
			}

			/* If insect is visible on the screen check if buzzy hits the insect. */
			if (insectActive)
			{
				if (spriteInsect.getGlobalBounds().intersects(spriteBee.getGlobalBounds()))
				{
					/* Add 75 to the score*/
					score += 75;
					pointsScoredSound.play();
					/* Remove the insect from the screen. */
					spriteInsect.setPosition(-100, spriteInsect.getPosition().y);
					insectActive = false;
				}
			}
			
			/* If buzzy moves out of window bounds reset the attempt*/
			if (spriteBee.getPosition().x > windowWidth or spriteBee.getPosition().y > windowHeight)
			{
				resetAttempt();
				livesRemaining -=1;
				lifeLostSound.play();
			}
		}

		/* Prepare the score text element. */
		prepareTextElement(scoreText, scoreString + std::to_string(score));
		/* Clear the window before updating. */ 
		window.clear();
		/* Draw background sprite */
		window.draw(spriteBackground);
		/* Prepare positions of remaining lives objects. */
		prepareLivesSprites(livesRemaining);
		/* Draw remaining lives sprites. */
		for (int i=0; i<maxLives; i++)
		{
			window.draw(livesSprites[i]);
		}

		// Game over 
		if (livesRemaining <= 0)
		{
			if (!gameOver) gameOverSound.play();
			gameOver = true;
			resetBee();
			acceptInput = false;
			prepareTextElement(titleText, "Game Over!", 75);
			FloatRect textRect = titleText.getLocalBounds();
			titleText.setOrigin(textRect.left + textRect.width / 2.0f,
									textRect.top + textRect.height / 2.0f);
			titleText.setPosition(windowWidth/2.0f, 200);
			window.draw(titleText);
		}
		/* Draw buzzy's sprite */
		window.draw(spriteBee);
		/* Draw lives text sprite */
		window.draw(livesText);
		/* Draw score text sprite */
		window.draw(scoreText);
		/* Draw power text sprite */
		window.draw(powerText);
		/* Draw power bar container sprite */
		window.draw(powerBarContainer);
		/* Draw power bar sprite */
		window.draw(powerBar);
		/* Draw insect sprite */
		window.draw(spriteInsect);
		/* Draw target sprites */
		drawTargets(targetSprites, positions, window);

		/* If the game is paused (showing start screen)*/
		if (paused)
		{
			float lastElementBottom = showInstructionTextElements(textElementPointers, instructionText, window);
			// Created By Text Setup
			FloatRect textRect = createdByText.getLocalBounds();
			createdByText.setOrigin(textRect.left + textRect.width / 2.0f,
									textRect.top + textRect.height / 2.0f);
			createdByText.setPosition(windowWidth/2,lastElementBottom + 105 + textRect.height/2);
			window.draw(createdByText);
			//Author text Setup
			lastElementBottom = createdByText.getPosition().y+textRect.height/2;
			textRect = authorText.getLocalBounds();
			authorText.setOrigin(textRect.left + textRect.width / 2.0f,
									textRect.top + textRect.height / 2.0f);
			authorText.setPosition(windowWidth/2,lastElementBottom + 10 + textRect.height/2);
			window.draw(authorText);
		}
		/* Display the objects drawn */
		window.display();
	}

}

void randomizePositions(int (&positions)[10], const TargetType (&targetTypes)[10])
{
	int count = 10;
	bool evilMascotInColumn = false;

	std::shuffle(positions, positions + 10, std::default_random_engine((int)time(0) * 10));

	//Check if selection is valid.
	int index = 0;
	for(; index<(10/2); index++)
	{
		switch (targetTypes[positions[index]])
		{
			case EvilMascot: 
				if (!evilMascotInColumn)
				{
					evilMascotInColumn = true;
				} 
				else
				{
					srand((int)time(0) * 10);
					int choice = (rand() % (10/2)) + (10/2);
					std::swap(positions[index], positions[choice]);
				}
				break;
			default: break;
		}
	}

	if (!evilMascotInColumn)
	{
		for (;index<10;index++)
		{
			if (targetTypes[positions[index]] == EvilMascot) break;
		}
		srand((int)time(0) * 10);
		int choice = (rand() % (10/2));
		std::swap(positions[index], positions[choice]);
	}
}
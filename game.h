#ifndef Game_h
#define Game_h

#include <random>
#include <chrono>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Shapes.h"

#define tetromino_position all_tetrominos.back().position 

struct Game
{
	Game();
	
	// RNDN generator
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist;
	
	// Game methods
	void Update();
	void HandleInput();
	void Render();
	void TextureSetter(int shape_num, sf::Sprite& shape_sprite);
	//Tetrominos methods
	void SpawnNewTetromino();
	void MoveTetromino();
	bool HitOtherTetrominoHorizontal(std::array<sf::Vector2i, 4> tetrominoToCheck);
	bool HitOtherTetrominoVertical(std::array<sf::Vector2i, 4> tetrominoToCheck);
	void ClearLines();
	void AnimateClearLines();
	void Rotate(bool cw);
	void FallDown();
	bool ReachedBottom();
	bool HitTheWall();
	void SaveHiScore();
	void Reset();
	
	void StartRecord(Button b);
	void StopRecord(Button b);
	
	void SpawnNewTetrominoEgg();
	void ColorSetter(Tetromino& tetromino);
	 
	bool egg = false;
	
	
	double tetrominoSpeed = 1;
	double tetrominoSpeedBackUp = 1;
	double tetrominoHorizontalSpeed = 0.07;

	std::vector<Tetromino> all_tetrominos;
	Shapes shapes;
	
	int direction = 0;
	bool game_over = false;
	bool pause = false;
	bool start_screen = true;
	
	bool canSpeedUp = false;
	
	bool speedBonus = false;
	
	int hiScore = 0;
	

	std::map<int, std::vector<std::pair<int, int>>> clearList;
	
	int score = 0;
	int totalScore = 0;
	int level = 1;
	int lines = 0;
	
	double enterSpeed = 200;
	bool lostFocus = false;
	
	sf::Font font;
	sf::Text txt_score;
	sf::Text txt_game_over;
	sf::Text txt_lines;
	sf::Text txt_level;
	sf::Text txt_hiscore;
	
	
	sf::Text txt_cw;
	sf::Text txt_cwAndccw;
	sf::CircleShape indicator;
	
	
	sf::Text txt_pause;
	
	sf::Text txt_enter;
	sf::Clock textAnimationClock;
	std::array<sf::Color, 7> colors;
	bool enterKeyWasReleased = true;
	
	std::vector<sf::Text> enter_sign;
	
	bool rotationKeyWasReleased = true;
	bool leftBtnWasReleased = true;
	bool rightBtnWasReleased = true;
	
	// We use Spacebar to entter Easter Egg mode
	// and this flag prevents newly spawned tetromino from immediatly falling down
	bool enteredEgg = false;
	
	
	bool twoButtons = false;
	bool tButtonWasReleased = true;
	
	sf::Vector2f previous_winSize;
	
	sf::RenderWindow window;
	sf::Event event;
	
	sf::Clock clock;
	sf::Time elapsed;
	
	sf::Clock keyPressClock;
	float keyPressLatency = 300;
	
	sf::Clock animationClock;
	bool animate = false;
	int numOfRowsDeleted;
	
	sf::Clock horizontalMoveClock;
	sf::Time elapsedHorizontalMove;
	

	int window_width = 10;
	int window_height = 16;
	
	// TEXTURES
	sf::Texture red_texture;
	sf::Texture blue_texture;
	sf::Texture green_texture;
	sf::Texture orange_texture;
	sf::Texture pink_texture;
	sf::Texture cyan_texture;
	sf::Texture yellow_texture;

	sf::RectangleShape bar;
	sf::RectangleShape fade;	
};
#endif
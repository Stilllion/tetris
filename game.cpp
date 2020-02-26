#include "game.h"

using namespace sf;

Game::Game() : dist(0, 6)
{
	window.create(sf::VideoMode(512, 512), "Tetris!");
	
	window.setSize(sf::Vector2u(700, 700));
	previous_winSize = static_cast<sf::Vector2f>(window.getSize());
	
	
	window.setVerticalSyncEnabled(true);

	red_texture.loadFromFile("Textures/red.png");

	pink_texture.loadFromFile("Textures/pink.png");
			
	blue_texture.loadFromFile("Textures/blue.png");

	orange_texture.loadFromFile("Textures/orange.png");
	
	yellow_texture.loadFromFile("Textures/yellow.png");
	
	green_texture.loadFromFile("Textures/green.png");

	cyan_texture.loadFromFile("Textures/cyan.png");
	
	bar.setSize(sf::Vector2f(20, 512));
	bar.setFillColor(sf::Color(57, 66, 61, 64));
	bar.setPosition(320, 0);
	
	// Read hi score 
	std::ifstream db;
	
	db.open("hiScore");
	
	db >> hiScore;
	db.close();
	
	
	font.loadFromFile("Old_R.ttf");
	
	txt_hiscore.setFont(font);
	txt_hiscore.setCharacterSize(14);
	txt_hiscore.setFillColor(sf::Color::White);
	txt_hiscore.setString("HiScore \n" + std::to_string(hiScore));
	txt_hiscore.setPosition(355, 350);
	
	
	txt_score.setFont(font);
	txt_score.setCharacterSize(14);
	txt_score.setFillColor(sf::Color::White);
	txt_score.setString("Score \n" + std::to_string(totalScore));
	txt_score.setPosition(355, 250);
	
	txt_lines.setFont(font);
	txt_lines.setCharacterSize(14);
	txt_lines.setFillColor(sf::Color::White);
	txt_lines.setString("Lines \n" + std::to_string(lines));
	txt_lines.setPosition(355, 150);
	
	txt_level.setFont(font);
	txt_level.setCharacterSize(14);
	txt_level.setFillColor(sf::Color::White);
	txt_level.setString("Level \n" + std::to_string(level));
	txt_level.setPosition(355, 50);
	
	txt_game_over.setFont(font);
	txt_game_over.setCharacterSize(28);
	txt_game_over.setFillColor(sf::Color::White);
	txt_game_over.setString("GAME OVER!");
	txt_game_over.setPosition(20, 256);
	
	txt_pause.setFont(font);
	txt_pause.setCharacterSize(28);
	txt_pause.setFillColor(sf::Color::White);
	txt_pause.setString("PAUSE");
	txt_pause.setPosition(100, 256);
	
	txt_enter.setFont(font);
	txt_enter.setCharacterSize(28);
	txt_enter.setFillColor(sf::Color::White);
	txt_enter.setString("ENTER");
	txt_enter.setPosition(70, 256);
	
	for(int i = 0; i < 5; ++i){
		enter_sign.push_back(txt_enter);
	}
	
	enter_sign[0].setString("E");
	enter_sign[1].setString("  n");
	enter_sign[2].setString("    t");
	enter_sign[3].setString("      e");
	enter_sign[4].setString("        r");
	
	
	txt_cw.setFont(font);
	txt_cw.setCharacterSize(10);
	txt_cw.setFillColor(sf::Color::White);
	txt_cw.setString("CW");
	txt_cw.setPosition(355, 430);
	
	txt_cwAndccw.setFont(font);
	txt_cwAndccw.setCharacterSize(10);
	txt_cwAndccw.setFillColor(sf::Color::White);
	txt_cwAndccw.setString("CW\nCCW");
	txt_cwAndccw.setPosition(355, 460);
	
	indicator.setRadius(5);
	indicator.setFillColor(sf::Color::Green);
	indicator.setOutlineColor(sf::Color::Blue);
	indicator.setOutlineThickness(2);
	indicator.setPosition(397, 430);
	
	
	colors[0] = sf::Color::Red;
	colors[1] = sf::Color(255, 20, 147);
	colors[2] = sf::Color(0, 100, 255);
	colors[3] = sf::Color(255, 127, 80);
	colors[4] = sf::Color::Yellow;
	colors[5] = sf::Color::Green;
	colors[6] = sf::Color::Cyan;
	
	
	fade.setSize(sf::Vector2f(320, 512));
	fade.setFillColor(sf::Color(42, 53, 77, 180));
	fade.setPosition(0, 0);
	
	
	tetrominoSpeedBackUp = tetrominoSpeed;
	
	// Random number seed
	mt.seed(std::time(0));
	
	start_screen = true;
	pause = false;
	game_over = false;
	lostFocus = false;
	
	

	clock.restart();
}

// Game methods
void Game::Update()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
		
		if(event.type == sf::Event::Resized){
			
			sf::Vector2f new_size = static_cast<sf::Vector2f>(window.getSize());
		
			if(window.getSize().y != previous_winSize.y){
				new_size.x = new_size.y;
				window.setSize(static_cast<sf::Vector2u>(new_size));
				previous_winSize = static_cast<sf::Vector2f>(window.getSize());
				break;
			}
			
			if(window.getSize().x != previous_winSize.x){
				new_size.y = new_size.x;
				window.setSize(static_cast<sf::Vector2u>(new_size));
				previous_winSize = static_cast<sf::Vector2f>(window.getSize());
				break;
			}
			
		}
		
		if (event.type == sf::Event::LostFocus) {
			if(!start_screen){
				pause = true;				
			}
			lostFocus = true;
		}
		
		if (event.type == sf::Event::GainedFocus) {
			lostFocus = false;
		}
	}
	
	
	if(start_screen){
		if(textAnimationClock.getElapsedTime().asMilliseconds() > enterSpeed){
			for(int i = 0; i < 5; ++i){
				enter_sign[i].setFillColor(colors[i]);
			}
			
			// Swap color forward
			for(int i = 1, j = 0; i < 5; ++i){
				std::swap(colors[j], colors[i]);
			}
			
			textAnimationClock.restart();
		}
		
		if(enterSpeed <= 10){
			egg = true;
			enteredEgg = true;
		}
	}
	
	if((egg && !game_over && !pause) || (!game_over && !start_screen && !pause)){
		
		if( direction != 0 && !HitOtherTetrominoHorizontal(tetromino_position) && !HitTheWall() ){
			if(elapsedHorizontalMove.asSeconds() > tetrominoHorizontalSpeed)
			{
				MoveTetromino();
				elapsedHorizontalMove = horizontalMoveClock.restart();
			}
		} else {
			direction = 0;
		}

		if(!ReachedBottom() && !HitOtherTetrominoVertical(tetromino_position))
		{
			if(elapsed.asSeconds() >= tetrominoSpeed)
			{
				FallDown();
				
				if(speedBonus){
					score += 1;
				}
				
				elapsed = clock.restart();
			}	
		} else {
			
			if(elapsed.asSeconds() >= tetrominoSpeed)
			{
				tetrominoSpeed = tetrominoSpeedBackUp;
				ClearLines();
				AnimateClearLines();
				if(egg){
					SpawnNewTetrominoEgg();					
				} else {
					SpawnNewTetromino();					
				}

				elapsed = clock.restart();
			}	
			
		}	
	}

}

void Game::HandleInput()
{	
	if (!Keyboard::isKeyPressed(Keyboard::T)){
		tButtonWasReleased = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::T) && tButtonWasReleased){
		
		if(twoButtons){
			twoButtons = false;
			indicator.setPosition(397, 430);
		} else {
			twoButtons = true;
			indicator.setPosition(397, 468);
		}
		tButtonWasReleased = false;
	}

	if(game_over){
		if (Keyboard::isKeyPressed(Keyboard::Enter)){
			Reset();
			start_screen = true;
			enterKeyWasReleased = false;
		}
	}
	
	if (!Keyboard::isKeyPressed(Keyboard::Enter)){
		enterKeyWasReleased = true;
	}
	
	if(start_screen && !pause){
		if (Keyboard::isKeyPressed(Keyboard::Enter) && enterKeyWasReleased){
			start_screen = false;
			enterKeyWasReleased = false;
		}
		
		if(!twoButtons)
		{	
			if(Keyboard::isKeyPressed(Keyboard::Down) && enterSpeed >= 10)
			{
				enterSpeed -= 0.8;
			}
			
			if(!Keyboard::isKeyPressed(Keyboard::Down) && enterSpeed <= 200)
			{
				enterSpeed += 0.8;
			}	
		} else {
			
			if(Keyboard::isKeyPressed(Keyboard::Space) && enterSpeed >= 10)
			{
				enterSpeed -= 0.8;
			}
			
			if(!Keyboard::isKeyPressed(Keyboard::Space) && enterSpeed <= 200)
			{
				enterSpeed += 0.8;
			}		
		}
	}


	if (Keyboard::isKeyPressed(Keyboard::Enter) && enterKeyWasReleased && !start_screen){
		if(pause){
			pause = false;
		} else {
			pause = true;
		}
			
		enterKeyWasReleased = false;
	}
	
	
	if(all_tetrominos.empty() || pause){
		return;
	}
	
	if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Up)){
			rotationKeyWasReleased = true;
	}
	
	if(twoButtons){
		if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Down)){
			rotationKeyWasReleased = true;
		}
	}

	if(!twoButtons){
		if (!Keyboard::isKeyPressed(Keyboard::Down)){
			tetrominoSpeed = tetrominoSpeedBackUp;
			speedBonus = false;
			score = 0;
			enteredEgg = false;
			canSpeedUp = true;
		}	
	} else {
		if (!Keyboard::isKeyPressed(Keyboard::Space)){
			tetrominoSpeed = tetrominoSpeedBackUp;
			speedBonus = false;
			score = 0;
			enteredEgg = false;
			canSpeedUp = true;
		}
	}

	
	if(Keyboard::isKeyPressed(Keyboard::Up) && rotationKeyWasReleased){
		// Clockwise
		Rotate(true);
		rotationKeyWasReleased = false;
	}
	
	if(twoButtons){
		if(Keyboard::isKeyPressed(Keyboard::Down) && rotationKeyWasReleased){
			// CounterClockwise
			Rotate(false);
			rotationKeyWasReleased = false;
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Space) && !enteredEgg && canSpeedUp)
		{
			// 0.08
			tetrominoSpeed = 0.015;
			speedBonus = true;
		}
	} else {
		if(Keyboard::isKeyPressed(Keyboard::Down) && !enteredEgg && canSpeedUp)
		{
			// 0.08
			tetrominoSpeed = 0.015;
			speedBonus = true;
		}	
	}
		
	if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Left)){
			direction = 0;
			leftBtnWasReleased = true;
	}
	
	if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Right)){
			direction = 0;
			rightBtnWasReleased = true;
	}
	
	// Key is still being pressed
	if(Keyboard::isKeyPressed(Keyboard::Left) && !leftBtnWasReleased)
	{
		if(keyPressClock.getElapsedTime().asMilliseconds() > keyPressLatency){
			direction = -1;
		}
		leftBtnWasReleased = false;
	}
	
	// Key is still being pressed
	if(Keyboard::isKeyPressed(Keyboard::Right) && !rightBtnWasReleased)
	{
		if(keyPressClock.getElapsedTime().asMilliseconds() > keyPressLatency){
			direction = 1;
		}
		rightBtnWasReleased = false;
	}

	
	if(Keyboard::isKeyPressed(Keyboard::Left) && leftBtnWasReleased)
	{
		direction = -1;
		
		if(!HitOtherTetrominoHorizontal(tetromino_position) && !HitTheWall())
		{
			MoveTetromino();			
		}
		
		leftBtnWasReleased = false;
		keyPressClock.restart();
	}
	
	if(Keyboard::isKeyPressed(sf::Keyboard::Right) && rightBtnWasReleased)
	{ 
		direction = 1;
		
		if(!HitOtherTetrominoHorizontal(tetromino_position) && !HitTheWall())
		{
			MoveTetromino();			
		}
		
		rightBtnWasReleased = false;
		keyPressClock.restart();
	}
}

void Game::Render()
{
	// 41, 49, 52
	window.clear(sf::Color(91, 97, 95));
	
	if(!egg){
		for(int i = 0; i < all_tetrominos.size(); ++i){
		
			for(int j = 0; j < 4; ++j){
				if(all_tetrominos[i].position[j].x >= 0 || all_tetrominos[i].position[j].y >= 0)
				{
					all_tetrominos[i].shape_sprite.setPosition(all_tetrominos[i].position[j].x*32, all_tetrominos[i].position[j].y*32);
					window.draw(all_tetrominos[i].shape_sprite);
				}
			}
		}	
	} else {
		for(int i = 0; i < all_tetrominos.size(); ++i){
		
			for(int j = 0; j < 4; ++j){
				if(all_tetrominos[i].position[j].x >= 0 || all_tetrominos[i].position[j].y >= 0)
				{
					all_tetrominos[i].text_shape[j].setPosition(all_tetrominos[i].position[j].x*32, all_tetrominos[i].position[j].y*32);
					window.draw(all_tetrominos[i].text_shape[j]);
				}
			}
		}	
	}

	window.draw(txt_score);
	window.draw(txt_level);
	window.draw(txt_lines);
	window.draw(txt_hiscore);
	window.draw(bar);
	
	window.draw(txt_cw);
	window.draw(txt_cwAndccw);
	window.draw(indicator);
	
	
	if(game_over){
		window.draw(fade);
		window.draw(txt_game_over);
	}
	
	if(start_screen){
		for(auto txt : enter_sign){
			window.draw(txt);			
		}
	}
	
	if(pause){
		window.draw(fade);
		window.draw(txt_pause);
	}
	
	window.display();
}

void Game::TextureSetter(int texture_num, sf::Sprite& shape_sprite)
{
	switch(texture_num){
		case 0: shape_sprite.setTexture(red_texture);
		break;
		case 1: shape_sprite.setTexture(pink_texture);
		break;
		case 2: shape_sprite.setTexture(blue_texture);
		break;
		case 3: shape_sprite.setTexture(orange_texture);
		break;
		case 4: shape_sprite.setTexture(yellow_texture);
		break;
		case 5: shape_sprite.setTexture(green_texture);
		break;
		case 6: shape_sprite.setTexture(cyan_texture);
		break;
	}	
}

// Tetromino methods
void Game::SpawnNewTetromino()
{
	int shape_num = dist(mt);
	
	Tetromino tetromino(shapes.getShape(shape_num));
	TextureSetter(shape_num, tetromino.shape_sprite);
	
	tetromino.shape_num = shape_num;
	
	// Change coordinates to spawn new tetromino above the top line
	for(int i = 0; i < 4; ++i){
		tetromino.position[i].y -= 2;
	}
	
	all_tetrominos.push_back(tetromino);
	
	if(HitOtherTetrominoVertical(tetromino.position) || HitOtherTetrominoHorizontal(tetromino.position)){
		game_over = true;
		SaveHiScore();
	}
	
	if(speedBonus){
		totalScore += score;
		score = 0;
	}
	
	canSpeedUp = false;
	
	// Maybe we got a speed bbonus, let's update the score
	txt_score.setString("Score \n" + std::to_string(totalScore));
}

void Game::FallDown()
{
	Tetromino& activeTetromino = all_tetrominos.back();
	
	for(int j = 0; j < 4; ++j){
		activeTetromino.position[j].y += 1;
	}
}

bool Game::ReachedBottom()
{
	if(all_tetrominos.empty()){
		return true;
	}
	
	std::array<sf::Vector2i, 4> potentialPos = all_tetrominos.back().position;

	for(int i = 0; i < 4; ++i){
		
		potentialPos[i].y += 1;
		
		if(potentialPos[i].y >= window_height){
			return true;
		}
	}
	
	return false;
}

bool Game::HitTheWall()
{
	if(all_tetrominos.empty()){
		return true;
	}
	
	std::array<sf::Vector2i, 4> potentialPos = all_tetrominos.back().position;
	
	for(int i = 0; i < 4; ++i){
		
		potentialPos[i].x += direction;
		
		if(potentialPos[i].x >= window_width || potentialPos[i].x < 0){
			return true;
		}
	}
	
	return false;
}

bool Game::HitOtherTetrominoHorizontal(std::array<sf::Vector2i, 4> tetrominoToCheck)
{
	if(all_tetrominos.size() <= 1){
		return false;
	}
	
	std::array<sf::Vector2i, 4> potentialPos = tetrominoToCheck;
	
	// Vertical hit
	for(int i = 0; i < all_tetrominos.size() - 1; ++i){
		for(int j = 0; j < all_tetrominos[i].position.size(); ++j){
			
			potentialPos = tetrominoToCheck;
			
			for(int k = 0; k < potentialPos.size(); ++k){
				
				potentialPos[k].x += direction;
				
				if(potentialPos[k].y == all_tetrominos[i].position[j].y && 
				   potentialPos[k].x == all_tetrominos[i].position[j].x)
				{
					return true;
				}
			}
		}
	}
}

bool Game::HitOtherTetrominoVertical(std::array<sf::Vector2i, 4> tetrominoToCheck)
{
	if(all_tetrominos.size() <= 1){
		return false;
	}
	
	std::array<sf::Vector2i, 4> potentialPos = tetrominoToCheck;
	bool hit = false;
	
	// Vertical hit
	for(int i = 0; i < all_tetrominos.size() - 1; ++i){	
		for(int j = 0; j < all_tetrominos[i].position.size(); ++j){
			
			potentialPos = tetrominoToCheck;
			
			for(int k = 0; k < potentialPos.size(); ++k){
				
				potentialPos[k].y += 1;
				
				if(potentialPos[k].x == all_tetrominos[i].position[j].x && 
				   potentialPos[k].y == all_tetrominos[i].position[j].y)
				{
					return true;
				}
			}
		}
	}
}

void Game::MoveTetromino()
{
	std::array<sf::Vector2i, 4>& currentPos = all_tetrominos.back().position;

	for(int i = 0; i < 4; ++i)
	{
		currentPos[i].x += direction;
	}
	
	direction = 0;
}


void Game::Rotate(bool cw)
{
	// Don't rorate a square
	if(all_tetrominos.back().shape_num == 4){
		return;
	}
	
	sf::Vector2i temp[4];
	std::array<sf::Vector2i, 4> potentialPos;
	
	for(int i = 0; i < 4; ++i){
		if(i != 1){
			temp[i].x = tetromino_position[1].x - tetromino_position[i].x;
			temp[i].y = tetromino_position[1].y - tetromino_position[i].y;
		}
	}
	
	for(int i = 0; i < 4; ++i){
		potentialPos[i] = tetromino_position[i];
		if(i != 1){
			if(cw)
			{
				potentialPos[i].x = (temp[i].x*0) + (temp[i].y*1) + tetromino_position[1].x;
				potentialPos[i].y = (temp[i].x*-1) + (temp[i].y*0) + tetromino_position[1].y;
				
			} else {
				
				potentialPos[i].x = (temp[i].x*0) + (temp[i].y*-1) + tetromino_position[1].x;
				potentialPos[i].y = (temp[i].x*1) + (temp[i].y*0) + tetromino_position[1].y;
				
			}
		}
		
		// Check  out of bounds
		if(potentialPos[i].x > 9 || potentialPos[i].x < 0 ||potentialPos[i].y > 15){
			return;
		}
		
	
	}
	
	// Check for collisions
	
	if(HitOtherTetrominoHorizontal(potentialPos) || HitOtherTetrominoVertical(potentialPos)){
		return;
	}

	for(int i = 0; i < 4; ++i){
		tetromino_position[i] = potentialPos[i];
	}
}

void Game::ClearLines()
{
	if(all_tetrominos.empty() || !clearList.empty()){
		return;
	}
	
	for(int y = window_height; y >= 0; --y){
		for(int x = 0; x < window_width; ++x){
			for(int i = 0 ; i < all_tetrominos.size(); ++i){
				for(int j = 0; j < all_tetrominos[i].position.size(); ++j)
				{
					if(all_tetrominos[i].position[j].x == x && all_tetrominos[i].position[j].y == y)
					{
						std::pair<int, int> tetroAndTile(i, j);
						clearList[y].push_back(tetroAndTile);
					}				
				}
			}
		}			
	}
}


void Game::AnimateClearLines()
{
	if(clearList.empty()){
		return;
	}
	
	int numOfRowsDeleted = 0;
	int rowBelowDeletion;
	
	int leftIndex = 4;
	int rightIndex = 5;
	
	int deletedTiles = 0;
	
	for(auto it = clearList.begin(); it != clearList.end(); ++it){
			
		if(it->second.size() == 10){
			
			canSpeedUp = false;
			
			while(deletedTiles != 10){
			
				if(animationClock.getElapsedTime().asMilliseconds() > 25){
					
					// Right Tile
					int tetrominoIndex = it->second[rightIndex].first;
					int positionIndex = it->second[rightIndex].second;
			
					all_tetrominos[tetrominoIndex].position[positionIndex] = sf::Vector2i(-100, -100);
					
					
					// Left Tile
					tetrominoIndex = it->second[leftIndex].first;
					positionIndex = it->second[leftIndex].second;
			
					all_tetrominos[tetrominoIndex].position[positionIndex] = sf::Vector2i(-100, -100);
				
					deletedTiles += 2;
					
					--leftIndex;
					++rightIndex;
					
					animationClock.restart();
				}
				
				while(window.pollEvent(event)){
					if(event.type == sf::Event::Closed){
						window.close();
					}
				}
				
				HandleInput();
				Render();
			}
	
			numOfRowsDeleted += 1;
			rowBelowDeletion = it->first;	
			
			leftIndex = 4;
			rightIndex = 5;
			
			deletedTiles = 0;
			
			// Drop down tiles
			for(int i = 0; i < all_tetrominos.size(); ++i){
				for(int j = 0; j < all_tetrominos[i].position.size(); ++j){
					
					if(all_tetrominos[i].position[j].y < rowBelowDeletion){
						all_tetrominos[i].position[j].y += numOfRowsDeleted;
						
						if(all_tetrominos[i].position[j].y > 15){
							all_tetrominos[i].position[j].y = 15;
						}
						
					}
				}
			}
			
			// Calculate score
			lines += numOfRowsDeleted;
			totalScore += numOfRowsDeleted * (4 * level);
			
			txt_lines.setString("Lines \n" + std::to_string(lines));
			
			
			if(lines % 8 == 0 && lines != 0){
				
				level += 1;
				
				tetrominoSpeed = tetrominoSpeedBackUp;
				// Speed up a little
				if(level < 10){
					tetrominoSpeed -= 0.07;					
				} else {
					tetrominoSpeed -= 0.02;
				}
				
				tetrominoSpeedBackUp = tetrominoSpeed;
				
				txt_level.setString("Level \n" + std::to_string(level));
			}
				
			// Bonus for TETRIS
			if(numOfRowsDeleted == 4){
				totalScore += 16;
			}
			
			numOfRowsDeleted = 0;		
		}
	}
	
	txt_score.setString("Score \n" + std::to_string(totalScore));
	
	clearList.clear();
}

void Game::SaveHiScore()
{
	if(totalScore > hiScore){
		// Save Hi score
		hiScore = totalScore;
		
		std::ofstream db;	
		db.open("hiScore");
		db << hiScore;
		db.close();
		
		txt_hiscore.setString("HiScore \n" + std::to_string(hiScore));
	}
}

void Game::Reset()
{
	all_tetrominos.clear();
	
	SaveHiScore();
	
	game_over = false;
	totalScore = 0;
	level = 1;
	lines = 0;
	tetrominoSpeed = 1;
	tetrominoSpeedBackUp = 1;
	
	txt_score.setString("Score \n" + std::to_string(totalScore));
	txt_level.setString("Level \n" + std::to_string(level));
	txt_lines.setString("Lines \n" + std::to_string(lines));
	
	enterSpeed = 200;
	egg = false;
	
}

void Game::ColorSetter(Tetromino& tetromino)
{
	for(int i = 0; i < 4; ++i){
		tetromino.text_shape[i].setFillColor(colors[tetromino.shape_num]);
	}
	
}

void Game::SpawnNewTetrominoEgg()
{
	
	int shape_num = dist(mt);
	Tetromino tetromino(shapes.getShape(shape_num));
	tetromino.shape_num = shape_num;
	
	
	ColorSetter(tetromino);
	
	// Change coordinates to spawn new tetromino above the top line
	for(int i = 0; i < 4; ++i){
		tetromino.position[i].y -= 1;
		tetromino.text_shape[i].setFont(font);
	}
	
	all_tetrominos.push_back(tetromino);
	
	if(HitOtherTetrominoVertical(tetromino.position) || HitOtherTetrominoHorizontal(tetromino.position)){
		game_over = true;
		
		SaveHiScore();
	}
	
	if(speedBonus){
		totalScore += score;
		score = 0;
	}
	
	// Maybe we got a speed bbonus, let's update the score
	txt_score.setString("Score \n" + std::to_string(totalScore));
	
	start_screen = false;
	canSpeedUp = false;
}
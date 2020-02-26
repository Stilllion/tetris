#ifndef shapes_h
#define shapes_h
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iterator>
#include <vector>
#include  <array>
#include <iostream>

struct Shapes{

	/*int Z_shape[4][2] =	{1, 0, 1, 1, 0, 1};
	int L_shape[4][2] = {1, 0, 1, 0, 1, 1};	
	int S_shape[4][2] = {0, 1, 1, 1, 1, 0};
	int I_shape[4][2] = {1, 0, 1, 0, 1, 0, 1, 0};
	int O_shape[4][2] = {1, 1, 1, 1, 0, 0};
	int J_shape[4][2] = {0, 1, 0, 1, 1, 1};
	int T_shape[4][2] = {0, 1, 1, 1, 0, 1};*/
	
	Shapes();
	// Array holding all the Shapes
	int brick[7][4][2] =
	{
		{1, 0, 1, 1, 0, 1},					// Z_shape
		{2, 0, 2, 0, 2, 2},					// L_shape
		{0, 3, 3, 3, 3, 0},					// S_shape
		{4, 0, 4, 0, 4, 0, 4, 0},			// I_shape
		{5, 5, 5, 5, 0, 0},					// O_shape
		{0, 6, 0, 6, 6, 6},					// J_shape
		{7, 0, 7, 7, 7, 0}					// T_shape
	};
	
	std::array<std::array<sf::Vector2i, 4>, 7> shapes;
	
	std::array<sf::Vector2i, 4> getShape(int i);
};

struct Tetromino
{
	Tetromino(std::array<sf::Vector2i, 4> pos);
		
	sf::Text txt_enter;

	std::array<sf::Vector2i, 4> position;
	sf::Sprite shape_sprite;
	
	std::array<sf::Text, 4> text_shape;
	
	int shape_num = -1;
};


#endif
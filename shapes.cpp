#include "shapes.h"

Shapes::Shapes()
{
	srand(time(0));
	
	for(int k = 0; k < 7; ++k){
		int counter = 0;
		
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 2; ++j)
			{
				if(brick[k][i][j] > 0)
				{
					shapes[k][counter].x = j+4;
					shapes[k][counter].y = i;
					++counter;
				}
			}	
		}
	}
}

std::array<sf::Vector2i, 4> Shapes::getShape(int i)
{
	return shapes[i];
}


Tetromino::Tetromino(std::array<sf::Vector2i, 4> pos) : position(pos)
{	
	txt_enter.setCharacterSize(20);
	txt_enter.setFillColor(sf::Color::White);
	txt_enter.setString("ENTR");
	
	for(int i = 0; i < 4; ++i){
		text_shape[i] = txt_enter;
	}
	
	text_shape[0].setString("E");
	text_shape[1].setString("N");
	text_shape[2].setString("T");
	text_shape[3].setString("R");
}
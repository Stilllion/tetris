#include "Game.h"
int main()
{
	Game g;
	while(g.window.isOpen()){
		
		if(!g.lostFocus){
			g.HandleInput();
		}
		
		g.Update();
		g.Render();
		g.elapsed += g.clock.restart();
		g.elapsedHorizontalMove += g.horizontalMoveClock.restart();
	}
}
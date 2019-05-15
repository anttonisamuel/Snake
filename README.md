# Snake
School project.  A simplified version of the classic Snake-game. 

# How to play

Start playing by clicking the Play-button. The game can be paused and resumed with the Pause/Resume-button and
the window can be closed by clicking the Quit-button. In case of a game-over, tha game can be restarted from
the Play-button.

Easy mode can be selected from the easymode-checkbox. On easy mode, snakes speed stays the same
throughout the game and opposite item is not places on the map.

Walls can be toggled on and off between games from the walls-checkbox. If walls are on, snake dies if it collides
with the wall. If walls are off, the snake goes through the wall an teleports to the opposite side.

If the snake eats food (the red item on the map) the snake grows and snakes movement speed is increased, if playng on
normal mode. If playng on easy mode, snakes speed stays the same.

Maps size can be adjusted from the spin box.

Controls are W, A, S and D and they work like arrow keys. If snake eats and opposite item (the blue item on the map),
controls are reversed, if the snake eats another one, the controls are restored to normal etc.

Snake dies if it eats its own tail or collides with the wall while walls are on.

Snake color gets darker as the tail grows longer (the tip of the tail is darker than the neck). Head stays black.

High score and the current game time is shown also.

# While doing this I learned to....
 - Design and implement a simple GUI using Qt and C++ 
 - Use Qt widgets and graphics

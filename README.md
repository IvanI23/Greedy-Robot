# Greedy-Robot
A project to implement my skills in C. The animation is completely robust and utilises C's memory management capabilities.

Greedy Robot program
A randomly placed Robot moves around randomly in a randomly shaped and sized arena. Robot collects coins (markers) and deposits them at banks (corners). When robot deposited all the markers and then returns home then the program ends.
Program is robust and never gets stuck in a loop as in every step the robot has a 10% chance of changing direction. And when hit obstacle the robots response is also random. Therefore given enough time the program will finish. Whenever the robot randomly turns to the Right Side it drops a coin on the arena.
Markers, Corners and home is protected pre- deployment so will never spawn surrounded by a blocker (Wall or Obstacle). Robot breaks obstacle when it hits it 8 times. When a coin is deposited a dollar picture is popped. Corners are sets randomly at any newly formed corners, if the arenas initial corners are filled up by another type of tile.
Modify speed by changed robot-> speed in the setRobot function in animation.c


Compile :
gcc main.c background.c animation.c DrawApp/graphics.c  
Change graphics.c with your path. Change the two headers at the directives in animation.c and background.c with the path. 
Run:
./a.exe| java -jar DrawApp/drawapp-4.0.jar

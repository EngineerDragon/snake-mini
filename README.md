# Snake Mini
## Very small snake code with C
Some friend challenged me about coding snake in c++ less than 300 lines. So I coded it with C using OpenGL about 150 lines (excluding glsl because I am going to turn it to a playground to test my shaders you can also put your own to test!) 
(I could have stuffed all the code in one line but it would not be fair so I made it readably small)

### How to build :
#### Edit Makefile, put your glad path and run :
```make
make
```
Very informative, I know.

#### If you are going to compile from windows : 
```bat
gcc -std=c99 main.c glad.c -o snake-mini -lglfw3 -lopengl32 -lgdi32
```
I am currently extremely lazy to do a makefile supporting windows as well just use that line.

## Controls : 
W : moves to up \
A : moves to left\
S : moves to down \
D : moves to right \
\
L : closes the game

If you ask why L is game over, its very pure dragon magic knowledge :) 


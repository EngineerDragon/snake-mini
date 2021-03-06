#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static GLFWwindow * window;
struct tile_index 
{
	uint8_t x,y;
};
static unsigned int shader,vao,vbo;
static uint8_t way = 0; // direct key calls 
struct {short size; struct tile_index* position;} snake;
static struct tile_index food;

static void exitfail(const char *message) 
{
	fprintf(stderr,"\e[31;1m%s\e[m",message);
	glfwTerminate();
	exit(1);
}

static char * readfile(const char * rd)
{
	FILE * f = fopen(rd,"r"); fseek(f,0,SEEK_END);
	long ssize = ftell(f); fseek(f,0,SEEK_SET);
	char * ret = malloc(ssize + 1); ret[ssize] = '\0';
	fread(ret,ssize,1,f); fclose(f);
	return ret;
}

static void compile_shaders()
{
	int res;char info[512];
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER),fs = glCreateShader(GL_FRAGMENT_SHADER);
	char * src = readfile("vert.glsl");
	glShaderSource(vs,1,(const char * const *)&src,NULL);
	free(src);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &res);
	if (!res) {glGetShaderInfoLog(vs,512,NULL,info);exitfail(info);}
	src = readfile("frag.glsl");
	glShaderSource(fs,1,(const char * const *)&src,NULL);
	free(src);
	glCompileShader(fs);
	glGetShaderiv(fs,GL_COMPILE_STATUS,&res);
	if (!res) {glGetShaderInfoLog(fs,512,NULL,info); exitfail(info);}
	shader = glCreateProgram();
	glAttachShader(shader,vs); glAttachShader(shader,fs);
	glLinkProgram(shader);
	glGetProgramiv(shader,GL_LINK_STATUS,&res);
	if (!res){glGetProgramInfoLog(shader,512,NULL,info); exitfail(info);}
	glDeleteShader(vs); glDeleteShader(fs);
}

static void initalize_point()
{
	float vrt[] = {0.0f};
	glGenBuffers(1,&vbo); glGenVertexArrays(1,&vao);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vrt),vrt,GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shader);
}

static void generate_food()
{

	uint8_t collide = 0;
	do 
	{
		food.x = rand() % 30; food.y = rand() % 30; collide = 0;
		for (short i = 0;i < snake.size;i++)
			if (snake.position[i].x == food.x && snake.position[i].y == food.y) {collide = 1; break;}
	} while(collide);
}

static void game_tick()
{
	for (short i = snake.size - 2;i != -1;i--)
		snake.position[i + 1] = snake.position[i];
	switch (way)
	{
		case GLFW_KEY_W:
		snake.position->y--;
		break;
		case GLFW_KEY_S:
		snake.position->y++;
		break; 
		case GLFW_KEY_A:
		snake.position->x--;
		break;
		case GLFW_KEY_D:
		snake.position->x++;
		break;
		default:
		printf("WHAT HAVE YOU DONE? I GOT %d",way);
	}
	if (snake.position->x == food.x && snake.position->y == food.y)
	{
		snake.position = realloc(snake.position,++snake.size * sizeof(struct tile_index));
		snake.position[snake.size-1] = (struct tile_index) {254,254};
		generate_food();
	}
	for (short i = 1;i < snake.size;i++) if (snake.position->x == snake.position[i].x && snake.position->y == snake.position[i].y || (snake.position->x >= 30 || snake.position->y >= 30)) 
	{
		printf("\e[31;1mGame Over. Score : %d\n\e[m",snake.size-3);
		glfwSetWindowShouldClose(window,1);
		break;
	}
}
 
static void key_event(GLFWwindow * wi,int key,int scancode,int action,int mods)
{
	long double hded = (2801.0* (long double)key-208993.0)/(37.0 * (long double)key - 2801.0),md = hded - (int)hded;
	if (action == GLFW_PRESS  && hded != way && md == 0) way = key;
}

int main(int argc,char ** argv)
{
	srand(time(NULL));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE,0);
	window = glfwCreateWindow(600,600, "Snake Mini", NULL,NULL);
	if (!window) exitfail("Unable to initalize GLFW window.");
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) exitfail("Unable to initalize GL loader.");
	glfwSetKeyCallback(window,(GLFWkeyfun)&key_event);
	glViewport(0, 0, 600, 600);
	glEnable(GL_PROGRAM_POINT_SIZE); 
	compile_shaders(); // set the shaders up
	glPointSize(20.0f);
	initalize_point();
	way = GLFW_KEY_D;	// snake position
	snake.size = 3;snake.position = malloc(sizeof(struct tile_index) * 3);
	memcpy(snake.position,(uint8_t[]){3,1,2,1,1,1},sizeof(struct tile_index) * 3);
	generate_food();
	// end the shaders
	double del = 0;
	while (!glfwWindowShouldClose(window))
	{	// game loop
		glClear(GL_COLOR_BUFFER_BIT); // no need to loop the shader since there is only one shader and only that will used
		glUniform1f(glGetUniformLocation(shader,"time"),glfwGetTime());
		glUniform1i(glGetUniformLocation(shader,"isfood"),0);
		for (short i = 0;i < snake.size;i++)
		{
			glUniform2i(glGetUniformLocation(shader,"tile"),snake.position[i].x,snake.position[i].y);
			glDrawArrays(GL_POINTS,0,1);	
		}
		glUniform1i(glGetUniformLocation(shader,"isfood"),1);
		glUniform2i(glGetUniformLocation(shader,"tile"),food.x,food.y);
		glDrawArrays(GL_POINTS,0,1);	
		if (glfwGetTime() - del > 0.05) { del += 0.05;game_tick(); } 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}	
	free(snake.position);
	glfwTerminate();
	return 0;
}

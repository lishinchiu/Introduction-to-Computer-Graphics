#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <time.h>
#include <unordered_map>
#include <queue>
#include <conio.h>
#include <vector>

#include "./Library/glut/glut.h"
#include "./Library/SOIL/include/SOIL.h"

#pragma comment( lib, "./Library/glut/glut32.lib" )
#pragma comment( lib, "./Library/SOIL/lib/SOIL.lib" )
using namespace std;

struct datas { int width, height; };
struct point { GLfloat x, y; };

int score;
int time_counter;
const int max_time = 2.0e4;
bool escape,  game_end;
int game_start;
int choose=1;
float position;
GLuint player_texture, coin_texture,background_texture,man_texture,number_texture,score_texture,
		boss_texture,walker_texture,background1_texture,pause_texture,timer_texture,background2_texture;
std::unordered_map<GLuint, datas> image_values;
unsigned char* image;
int width, height, channels;
int stay,stay2=0;
float player_x,man_x;
const float player_y = -0.3,man_y=0.45;
const float player_size = 0.1;
bool move_left, move_right;

int k;
std::queue<point> coins;
const float coin_size = 0.04;
const float coin_step = 0.01;
const int coin_num = 10;

void set_texture(GLuint& texture, const char* file) {
	image = SOIL_load_image(file, &width, &height, &channels, SOIL_LOAD_AUTO);

	if (image) {
		texture = SOIL_create_OGL_texture(image, width, height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

		SOIL_free_image_data(image);

		image_values[texture] = datas{ width, height };
	}
	else {
		printf("SOIL loading error: '%s' \n", SOIL_last_result());
	}
}


void TimerFunc(int value) {
	if (!game_start || game_end) {
		glutTimerFunc(20, TimerFunc, 1);
		return;
	}
	if ((time_counter += 20) == max_time)
		game_end = true;
	for (int i = coins.size(); i > 0; i--) {
		coins.front().y -= coin_step + score * 0.0001;
		float buttom = coins.front().y - coin_size;
		if (buttom < -0.2 && abs(coins.front().x - player_x) < player_size)
			score++;
		else if (coins.front().y - coin_size > -0.5)
			coins.push(coins.front());
		coins.pop();
	}

	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}
void change(int value) {
	k = rand();
	k = k % 2;

	glutTimerFunc(500, change, 5);
}
int time1=0;
void count(int value) {
	time1 += 1;
	glutTimerFunc(1000, count, 6);
}
void Timer_man_control(int value) {

	if (k==1 && man_x > -0.45)
		man_x -= 0.01;
	if (k==0 && man_x < 0.45)
		man_x += 0.01;

	glutPostRedisplay();

	glutTimerFunc(10, Timer_man_control, 4);
}
void Timer_coin_control(int value) {
	if (!game_start || game_end) {
		glutTimerFunc(500, Timer_coin_control, 2);
		return;
	}
	if (coins.size() < coin_num)
		position = (GLfloat)rand() / (GLfloat)(RAND_MAX / 0.96f) - 0.48f;
		coins.push(point{ man_x, 0.45f });
	glutPostRedisplay();
	glutTimerFunc(500, Timer_coin_control, 2);
}

void Timer_player_control(int value) {
	if (!game_start || game_end) {
		glutTimerFunc(10, Timer_player_control, 3);
		return;
	}
	if (move_left && player_x > -0.45)
		player_x -= 0.01;
	if (move_right && player_x < 0.45)
		player_x += 0.01;

	glutPostRedisplay();
	glutTimerFunc(10, Timer_player_control, 3);

}
void SpecialPress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		move_left = true;
		break;
	case GLUT_KEY_RIGHT:
		move_right = true;
		break;
	default:
		break;
	}
}
void SpecialRelease(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		move_left = false;
		break;
	case GLUT_KEY_RIGHT:
		move_right = false;
		break;
	default:
		break;
	}
}

void KeyboardPress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	case 32:
		game_start = 1;
		time1 = stay2;
		break;
	case 112:
		game_start = 2;
		stay = time_counter;
		stay2 = time1;
		break;
	case 49:
		choose = 1;
		break;
	case 50:
		choose = 2;
		break;
	case 51:
		choose = 3;

		break;
	default:
		break;
	}
}
void init() {
	std::srand(std::time(NULL));

	glEnable(GL_TEXTURE_2D);

	set_texture(man_texture, "./Images/man.png");
	set_texture(player_texture, "./Images/player.png");
	set_texture(coin_texture, "./Images/coin.png");
	set_texture(number_texture, "./Images/number.png");
	set_texture(score_texture, "./Images/score.png");
	set_texture(background1_texture, "./Images/background.png"); 
	set_texture(pause_texture, "./Images/pause.png");
	set_texture(boss_texture, "./Images/boss.png");
	set_texture(walker_texture, "./Images/walker.png");
	set_texture(timer_texture, "./Images/timer.png");
	set_texture(background2_texture, "./Images/background2.png");

	glBindTexture(GL_TEXTURE_2D, player_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_DEPTH_TEST);

	glutTimerFunc(20, TimerFunc, 1);
	glutTimerFunc(160, Timer_coin_control, 2);
	glutTimerFunc(10, Timer_player_control, 3);
	glutTimerFunc(500, Timer_man_control, 4);
	glutTimerFunc(500, change, 5);
	glutTimerFunc(1000, count, 5);

	glutSpecialFunc(SpecialPress);
	glutSpecialUpFunc(SpecialRelease);
	glutKeyboardFunc(KeyboardPress);
	time1 = 0;
	score = time_counter = 0;
	move_left = move_right = false;
	escape = game_end = false;
	game_start = 0;
	player_x = 0;
}

void reshape(int w, int h) {
	float aspectRatio = (float)w / (float)h;
	float newWidth, newHeight;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5 * aspectRatio, 0.5 * aspectRatio, -0.5, 0.5);
	glMatrixMode(GL_MODELVIEW);
}
void draw(int number, float x, float y, float xSize, float ySize) {
	float x_left = (float)number / 10;
	float x_right = x_left + 0.1;
	glTexCoord2f(x_left, 0.0); glVertex2f(x - xSize, y - ySize);
	glTexCoord2f(x_right, 0.0); glVertex2f(x + xSize, y - ySize);
	glTexCoord2f(x_right, 1.0); glVertex2f(x + xSize, y + ySize);
	glTexCoord2f(x_left, 1.0); glVertex2f(x - xSize, y + ySize);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float aspectRatio = (float)width / (float)height; 
	
	if (game_end) {
		
	}
	else if (game_start==1) {
		int player_longSide = std::max(image_values[player_texture].width, image_values[player_texture].height);
		float player_xSize = player_size * image_values[player_texture].width / player_longSide;
		float player_ySize = player_size * image_values[player_texture].height / player_longSide;
		int coin_longSide = std::max(image_values[coin_texture].width, image_values[coin_texture].height);
		float coin_xSize = coin_size * image_values[coin_texture].width / coin_longSide;
		float coin_ySize = coin_size * image_values[coin_texture].height / coin_longSide;

		// player
		glBindTexture(GL_TEXTURE_2D, player_texture);
		glBegin(GL_QUADS);
		float x = player_x;
		glTexCoord2f(0.0, 0.0); glVertex2f(x - player_xSize, player_y - player_ySize);
		glTexCoord2f(1.0, 0.0); glVertex2f(x + player_xSize, player_y - player_ySize);
		glTexCoord2f(1.0, 1.0); glVertex2f(x + player_xSize, player_y + player_ySize);
		glTexCoord2f(0.0, 1.0); glVertex2f(x - player_xSize, player_y + player_ySize);
		glEnd();

		//man
		if (choose == 1) {
			glBindTexture(GL_TEXTURE_2D, man_texture);
		}
		else if (choose == 2) {
			glBindTexture(GL_TEXTURE_2D, boss_texture);
		}
		else if (choose == 3) {
			glBindTexture(GL_TEXTURE_2D, walker_texture);
		}

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(man_x, 0.35);
		glTexCoord2f(1.0, 0.0); glVertex2f(man_x+0.2, 0.35);
		glTexCoord2f(1.0, 1.0); glVertex2f(man_x+0.2, 0.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(man_x, 0.5);

		glEnd();
		// coins
		glBindTexture(GL_TEXTURE_2D, coin_texture);
		glBegin(GL_QUADS);
		for (int i = coins.size(); i > 0; i--) {
			float x = coins.front().x;
			float y = coins.front().y;
			coins.push(coins.front());
			coins.pop();
			glTexCoord2f(0.0, 0.0); glVertex2f(x - coin_xSize, y - coin_ySize);
			glTexCoord2f(1.0, 0.0); glVertex2f(x + coin_xSize, y - coin_ySize);
			glTexCoord2f(1.0, 1.0); glVertex2f(x + coin_xSize, y + coin_ySize);
			glTexCoord2f(0.0, 1.0); glVertex2f(x - coin_xSize, y + coin_ySize);
		}
		glEnd();

		//score
		glBindTexture(GL_TEXTURE_2D, score_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0.2, 0.4);  // ¥ª¤U¸}
		glTexCoord2f(1.0, 0.0); glVertex2f(0.48 , 0.4);  // ¥k¤W¨¤
		glTexCoord2f(1.0, 1.0); glVertex2f(0.48, 0.5);  // 
		glTexCoord2f(0.0, 1.0); glVertex2f(0.2 , 0.5);  // 
		glEnd();
		//timer
		glBindTexture(GL_TEXTURE_2D, timer_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0.2, 0.3);  // ¥ª¤U¸}
		glTexCoord2f(1.0, 0.0); glVertex2f(0.48, 0.3);  // ¥k¤W¨¤
		glTexCoord2f(1.0, 1.0); glVertex2f(0.48, 0.4);  // 
		glTexCoord2f(0.0, 1.0); glVertex2f(0.2, 0.4);  // 
		glEnd();

		//number
		glBindTexture(GL_TEXTURE_2D, number_texture);
		glBegin(GL_QUADS);
		draw(score%10,			0.61, 0.44, 0.025, 0.04);
		draw((score/10)%10,		0.56, 0.44, 0.025, 0.04);
		draw((score/100)%100,	0.51, 0.44, 0.025, 0.04);

		draw((20-time1) % 10,	0.56, 0.34, 0.025, 0.04);
		draw(((20-time1)/10) % 10, 0.51, 0.34, 0.025, 0.04);

		glEnd();
		
		//background
		glBindTexture (GL_TEXTURE_2D, background2_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-0.65, -0.1);
		glTexCoord2f(1.0, 0.0); glVertex2f(-0.2, -0.1);
		glTexCoord2f(1.0, 1.0); glVertex2f(-0.2 ,0);
		glTexCoord2f(0.0, 1.0); glVertex2f(-0.65, 0);
		
		glEnd();
	}
	else if(game_start==2) {
		glBindTexture(GL_TEXTURE_2D,pause_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-0.8 ,-0.8);  // ¥ª¤U¸}
		glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.8);  // ¥k¤W¨¤
		glTexCoord2f(1.0, 1.0); glVertex2f(0.8, 0.8);  // 
		glTexCoord2f(0.0, 1.0); glVertex2f(-0.8, 0.8);  // 
		glEnd();
		time_counter=stay  ;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, background1_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-0.8, -0.5);  // ¥ª¤U¸}
		glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.5);  // ¥k¤W¨¤
		glTexCoord2f(1.0, 1.0); glVertex2f(0.8, 0.5);  // 
		glTexCoord2f(0.0, 1.0); glVertex2f(-0.8, 0.5);  // 
		glEnd();
	}
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("eatmoney");

	init();
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
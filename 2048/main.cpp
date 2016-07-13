#include "Functions.h"
#include <time.h>
#include <stdlib.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>

int const ScreenWidth = 1000;
int const ScreenHeight = 610;
int const FPS = 120;

int board[4][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
bool availability[4][4] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } };
int points = 0;
int best = 0;
bool Move = true;
int moveSpeed = 100;
bool draw = true;
char pointsC[10] = { '0' };
char bestC[10] = { '0' };
bool done = false;
bool start = false;

ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *font;
ALLEGRO_FONT *font_points;
ALLEGRO_BITMAP *PressEnter = NULL;
ALLEGRO_BITMAP *TitleScreen = NULL;
ALLEGRO_BITMAP *Score = NULL;
ALLEGRO_BITMAP *Best = NULL;
ALLEGRO_BITMAP *Lose = NULL;
ALLEGRO_BITMAP *Win = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *TileBlank = NULL;
ALLEGRO_BITMAP *Tile2 = NULL;
ALLEGRO_BITMAP *Tile4 = NULL;
ALLEGRO_BITMAP *Tile8 = NULL;
ALLEGRO_BITMAP *Tile16 = NULL;
ALLEGRO_BITMAP *Tile32 = NULL;
ALLEGRO_BITMAP *Tile64 = NULL;
ALLEGRO_BITMAP *Tile128 = NULL;
ALLEGRO_BITMAP *Tile256 = NULL;
ALLEGRO_BITMAP *Tile512 = NULL;
ALLEGRO_BITMAP *Tile1024 = NULL;
ALLEGRO_BITMAP *Tile2048 = NULL;

//Helper Functions
bool isFull()
{
	bool flag = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0)
				flag = false;
		}
	}
	return flag;
}
void randomGen()
{
	int TileValue = rand() % 20;
	int randi, randj;
	do
	{
		randi = rand() % 4;
		randj = rand() % 4;
	} while (board[randi][randj] != 0);

	if (TileValue == 0)
		board[randi][randj] = 4;
	else
		board[randi][randj] = 2;

	Move = false;

}
void clearBoard()
{
	points = 0;
	pointsC[0] = '0';
	for (int k = 1; k < 10; k++)
		pointsC[k] = '\0';
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			board[i][j] = 0;
}
void clearAvailability()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			availability[i][j] = 1;
}


//Imput Functions
void upMove()
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 1; i < 4; i++)
		{
			if (board[i][j] != 0)
			{
				for (int k = i - 1; k >= 0; k--)
				{
					if (board[k][j] == 0)
					{
						board[k][j] = board[k + 1][j];
						board[k + 1][j] = 0;
						MoveUp(i, j, k, board[k][j], false);
						Move = true;
					}
					bool a = board[k][j] != 0 && board[k + 1][j] == board[k][j] && availability[k][j];
					if (board[k][j] != 0 && board[k + 1][j] == board[k][j] && availability[k][j] && availability[k + 1][j])
					{
						availability[k][j] = false;
						MoveUp(i, j, k, board[k][j], true);
						board[k][j] += board[k + 1][j];
						board[k + 1][j] = 0;
						points += board[k][j];
						Move = true;
						

					}
				}
			}
		}
		clearAvailability();
	}


}
void downMove()
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 2; i >= 0; i--)
		{
			if (board[i][j] != 0)
			{
				for (int k = i + 1; k < 4; k++)
				{
					if (board[k][j] == 0)
					{
						board[k][j] = board[k - 1][j];
						board[k - 1][j] = 0;
						MoveDown(k - 1, j, k, board[k][j], false);
						Move = true;
					}
					if (board[k][j] != 0 && board[k - 1][j] == board[k][j] && availability[k][j] && availability[k - 1][j])
					{
						availability[k][j] = false;
						MoveDown(k - 1, j, k, board[k][j], true);
						board[k][j] += board[k - 1][j];
						board[k - 1][j] = 0;
						points += board[k][j];
						Move = true;
						
					}
				}
			}


		}
		clearAvailability();
	}


}
void leftMove()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			if (board[i][j] != 0)
			{
				for (int k = j - 1; k >= 0; k--)
				{
					if (board[i][k] == 0)
					{
						board[i][k] = board[i][k + 1];
						board[i][k + 1] = 0;
						MoveLeft(j, i, k, board[i][k], false);
						Move = true;
					}
					if (board[i][k] != 0 && board[i][k + 1] == board[i][k] && availability[i][k] && availability[i][k + 1])
					{
						availability[i][k] = false;
						MoveLeft(j, i, k, board[i][k], true);
						board[i][k] += board[i][k + 1];
						board[i][k + 1] = 0;
						points += board[i][k];
						Move = true;
						
					}
				}

			}
		}
		clearAvailability();
	}


}
void rightMove()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 2; j >= 0; j--)
		{
			if (board[i][j] != 0)
			{
				for (int k = j + 1; k < 4; k++)
				{
					if (board[i][k] == 0)
					{
						board[i][k] = board[i][k - 1];
						board[i][k - 1] = 0;
						MoveRight(j, i, k, board[i][k], false);
						Move = true;
					}
					if (board[i][k] != 0 && board[i][k - 1] == board[i][k] && availability[i][k] && availability[i][k - 1])
					{
						availability[i][k] = false;
						MoveRight(j, i, k, board[i][k], true);
						board[i][k] += board[i][k - 1];
						board[i][k - 1] = 0;
						points += board[i][k];
						Move = true;
						
					}
				}
			}

		}
		clearAvailability();
	}


}

//Endgame check
bool checkWin()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
		if (board[i][j] == 2048)
			return true;
		}
	return false;
}
bool checkLose()
{
	bool flag = false;
	if (isFull())
	{
		flag = true;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < 3; j++)
			{
				if (board[i][j] == 0 || (board[i][j] == board[i][j - 1] || board[i][j] == board[i][j + 1]))
					flag = false;
				if (board[j][i] == 0 || (board[j][i] == board[j - 1][i] || board[j][i] == board[j + 1][i]))
					flag = false;
			}
		}
	}
	return flag;
}

//Allegro Functions
void LoadBitmaps(){
	PressEnter = al_load_bitmap("PressEnter.bmp");
	TitleScreen = al_load_bitmap("Title.bmp");
	Score = al_load_bitmap("Score.bmp");
	Best = al_load_bitmap("Best.bmp");
	Win = al_load_bitmap("WinScreen.bmp");
	Lose = al_load_bitmap("LoseScreen.bmp");
	background = al_load_bitmap("Background.bmp");
	TileBlank = al_load_bitmap("TileBlank.bmp");
	Tile2 = al_load_bitmap("Tile 2.bmp");
	Tile4 = al_load_bitmap("Tile 4.bmp");
	Tile8 = al_load_bitmap("Tile 8.bmp");
	Tile16 = al_load_bitmap("Tile 16.bmp");
	Tile32 = al_load_bitmap("Tile 32.bmp");
	Tile64 = al_load_bitmap("Tile 64.bmp");
	Tile128 = al_load_bitmap("Tile 128.bmp");
	Tile256 = al_load_bitmap("Tile 256.bmp");
	Tile512 = al_load_bitmap("Tile 512.bmp");
	Tile1024 = al_load_bitmap("Tile 1024.bmp");
	Tile2048 = al_load_bitmap("Tile 2048.bmp");
}
void DrawTile(int i, int j, int value, bool coord)
{
	int x = 20, y = 20;
	if (coord)
	{
		x = i;
		y = j;
		i = 0;
		j = 0;
	}
	switch (value)
	{
	case 2:
		al_draw_bitmap(Tile2, x + 145 * j, y + 145 * i, 0); break;
	case 4:
		al_draw_bitmap(Tile4, x + 145 * j, y + 145 * i, 0); break;
	case 8:
		al_draw_bitmap(Tile8, x + 145 * j, y + 145 * i, 0); break;
	case 16:
		al_draw_bitmap(Tile16, x + 145 * j, y + 145 * i, 0); break;
	case 32:
		al_draw_bitmap(Tile32, x + 145 * j, y + 145 * i, 0); break;
	case 64:
		al_draw_bitmap(Tile64, x + 145 * j, y + 145 * i, 0); break;
	case 128:
		al_draw_bitmap(Tile128, x + 145 * j, y + 145 * i, 0); break;
	case 256:
		al_draw_bitmap(Tile256, x + 145 * j, y + 145 * i, 0); break;
	case 512:
		al_draw_bitmap(Tile512, x + 145 * j, y + 145 * i, 0); break;
	case 1024:
		al_draw_bitmap(Tile1024, x + 145 * j, y + 145 * i, 0); break;
	case 2048:
		al_draw_bitmap(Tile2048, x + 145 * j, y + 145 * i, 0); break;
	case -1:
		al_draw_bitmap(TileBlank, x + 145 * j, y + 145 * i, 0); break;
		break;
	}
}
void DrawResult()
{
	al_draw_bitmap(background, 0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			DrawTile(i, j, board[i][j], false);
		}
	}
	Points();
}
void LoseScreen()
{
	al_rest(1);
	DrawResult();
	al_draw_tinted_bitmap(Lose, al_map_rgba_f(0.6, 0.6, 0.6, 0.6), 10, 10, 0);
	al_draw_text(font, al_map_rgb(119, 110, 101), 300, 200, ALLEGRO_ALIGN_CENTER, "Game over!");
	al_flip_display();
}
void WinScreen()
{
	al_rest(1);
	DrawResult();
	al_draw_tinted_bitmap(Win, al_map_rgba_f(0.6, 0.6, 0.6, 0.6), 10, 10, 0);
	al_draw_text(font, al_map_rgb(255, 255, 255), 300, 200, ALLEGRO_ALIGN_CENTER, "You win!");
	al_flip_display();
}
void MoveLeft(int i, int j, int ti, int value, bool add)
{
	int x = 20 + 145 * i;
	int y = 20 + 145 * j;
	int tx = 20 + 145 * ti;

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue_anim = al_create_event_queue();
	al_register_event_source(event_queue_anim, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (x >= tx)
	{

		ALLEGRO_EVENT events_anim;
		al_wait_for_event(event_queue_anim, &events_anim);

		if (events_anim.type == ALLEGRO_EVENT_TIMER)
		{
			x -= moveSpeed;
			draw = true;
		}

		if (draw && add == false)
		{
			draw = false;
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			Points();
			al_flip_display();
		}

		if (add == true && draw == true && x <= tx)
		{
			draw = false;
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			DrawTile(tx, y, value * 2, true);
			Points();
			al_flip_display();
		}
	}

	al_destroy_event_queue(event_queue_anim);
	al_destroy_timer(timer);
}
void MoveRight(int i, int j, int ti, int value, bool add)
{
	int x = 20 + 145 * i;
	int y = 20 + 145 * j;
	int tx = 20 + 145 * ti;

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue_anim = al_create_event_queue();
	al_register_event_source(event_queue_anim, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (x <= tx)
	{

		ALLEGRO_EVENT events_anim;
		al_wait_for_event(event_queue_anim, &events_anim);

		if (events_anim.type == ALLEGRO_EVENT_TIMER)
		{
			x += moveSpeed;
			draw = true;
		}

		if (draw)
		{
			draw = false;
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			Points();
			al_flip_display();
		}

		if (add == true && x == tx)
		{
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			DrawTile(tx, y, value * 2, true);
			Points();
			al_flip_display();
		}
	}

	al_destroy_event_queue(event_queue_anim);
	al_destroy_timer(timer);
}
void MoveUp(int i, int j, int ti, int value, bool add)
{
	int x = 20 + 145 * i;
	int y = 20 + 145 * j;
	int ty = 20 + 145 * ti;

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue_anim = al_create_event_queue();
	al_register_event_source(event_queue_anim, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (y >= ty)
	{

		ALLEGRO_EVENT events_anim;
		al_wait_for_event(event_queue_anim, &events_anim);

		if (events_anim.type == ALLEGRO_EVENT_TIMER)
		{
			y -= moveSpeed;
			draw = true;
		}

		if (draw)
		{
			draw = false;
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			Points();
			al_flip_display();
		}

		if (add == true && y == ty)
		{
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			DrawTile(x, ty, value * 2, true);
			Points();
			al_flip_display();
		}
	}

	al_destroy_event_queue(event_queue_anim);
	al_destroy_timer(timer);
}
void MoveDown(int i, int j, int ti, int value, bool add)
{
	int x = 20 + 145 * i;
	int y = 20 + 145 * j;
	int ty = 20 + 145 * ti;

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue_anim = al_create_event_queue();
	al_register_event_source(event_queue_anim, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (y <= ty)
	{

		ALLEGRO_EVENT events_anim;
		al_wait_for_event(event_queue_anim, &events_anim);

		if (events_anim.type == ALLEGRO_EVENT_TIMER)
		{
			y += moveSpeed;
			draw = true;
		}

		if (draw)
		{
			draw = false;
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			Points();
			al_flip_display();
		}

		if (add == true && y == ty)
		{
			al_clear_to_color(al_map_rgb(250, 243, 239));
			al_draw_bitmap(background, 0, 0, 0);
			for (int k = 0; k < 4; k++)
			{
				for (int s = 0; s < 4; s++)
				{
					if (k == i && s == j)
						DrawTile(i, j, -1, false);

					DrawTile(k, s, board[k][s], false);
				}
			}
			DrawTile(x, ty, value * 2, true);
			Points();
			al_flip_display();
		}
	}

	al_destroy_event_queue(event_queue_anim);
	al_destroy_timer(timer);
}
void Destroy()
{
	al_destroy_font(font);
	al_destroy_font(font_points);
	al_destroy_bitmap(background);
	al_destroy_bitmap(Score);
	al_destroy_bitmap(Best);
	al_destroy_bitmap(Win);
	al_destroy_bitmap(Lose);
	al_destroy_bitmap(TileBlank);
	al_destroy_bitmap(Tile2);
	al_destroy_bitmap(Tile4);
	al_destroy_bitmap(Tile8);
	al_destroy_bitmap(Tile16);
	al_destroy_bitmap(Tile32);
	al_destroy_bitmap(Tile64);
	al_destroy_bitmap(Tile128);
	al_destroy_bitmap(Tile256);
	al_destroy_bitmap(Tile512);
	al_destroy_bitmap(Tile1024);
	al_destroy_bitmap(Tile2048);

}
void Points()
{
	int temp, i = -1;
	int tPoints = points;
	if (points > best)
	{
		best = points;
		int tBest = best;
		temp = tBest;
		do
		{
			temp = temp / 10;
			i++;
		} while (temp > 0);
		do
		{
			bestC[i] = tBest % 10 + 48;
			tBest = tBest / 10;
			i--;
		} while (tBest > 0);
	}
	temp = points;
	do
	{
		temp = temp / 10;
		i++;
	} while (temp > 0);
	do
	{
		pointsC[i] = tPoints % 10 + 48;
		tPoints = tPoints / 10;
		i--;
	} while (tPoints > 0);

	al_draw_bitmap(Score, 650, 100, NULL);
	al_draw_text(font_points, al_map_rgb(255, 255, 255), 800, 155, ALLEGRO_ALIGN_CENTER, pointsC);

	al_draw_bitmap(Best, 650, 350, NULL);
	al_draw_text(font_points, al_map_rgb(255, 255, 255), 800, 405, ALLEGRO_ALIGN_CENTER, bestC);


}
void Title()
{
	start = true;
	ALLEGRO_TIMER *title_timer = al_create_timer(1.0 / 60);
	ALLEGRO_EVENT_QUEUE *event_queue_title = al_create_event_queue();
	al_register_event_source(event_queue_title, al_get_timer_event_source(title_timer));
	al_start_timer(title_timer);
	double i = 0.1;

	while (i <= 1)
	{
		ALLEGRO_EVENT events_title;
		al_wait_for_event(event_queue_title, &events_title);

		if (events_title.type == ALLEGRO_EVENT_TIMER)
		{
			al_draw_bitmap(TitleScreen, 0, 0, 0);
			al_draw_tinted_bitmap(PressEnter, al_map_rgba_f(i, i, i, i), 261, 450, 0);
			al_flip_display();
			i += 0.05;
		}
	}
	ALLEGRO_KEYBOARD_STATE keyStateTitle;
	al_register_event_source(event_queue_title, al_get_keyboard_event_source());
	bool next = false;

	while (!next)
	{
		ALLEGRO_EVENT events_title;
		al_wait_for_event(event_queue_title, &events_title);

		if (events_title.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (events_title.keyboard.keycode == ALLEGRO_KEY_ENTER)
				next = true;
			if (events_title.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				done = true;
				next = true;
			}

		}

	}
	if (!done)
	{
		randomGen();
		randomGen();
		i = 1;
		while (i >= 0)
		{
			ALLEGRO_EVENT events_title;
			al_wait_for_event(event_queue_title, &events_title);

			if (events_title.type == ALLEGRO_EVENT_TIMER)
			{
				DrawResult();
				al_draw_tinted_bitmap(TitleScreen, al_map_rgba_f(i, i, i, i), 0, 0, 0);
				al_flip_display();
				i -= 0.05;
			}
		}
	}

	al_flip_display();
	al_destroy_timer(title_timer);
	al_destroy_event_queue(event_queue_title);
	al_destroy_bitmap(TitleScreen);
	al_destroy_bitmap(PressEnter);

}

int main()
{
	srand((unsigned)time(0));

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, NULL);
		return -1;
	}
	display = al_create_display(ScreenWidth, ScreenHeight);
	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not create Allegro Window", NULL, NULL);
		return -1;
	}


	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_native_dialog_addon();
	LoadBitmaps();

	font = al_load_font("ClearSans-Regular.ttf", 100, NULL);
	font_points = al_load_font("ClearSans-Regular.ttf", 55, NULL);


	
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_KEYBOARD_STATE keyState;
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

	while (!done)
	{
		if (!start)
		{
			Title();
		}
		if (checkLose())
		{
			DrawResult();
			al_flip_display();
			LoseScreen();
			if (al_show_native_message_box(display, "", "Try again?", "", NULL, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO))
			{
				clearBoard();
				randomGen();
				randomGen();
				DrawResult();
				al_flip_display();

			}
			else
				done = true;
		}

		if (checkWin())
		{
			DrawResult();
			al_flip_display();
			WinScreen();
			if (al_show_native_message_box(display, "", "Try again?", "", NULL, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO))
			{
				clearBoard();
				randomGen();
				randomGen();
				DrawResult();
				al_flip_display();
			}
			else
				done = true;
		}

		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN:
				downMove();
				if (!isFull() && Move)
					randomGen();
				al_rest(0.1);
				DrawResult();
				al_flip_display();
				break;
			case ALLEGRO_KEY_UP:
				upMove();
				if (!isFull() && Move)
					randomGen();
				al_rest(0.1);
				DrawResult();
				al_flip_display();
				break;
			case ALLEGRO_KEY_RIGHT:
				rightMove();
				if (!isFull() && Move)
					randomGen();
				al_rest(0.1);
				DrawResult();
				al_flip_display();
				break;
			case ALLEGRO_KEY_LEFT:
				leftMove();
				if (!isFull() && Move)
					randomGen();
				al_rest(0.1);
				DrawResult();
				al_flip_display();
				break;
			case ALLEGRO_KEY_R:
				clearBoard();
				randomGen();
				randomGen();
				DrawResult();
				al_flip_display();
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
	}

	Destroy();
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
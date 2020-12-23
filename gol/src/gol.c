

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include "gol_config.h"
#include "cells.h"
#include "display.h"

/*
 * global variables
 */
cell_t env[config_NE][config_ME];
cell_t update_env[config_NE][config_ME];
bool reproduction_flag = false; // is high when it's mating season

int STARTX = 0;
int STARTY = 0;
int ENDX = config_ME;
int ENDY = config_NE;
WINDOW *win;
/*
 * main code
 */
int main(void)
{
	pthread_t threadptrs[config_K * config_L]; // our thread handles
	threadID_t threadID[config_K * config_L]; // thread ID

	// initialize workspace
	initEnvironment();

	// create the threads
	printf("\ncreating threads...\n");
	size_t index;
	for (size_t i = 0; i != config_K; ++i)
	{
		for (size_t j = 0; j != config_L; ++j)
		{
			index = i * config_L + j; // map (i,j) to an 1-d index
			threadID[index].row = i;
			threadID[index].col = j;
			// the following if condition returns 0 on the successful creation of each thread:
			if (pthread_create(&threadptrs[index], NULL, &updateCommFunc,
					&threadID[index]) != 0)
			{
				printf("failed to create the thread %d\n", (int) index);
				return 1;
			}
		}
	}

	// initialize display with ncurses
	initDisplay();

	unsigned short int ctr = 0;
	while (1)
	{
		reproduction_flag = true;
		usleep(config_TL / 2); // allow new generation to check in
		reproduction_flag = false;
		usleep(config_TL / 2); // put a hold on reproduction to update display
		if (++ctr == config_TDISP)
		{
			ctr = 0;
			updateDisplay();
		}
		copyEnvironment(); // write changes to the environment, env, from update_env
	}

	// should never arrive here;
	return 1;
}
/*
 * cells.c
 *
 *  Created on: May 30, 2020
 *      Author: takis
 */


/*
 * PRIVATE FUNCTIONS
 */

/*
 * transfer a single community identified by the block-pair (iT,jT) to env and
 * update_env using data_init[][]
 */
void transferCommunity(size_t iT, size_t jT,
		const cell_t data_init[config_NC][config_MC])
{
	size_t i_0 = iT * config_NC;
	size_t j_0 = jT * config_MC;

	printf("     ... transferring block (%d, %d)\n", (int) (iT + 1),
			(int) (jT + 1));
	// copy this community to each community in env to initialize it
	for (size_t i = 0; i != config_NC; ++i)
	{
		for (size_t j = 0; j != config_MC; ++j)
		{
			env[i_0 + i][j_0 + j] = update_env[i_0 + i][j_0 + j] =
					data_init[i][j];
		}
	}
}

/*
 * function counts the number of live neighbours of a cell located
 * at row r and column c of the env array
 *
 * for reference, neighbours are designated as follows:
 *     		a b c
 *              d X e
 *              f g h
 *
 *
 */
size_t countLiveNeighbours(size_t row, size_t col)
{
	size_t cell_count = 0;
	int o,p;
	neighbour_t
		if(row == 0)
		{
			a_posn = b_posn = c_posn = 0;
		}
		if(row == config_NE)
				{
					f_posn = g_posn = h_posn = 0;
				}

		if(col == 0)
			{
				a_posn = d_posn = f_posn = 0;
			}
			if(col == config_ME)
					{
						c_posn = e_posn = h_posn = 0;
					}
e
for(int i = a_postn; i<=h_postn;i++)
{
	if(p==2)
	{
		o++;
		p=0;
	}
	if(o==p)
	{
		p++
	}
	cell_count += env[row +o][col+p];
	p++
}
	// your code goes here

	return cell_count;
}

/*
 * update cell located at row r and column c in env (indicated by X):
 *
 *				a b c
 *				d X e
 *				f g h
 *
 * with nearest neighbours indicated as shown from a, b, ..., h.
 *
 * this function features Conway's rules:
 * 		- if a cell is dead but surrounded by exactly three live neighbours, it sprouts to life (birth)
 * 		- if a cell is live but has more than 4 live neighbours, it dies (overpopulation)
 * 		- if a cell is live but has fewer than 2 live neighbours, it dies (underpopulation)
 * 		- all other dead or live cells remain the same to the next generation (i.e., a live cell must
 * 		  have exactly three neighbours to survive)
 *
 */
void updateCell(size_t r, size_t c)
{
	cell_t state_cell = env[r][c];
	size_t live_neighbours = countLiveNeighbours(r, c);
if(live_neighbours ==2||live_neighbours==3)
{
	update_env[r][c] = 1;
}
else
	update_env[r][c] = 0;
	// your code goes here
}

/*
 * PUBLIC FUNCTIONS
 */
/*
 * seed environment on a community-by-community basis,
 * from standard input; we assume that the seed input is exactly
 * the size of a community; 9999 indicates end of file;
 * run this before started ncurses environment;
 */
void initEnvironment(void)
{
	// start by reading in a single community
	int token;
	cell_t datum;
	cell_t community_init[config_NC][config_MC];

	printf("\ninitializing environment...\n");
	printf("     ... loading template community from stdin\n");
	for (size_t i = 0; i != config_NC; ++i)
	{
		for (size_t j = 0; j != config_MC; ++j)
		{
			scanf("%d", &token);
			datum = (cell_t) token;
			community_init[i][j] = datum;
		}
	}
	printf("     ... done.\n");

	printf("     ... creating communities\n");
	// copy this community to each community in env to initialize it
	for (size_t i = 0; i != config_K; ++i)
	{
		for (size_t j = 0; j != config_L; ++j)
		{
			transferCommunity(i, j, community_init);
		}
	}
	printf("     ... done.\n");

}
/*
 * write changes to the environment, env, from update_env
 */
void copyEnvironment(void)
{
	// copy this community to each community in env to initialize it
	for (size_t i = 0; i != config_NE; ++i)
	{
		for (size_t j = 0; j != config_ME; ++j)
		{
			env[i][j] = update_env[i][j];
		}
	}
}

/*
 * this function updates all the cells for a thread (corresponding to one community)
 */
void* updateCommFunc(void *param)
{
size_t r , c;
threadID_t *threadID = param;

r = (size_t)(*threadID).row;
c = (size_t)(*threadID).col;
size_t i_0 = r * config_NC;
size_t j_0 = c * config_MC;

while(1)
{
	if(reproduction == true)
	{
		for(int i=0;i<config_NC;i++)

			for(int j=0;i<config_MC;i++)
		{
				updateCell(i_0 + i, j_0 + j);
		}
	}
}
	// your code goes here
}

/*
 * display.c
 *
 *  Created on: May 30, 2020
 *      author: takis
 *      note: a lot of this code adapted from the TDLP tutorial on ncurses,
 *      by Pradeep Padala
 */


/*
 * PRIVATE FUNCTIONS
 */
void create_newwin(int height, int width)
{
	win = newwin(height, width, STARTY, STARTX);
	box(win, 0, 0); /* 0, 0 gives default characters
	 * for the vertical and horizontal
	 * lines */
	wrefresh(win); /* show that box */

	return;
}

/*
 * PUBLIC FUNCTIONS
 */
void initDisplay(void)
{
	printf("\ninitializing display...\n");
	usleep(2 * config_TL);
	initscr();
	cbreak();
	timeout(TIME_OUT);
	keypad(stdscr, TRUE);
	create_newwin(config_NE, config_ME);
}

void updateDisplay(void)
{
//	ENDX = COLS - 1;
//	ENDY = LINES - 1;

	int i, j;
	wclear(win);
	for (i = STARTX; i != config_ME; ++i)
		for (j = STARTY; j != config_NE; ++j)
			if (env[j][i] == live)
				mvwaddch(win, j, i, CELL_CHAR);
	wrefresh(win);
}

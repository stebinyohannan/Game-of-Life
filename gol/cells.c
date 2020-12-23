/*
 * cells.c
 *
 *  Created on: May 30, 2020
 *      Author: takis
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gol_config.h"
#include "cells.h"

/*
 * declare important variables (defined in main file as global variables)
 */
//extern cell_t **env;
//extern cell_t **update_env;
extern cell_t env[config_NE][config_ME];
extern cell_t update_env[config_NE][config_ME];
extern bool reproduction_flag;

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

size_t countLiveNeighbours(size_t row, size_t col)
{
	size_t cell_count = 0;
	neighbour_t s;

	// your code goes here
for(s = 0;s<=h_posn;s++)
{
switch (s)
{
case a_posn:
	if((row > 0)&&(col >0))
{
		if(env[row-1][col-1] ==live)
				cell_count++;
				break;
}
	if((row == 0)&&(col >0))
{
		if(env[config_NE-1][col-1] == live)
				cell_count++;
break;
}
	if((row > 0)&&(col ==0))
{
		if(env[row-1][config_ME-1] == live)
				cell_count++;
break;
}
	if((row == 0)&&(col ==0))
{
		if(env[config_NE-1][config_ME-1] ==live)
				cell_count++;
break;
}
case b_posn:
	if(row > 0)
{
		if(env[row-1][col] ==live)
				cell_count++;
break;
}
	if((row == 0))
{
		if(env[config_NE-1][col] ==live)
				cell_count++;
break;
}

case c_posn:
	if((row > 0)&&(col < (config_ME-1)))
{
		if(env[row-1][col+1] ==live)
				cell_count++;
break;
}
	if((row == 0)&&(col < (config_ME-1)))
{
		if(env[config_NE-1][col+1] ==live)
				cell_count++;
break;
}
	if((row > 0)&&(col ==(config_ME-1)))
{
		if(env[row-1][0] ==live)
				cell_count++;
break;
}
	if((row == 0)&&(col == (config_ME-1)))
{
		if(env[config_NE-1][0] ==1)
				cell_count++;
break;
}
case d_posn:
	if((col >0))
{
		if(env[row][col-1] ==live)
				cell_count++;
break;
}
	if((col ==0))
{
		if(env[row][config_ME-1] ==live)
				cell_count++;
break;
}
case e_posn:
	if(col < (config_ME-1))
{
		if(env[row][col+1] ==live)
				cell_count++;
break;
}
	if(col == (config_ME-1))
{
		if(env[row][0] ==live)
				cell_count++;
break;
}
case f_posn:
	if((row < (config_NE-1))&&(col > 0))
{
		if(env[row+1][col-1] ==live)
				cell_count++;
break;
}
	if((row == (config_NE-1))&&(col > 0))
{
		if(env[0][col-1] == live)
				cell_count++;
break;
}
	if((row < (config_NE-1))&&(col ==0))
{
		if(env[row+1][config_ME-1] == live)
				cell_count++;
break;
}
	if((row == (config_NE-1))&&(col == 0))
{
		if(env[0][config_ME-1] ==live)
				cell_count++;
break;
}
case g_posn:
	if(row < (config_NE-1))
{
		if(env[row+1][col] ==live)
				cell_count++;
break;
}
	if((row == (config_NE-1)))
{
		if(env[0][col] ==live)
				cell_count++;
break;
}

case h_posn:
	if((row < (config_NE-1))&&(col <(config_ME-1)))
{
		if(env[row+1][col+1] ==live)
				cell_count++;
break;
}
	if((row == (config_NE-1))&&(col <(config_ME-1)))
{
		if(env[0][col+1] ==live)
				cell_count++;
break;
}
	if((row < (config_NE-1))&&(col ==(config_ME-1)))
{
		if(env[row+1][0] ==live)
				cell_count++;
break;
}
	if((row == (config_NE-1))&&(col == (config_ME-1)))
{
		if(env[0][0] ==live)
				cell_count++;
break;
}
}
}
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



if((live_neighbours ==3)&&(state_cell == dead))
{
	update_env[r][c] = live;
}
if((state_cell == live)&&((live_neighbours>3)||(live_neighbours<2)))
	update_env[r][c] = dead;
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
while (1)
{

	if(reproduction_flag == true)
	{
		for(size_t i=0;i != config_NC;i++)
		for(size_t j=0;j != config_MC;j++)
{
				updateCell(i_0+i, j_0+j);}
	}

}

}

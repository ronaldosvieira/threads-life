/*
 * The Game of Life
 *
 * a cell is born, if it has exactly three neighbours 
 * a cell dies of loneliness, if it has less than two neighbours 
 * a cell dies of overcrowding, if it has more than three neighbours 
 * a cell survives to the next generation, if it does not die of loneliness 
 * or overcrowding 
 *
 * In this version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
 * The game plays a number of steps (given by the input), printing to the screen each time.  'x' printed
 * means on, space means off.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <pthread.h>

using std::cin;
using std::cout;
using std::endl;
using namespace std::chrono;

typedef unsigned char cell_t;

typedef struct {
    int id;
    int startl, endl;
    int size, steps;
} thread_arg, *ptr_thread_arg;

const int NUM_THREADS = 4;
pthread_barrier_t barrier;

cell_t **prev, **next, **tmp;

cell_t **allocate_board (int size) {
	cell_t **board = (cell_t **) malloc(size * sizeof(cell_t*));
	int	i;
	
	for (i = 0; i < size; i++) {
		board[i] = (cell_t *) malloc(size * sizeof(cell_t));
	}
	
	return board;
}

void free_board(cell_t **board, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(board[i]);
    }
    
	free(board);
}

/* return the number of on cells adjacent to the i,j cell */
int adjacent_to(cell_t **board, int size, int i, int j) {
	int	k, l, count = 0;
	
	int sk = (i > 0)? i - 1 : i;
	int ek = (i + 1 < size)? i + 1 : i;
	int sl = (j > 0)? j - 1 : j;
    int el = (j + 1 < size)? j + 1 : j;

	for (k = sk; k <= ek; k++) {
		for (l = sl; l <= el; l++) {
			count += board[k][l];
		}
	}
	
	count -= board[i][j];
	
	return count;
}

void* play_thread(void *arg) {
    ptr_thread_arg t_arg = (ptr_thread_arg) arg;
    
    for (int k = 0; k < t_arg->steps; ++k) {
        /* for each cell, apply the rules of Life */
    	for (int i = t_arg->startl; i < t_arg->endl; i++) {
    		for (int j = 0; j < t_arg->size; j++) {
    			int a = adjacent_to(prev, t_arg->size, i, j);
    			
    			if (a == 2) next[i][j] = prev[i][j];
    			if (a == 3) next[i][j] = 1;
    			if (a < 2) next[i][j] = 0;
    			if (a > 3) next[i][j] = 0;
    		}
    	}
    	
    	pthread_barrier_wait(&barrier);
    	
    	if (t_arg->id == 0) {
        	#ifdef DEBUG
    		printf("%d ----------\n", i);
    		print(next, size);
    		#endif
    		
    		tmp = next;
    		next = prev;
    		prev = tmp;
    	}
    	
    	pthread_barrier_wait(&barrier);
    }
}

/* print the life board */
void print(cell_t **board, int size) {
	int	i, j;
	
	/* for each row */
	for (j = 0; j < size; j++) {
		/* print each column position... */
		for (i = 0; i < size; i++) {
			printf("%c", board[i][j]? 'x' : ' ');
		}
		
		/* followed by a carriage return */
		printf("\n");
	}
}

/* read a file into the life board */
void read_file(FILE *f, cell_t **board, int size) {
	int	i, j;
	char *s = (char *) malloc(size + 10);
	char c;
	
	for (j = 0; j < size; j++) {
		/* get a string */
		fgets(s, size + 10, f);
		
		/* copy the string to the life board */
		for (i = 0; i < size; i++) {
		 	//c=fgetc(f);
			//putchar(c);
			board[i][j] = s[i] == 'x';
		}
		
		//fscanf(f,"\n");
	}
}

int main() {
    high_resolution_clock::time_point start, end;
	int size, steps;
	FILE *f;
    
    f = fopen("examples/1000,100", "r");

    fscanf(f, "%d %d", &size, &steps);
	
	prev = allocate_board(size);
	read_file(f, prev, size);
	
	fclose(f);
	
	next = allocate_board(size);
	int i, j;
	
	#ifdef DEBUG
	printf("Initial \n");
	print(prev, size);
	printf("----------\n");
	#endif
	
	pthread_t threads[NUM_THREADS];
	thread_arg args[NUM_THREADS];
	
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		args[i].id = i;
		
		args[i].size = size;
		args[i].steps = steps;
		
		args[i].startl = (int) ((1.0f * size / NUM_THREADS) * i);
		args[i].endl = (int) ((1.0f * size / NUM_THREADS) * (i + 1));
	}
	
	start = high_resolution_clock::now();

	for (int i = 0; i < NUM_THREADS; ++i) {
	    pthread_create(&(threads[i]), NULL, play_thread, &(args[i]));
	}
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}
	
    end = high_resolution_clock::now();
    
    long dif = duration_cast<nanoseconds>(end - start).count();
    printf("Elasped time: %ld nanoseconds.\n", dif);
    
    pthread_barrier_destroy(&barrier);
	
	//print(prev,size);
	free_board(prev,size);
	free_board(next,size);
}

#include <stdio.h>
#include <stdlib.h>

#define boardFile "board.txt"
#define tilesFile "tiles.txt"

typedef struct {
    char color1; int num1;  /* horizontal */
    char color2; int num2;  /* vertical */
} tile;

tile *readTile(int **mark_p, int *num_p);
tile **readBoard(tile **board_p, tile *tile_v, int *nrp, int *ncp, int *add_len_p, int **mark_p);
void fillBoardR(int pos, tile *val, tile **board_p, tile **sol, int *mark, int num_tiles, int nr, int nc, int k, int *max_points_p);
int check(tile *board, int nr, int nc);
void checkErroreAllocazione(void *p);
void libera(tile *tiles, int *mark, tile *board, tile **addresses);


int main(void) {
    tile *tiles, *board, **addresses;
    int nr, nc, add_count = 0, max_points = -1, *mark, num;

    tiles = readTile(&mark, &num);
    addresses = readBoard(&board, tiles, &nr, &nc, &add_count, &mark);

    fillBoardR(0, tiles, &board, addresses, mark, num, nr, nc, add_count, &max_points);

    printf("The highest score is: %d\n", max_points);

    libera(tiles, mark, board, addresses);

    return EXIT_SUCCESS;
}


tile *readTile(int **mark_p, int *num_p) {
    FILE *fpTiles = fopen(tilesFile, "r");
    int num, i;
    tile *tiles;

    if (fpTiles == NULL) {
        printf("Error opening Tiles file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fpTiles, "%d", &num);
    tiles = (tile *) malloc(num * sizeof(tile));
    checkErroreAllocazione(tiles);

    *mark_p = (int *) calloc(num, sizeof(int));
    checkErroreAllocazione(*mark_p);

    for (i=0; i<num; i++) {
        fscanf(fpTiles, " %c %d", &(tiles[i].color1), &(tiles[i].num1));
        fscanf(fpTiles, " %c %d", &(tiles[i].color2), &(tiles[i].num2));
    }
    *num_p = num;

    fclose(fpTiles);
    return tiles;
}


tile **readBoard(tile **board_p, tile *tile_v, int *nrp, int *ncp, int *add_len_p, int **mark_p) {
    FILE *fpBoard = fopen(boardFile, "r");
    int nr, nc, i, tile_i, rotation;             /* rotation = 0 non ruotato, rotation = 1 ruotato */
    tile **addresses;                            /* il vettore di indirizzi delle tile vuote nella board */
    tile *board;

    if (fpBoard == NULL) {
        printf("Error opening Board file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fpBoard, "%d %d", &nr, &nc);

    board = (tile *) malloc(nr*nc * sizeof(tile));
    checkErroreAllocazione(board);

    addresses = (tile **) malloc(nr*nc * sizeof(tile *));
    checkErroreAllocazione(addresses);

    for (i=0; i<nr*nc; i++) {
        fscanf(fpBoard, "%d %*c %d", &tile_i, &rotation);
        if (tile_i == -1) {
            addresses[*add_len_p] = &(board[i]);
            (*add_len_p)++;
        }
        else if (rotation == 0) {
            board[i].color1 = tile_v[tile_i].color1; board[i].num1 = tile_v[tile_i].num1;
            board[i].color2 = tile_v[tile_i].color2; board[i].num2 = tile_v[tile_i].num2;
            /* segno che questa tessera non e piu disponibile */
            (*mark_p)[tile_i] = 1;
        }
        else if (rotation == 1) {
            board[i].color1 = tile_v[tile_i].color2; board[i].num1 = tile_v[tile_i].num2;
            board[i].color2 = tile_v[tile_i].color1; board[i].num2 = tile_v[tile_i].num1;
            (*mark_p)[tile_i] = 1;
        }
    }
    *board_p = board;
    *nrp = nr; *ncp = nc;

    fclose(fpBoard);
    return addresses;
}


void fillBoardR(int pos, tile *val, tile **board_p, tile **sol, int *mark, int num_tiles, int nr, int nc, int k, int *max_points_p) {
    int i, score;

    /* condizione di terminazione */
    if (pos >= k) {
        score = check(*board_p, nr, nc);
        if (score > *max_points_p)
            *max_points_p = score;
        return;
    }
    /* ciclo su tutte le tile che ho e se non le ho ancora usate le inserisco negli spazi vuoti */
    for (i=0; i<num_tiles; i++) {
        if (mark[i] == 0) {
            mark[i] = 1;

            sol[pos] -> color1 = val[i].color1; sol[pos] -> num1 = val[i].num1;
            sol[pos] -> color2 = val[i].color2; sol[pos] -> num2 = val[i].num2;
            fillBoardR(pos+1, val, board_p, sol, mark, num_tiles, nr, nc, k, max_points_p);
            /* tessera ruotata */
            sol[pos] -> color1 = val[i].color2; sol[pos] -> num1 = val[i].num2;
            sol[pos] -> color2 = val[i].color1; sol[pos] -> num2 = val[i].num1;
            fillBoardR(pos+1, val, board_p, sol, mark, num_tiles, nr, nc, k, max_points_p);
            /* backtrack */
            mark[i] = 0;
        }
    }
}


int check(tile *board, int nr, int nc) {
    int score = 0, i, j;
    int flag_r, flag_c, sum;

    for (i=0; i<nr; i++) {              /* check sulle righe */
        flag_r = 0;
        sum = 0;
        for (j=0; j<nc && flag_r==0; j++) {
            sum += board[j + nc*i].num1;
            if (j > 0 && board[j + nc*i].color1 != board[(j-1) + nc*i].color1)
                flag_r = -1;
        }
        if (flag_r == 0)
            score += sum;
    }
    for (i=0; i<nc; i++) {              /* check sulle colonne */
        flag_c = 0;
        sum = 0;
        for (j=0; j<nr && flag_c==0; j++) {
            sum += board[nc*j + i].num2;
            if (j > 0 && board[nc*j + i].color2 != board[nc*(j-1) + i].color2)
                flag_c = -1;
        }
        if (flag_c == 0)
            score += sum;
    }
    return score;
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}


void libera(tile *tiles, int *mark, tile *board, tile **addresses) {
    free(tiles);
    free(mark);
    free(board);
    free(addresses);
}
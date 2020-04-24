#include <stdio.h>
#include <stdlib.h>

int mark[100][100];
int top = 0;

typedef struct _offsets{
	int vert;
	int horiz;
}offsets;

offsets Move[8] = { {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };
// dir에 따른 Moveset의 초기화.

typedef struct {
	int Row;
	int Col;
	int Dir;
}element;

element stack[100]; // 동적으로 입력받고 싶으나.. main에서 row와 col을 받고 나서야 가능

void Push(element item) {
	if (top >= 99)
		printf("Push Error!\n");
	stack[++top] = item;
}

element Pop() {
	if (top == -1)
		printf("pop Error!\n");
	return stack[top--];
}

void findPath(int** maze, int mazeRow, int mazeCol)
{
	int i, row, col, nextRow, nextCol, dir, found = 0;
	int exitRow = mazeRow;
	int exitCol = mazeCol;
	element position;

	mark[1][1] = 1;
	top = 0;

	stack[0].Row = 1;
	stack[0].Col = 1;
	stack[0].Dir = 1;

	while (top > -1 && !found) { // stack이 empty, 혹은 찾은경우 출력
		position = Pop(); // position pop. 이전 단계 position으로 복귀.
		row = position.Row; // 현재 position의 row
		col = position.Col; // 현재 position의 col
		dir = position.Dir; // 현재 position의 dir

		while (dir < 8 && !found) { // 8방향을 모두 탐색, 혹은 찾은경우 출력.
			nextRow = row + Move[dir].vert;
			nextCol = col + Move[dir].horiz; // 다음 목적지 지정.

			if (nextRow == exitRow && nextCol == exitCol) //마지막 포인트 도달.
				found = 1;

			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {
				// 다음 maze 목적지가 0 그리고 unmarked
				mark[nextRow][nextCol] = 1; // 목적지 지정되었음을 mark

				position.Row = row; // 필요한가?
				position.Col = col; // 필요한가??
				position.Dir = ++dir; // direction변화.
				Push(position); // position의 data 갱신

				row = nextRow; // row 갱신 새롭게 전진
				col = nextCol; // col 갱신 새롭게 전진
				dir = 0; // dir 초기화
			} 
			else // 이미 방문했거나(marked), 장애물이 있음(1)
				dir++;
		}
	}
	if (found) { // 찾았다면.
		printf("The path is : \n");
		printf("row col\n"); // 출력한다.
		for (i = 0; i <= top; i++)
			printf("%2d%5d\n", stack[i].Row, stack[i].Col); // stack에 저장된 path
		printf("%2d%5d\n", row, col); // last position (갱신된)
		printf("%2d%5d\n", exitRow, exitCol); // 출구.
	}
	else
		printf("Maze does not have a path\n");
}



int main()
{
	int row = -1;
	int col = -1;

	FILE* fp = fopen("maze.txt", "r");
	fscanf(fp, "%d %d", &row, &col);
	//printf("row: %d, col : %d\n", row, col); // 3, 4

	int** maze = (int**)malloc(sizeof(int*)*(row + 2));
	for (int i = 0; i < row + 2; i++)
		maze[i] = (int*)malloc(sizeof(int)*(col + 2)); // making extended maze

	for (int i = 1; i < row + 1; i++) {
		for (int j = 1; j < col + 1; j++)
			fscanf(fp, "%d", &maze[i][j]);
	}

	for (int j = 0; j < col + 2; j++)
		maze[0][j] = 1; // 위 테두리.

	for (int k = 0; k < col + 2; k++)
		maze[row + 1][k] = 1; // 아래 테두리

	for (int l = 0; l < row + 2; l++)
		maze[l][0] = 1; // 왼쪽 테두리

	for (int m = 0; m < row + 2; m++)
		maze[m][col + 1] = 1; // 오른쪽 테두리

	/*
	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++)
			printf("%d ", maze[i][j]);
		printf("\n");
	} // 제대로 틀이 만들어졌는지 확인.
	*/

	findPath(maze, row, col);


	for (int i = 0; i < row + 2; i++)
		free(maze[i]);
	free(maze); // 메모리 해제

	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int mark[100][100];
int top = 0;

typedef struct _offsets{
	int vert;
	int horiz;
}offsets;

offsets Move[8] = { {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };
// dir�� ���� Moveset�� �ʱ�ȭ.

typedef struct {
	int Row;
	int Col;
	int Dir;
}element;

element stack[100]; // �������� �Է¹ް� ������.. main���� row�� col�� �ް� ������ ����

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

	while (top > -1 && !found) { // stack�� empty, Ȥ�� ã����� ���
		position = Pop(); // position pop. ���� �ܰ� position���� ����.
		row = position.Row; // ���� position�� row
		col = position.Col; // ���� position�� col
		dir = position.Dir; // ���� position�� dir

		while (dir < 8 && !found) { // 8������ ��� Ž��, Ȥ�� ã����� ���.
			nextRow = row + Move[dir].vert;
			nextCol = col + Move[dir].horiz; // ���� ������ ����.

			if (nextRow == exitRow && nextCol == exitCol) //������ ����Ʈ ����.
				found = 1;

			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {
				// ���� maze �������� 0 �׸��� unmarked
				mark[nextRow][nextCol] = 1; // ������ �����Ǿ����� mark

				position.Row = row; // �ʿ��Ѱ�?
				position.Col = col; // �ʿ��Ѱ�??
				position.Dir = ++dir; // direction��ȭ.
				Push(position); // position�� data ����

				row = nextRow; // row ���� ���Ӱ� ����
				col = nextCol; // col ���� ���Ӱ� ����
				dir = 0; // dir �ʱ�ȭ
			} 
			else // �̹� �湮�߰ų�(marked), ��ֹ��� ����(1)
				dir++;
		}
	}
	if (found) { // ã�Ҵٸ�.
		printf("The path is : \n");
		printf("row col\n"); // ����Ѵ�.
		for (i = 0; i <= top; i++)
			printf("%2d%5d\n", stack[i].Row, stack[i].Col); // stack�� ����� path
		printf("%2d%5d\n", row, col); // last position (���ŵ�)
		printf("%2d%5d\n", exitRow, exitCol); // �ⱸ.
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
		maze[0][j] = 1; // �� �׵θ�.

	for (int k = 0; k < col + 2; k++)
		maze[row + 1][k] = 1; // �Ʒ� �׵θ�

	for (int l = 0; l < row + 2; l++)
		maze[l][0] = 1; // ���� �׵θ�

	for (int m = 0; m < row + 2; m++)
		maze[m][col + 1] = 1; // ������ �׵θ�

	/*
	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++)
			printf("%d ", maze[i][j]);
		printf("\n");
	} // ����� Ʋ�� ����������� Ȯ��.
	*/

	findPath(maze, row, col);


	for (int i = 0; i < row + 2; i++)
		free(maze[i]);
	free(maze); // �޸� ����

	return 0;
}
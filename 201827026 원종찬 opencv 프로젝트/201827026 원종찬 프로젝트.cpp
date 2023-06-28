#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int cnt = 0;

//board 사이즈
int BOARD_ROWS = 19;//y
int BOARD_COLS = 19; //x

//board 시작점.
Point START_POINT = Point(33, 33);
Point END_POINT = Point(467, 467);


int recognize_go_board(Mat board, Mat result_board, Point start_point, Point end_point, int go_board_rows, int go_board_cols, int* result_board_points)
{
	//board size. and gap
	int row_size = end_point.x - start_point.x;
	int row_gap = (row_size / (go_board_rows - 1));

	int col_size = end_point.y - start_point.y;
	int col_gap = (col_size / (go_board_cols - 1));

	uchar* pointer_board = (uchar*)board.data;

	int bp_index = 0;
	for (int i = 0; i < go_board_rows; ++i) {
		for (int j = 0; j < go_board_cols; ++j, bp_index++) {
			int board_i = i * row_gap + start_point.y + 1;
			int board_j = j * col_gap + start_point.x + 1;

			int index = board_j + board_i * board.cols;
			
			
			

			//주변점.
			int ni = board_i + 5;
			int nj = board_j + 5;

			if (ni + 5 >= board.rows - 1)
			
				ni = board_i - 5;
			
			if (nj + 5 >= board.cols - 1)
				nj = board_j - 5;


			int n_index = nj + ni * board.cols;

			//현재점 이 흰색일때
			if (pointer_board[index] > 100) {
				result_board_points[bp_index] = 1;
				

				//주변점 이 흰색일때
				if (pointer_board[n_index] > 100) {
					result_board_points[bp_index] = 2;
					circle(result_board, Point(board_j, board_i), 2, Scalar(0, 0, 255), 2);
					circle(result_board, Point(board_j, board_i), 13, Scalar(0, 255, 0), 2);
				}
				else {
					result_board_points[bp_index] = 0;
				}
			}
			//현재점 이 흑색일때
			else if (pointer_board[index] < 100)
			{
				//주변점 이 흑색일때
				if (pointer_board[n_index] < 100) {
					result_board_points[bp_index] = 1;
					circle(result_board, Point(board_j, board_i), 2, Scalar(255, 0, 0), 2);
					circle(result_board, Point(board_j, board_i), 13, Scalar(0, 255, 0), 2);
				}
				else {
					result_board_points[bp_index] = 0;
				}
			}
		}
	}


	return 0;
}


int main()
{

	const char* path = ".\\input\\*.jpg";
	vector<String> filenames; glob(path, filenames, true /* recursive */);
	ofstream fout;
	for (int idx = 0; idx < filenames.size(); idx++)
	{
		String filename = filenames[idx];

		Mat board;

		board = imread(filename);
		//동적 배열 할당
		int* result_board_points = new int[BOARD_ROWS * BOARD_COLS];
		Mat result;
		board.copyTo(result);

		Mat gray_board;
		cvtColor(board, gray_board, COLOR_RGB2GRAY);

		//글자 지우기 위해 블러 사용.
		medianBlur(gray_board, gray_board, 3);
		medianBlur(gray_board, gray_board, 3);

		recognize_go_board(gray_board, result, START_POINT, END_POINT, BOARD_ROWS, BOARD_COLS, result_board_points);

		fout.open(".\\text\\" + to_string(cnt) + ".txt");
		int w=0;
		int b=0;
		int index = 0;
		for (int i = 0; i < BOARD_ROWS; ++i)
		{
			for (int j = 0; j < BOARD_COLS; ++j, ++index)
			{
				if (result_board_points[index] == 0)
				{
					fout << "  0";
				}
				else if (result_board_points[index] == 1)
				{
					fout << " 흑";
					b++;
				}
				else if (result_board_points[index] == 2)
				{
					fout << " 백";
					w++;
				}
			}
			printf("\n");
			fout << "\n";
		}
		fout << " 흑 개수:";
		fout << b;
		fout << " 개";

		fout << "\n";
		fout << " 백 개수:" ;
		fout << w;
		fout << " 개";
		imshow("input", board);
		imshow("result", result);
		imwrite(".\\output\\" + to_string(cnt) + ".jpg", result);	
		fout.close();
		cnt++;

		waitKey(0);
	}
	return 0;
}
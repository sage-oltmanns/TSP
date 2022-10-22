#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Point {
	float x, y;
};

float distance(int i, int j, vector<Point> a)
{
	return sqrt(
		(a[i].x - a[j].x) * (a[i].x - a[j].x)
		+ (a[i].y - a[j].y) * (a[i].y - a[j].y));
}


float findTourDistance(int i, int j, vector<vector<float>>& dp, vector<Point> a)
{
	if (dp[i][j] > 0) {
		return dp[i][j];
	}

	float distance1 = findTourDistance(i + 1, j, dp, a) + distance(i, i + 1, a);
	float distance2 = findTourDistance(i + 1, i, dp, a) + distance(j, i + 1, a);
	
	if (distance1 > distance2) {
		dp[i][j] = distance2;
	}
	else {
		dp[i][j] = distance1;
	}

	return dp[i][j];
}



void TSP(int N, vector<Point> a)
{
	vector<vector<float>> dp(N, vector<float> (N, 0));

	for (int j = 1; j < N - 1; j++)
		dp[N - 1][j] = distance(N - 1, N, a)
		+ distance(j, N, a);
	cout << findTourDistance(1, 1, dp, a) << endl;
}



int main(int argc, char** argv)
{
	ifstream fin;
	int numberOfCities;
	if (argc != 2) {
		cout << "Missing a Sample File" << endl;
		return -1;
	}

	fin.open(argv[1]);

	if (!fin) {
		cout << "File could not be opened" << endl;
		return -2;
	}

	fin >> numberOfCities;
	vector<Point> a(numberOfCities + 1);
	int i;
	for (i = 1; i <= numberOfCities; i++) 
	{
		fin >> a[i].x;
		fin >> a[i].y;
	}
	

	TSP(numberOfCities, a);

	fin.close();
	return 0;
}
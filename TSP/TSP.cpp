#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Point {
	float x, y;
};

struct bests {
	float best_distance;
	string best_path;
};

string correctBestPath(string best_path)
{
	size_t num_cities = best_path.length();
	
	// Fix best path values since indexes are 1 off 
	int i;
	for (i = 0; i < num_cities; i++)
	{
		best_path[i] = int(best_path[i]) - 1;
	}
	
	return best_path;
}

float distance(int i, int j, vector<Point> cities)
{
	return sqrt(
		(cities[i].x - cities[j].x) * (cities[i].x - cities[j].x)
		+ (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
}


bests findTourDistance(int i, int j, vector<vector<bests>>& dp, vector<Point> cities)
{
	// If a distance is already in the table return it
	if (dp[i][j].best_distance > 0) 
		return dp[i][j];

	// Recursive call to find tour distance to add points to a tour
	float distance1 = findTourDistance(i + 1, j, dp, cities).best_distance + distance(i, i + 1, cities);
	float distance2 = findTourDistance(i + 1, i, dp, cities).best_distance + distance(j, i + 1, cities);
	
	if (distance1 > distance2) {
		// Check if point j is in the path already
		size_t temp = dp[i + 1][i].best_path.find(to_string(j));

		// If it in the list remove it
		if (temp != -1) 
			dp[i + 1][i].best_path.replace(temp, 1, "");
		
		// Add j to the end of the chosen best path
		dp[i][j].best_path = dp[i + 1][i].best_path + to_string(j);

		// Add the best distance to the point into the dp table
		dp[i][j].best_distance = distance2;
	}
	else {
		// Check if point i is in the path already
		size_t temp = dp[i + 1][j].best_path.find(to_string(i));

		// If it is remove it
		if (temp != -1)
			dp[i + 1][j].best_path.replace(temp, 1, "");
	
		// Add i to the end of the chosen best path
		dp[i][j].best_path = dp[i + 1][j].best_path + to_string(i);

		// Add the best distance to the dp table
		dp[i][j].best_distance = distance1;
		
	}

	return dp[i][j];
}

vector<Point> getCities(ifstream& fin, int numberOfCities)
{
	vector<Point> cities(numberOfCities + 1);

	// Read in cities from input file
	int i;
	for (i = 1; i <= numberOfCities; i++)
	{
		fin >> cities[i].x;
		fin >> cities[i].y;
	}

	return cities;
}

void outputSolution(ofstream& fout, bests solution)
{
	size_t num_cities = solution.best_path.length();
	// Output cities backwards since we find them 
	int i;
	for (i = int(num_cities) - 1; i >= 0; i--)
	{
		fout << solution.best_path[i] << "  ";
	}
	fout << endl << solution.best_distance;
}

bests TSP(int N, vector<Point> cities)
{
	vector<vector<bests>> dp(N, vector<bests>(N, { 0, "" }));

	// Initialize paths to the rightmost point
	for (int j = 1; j < N - 1; j++)
	{
		dp[N - 1][j].best_distance = distance(N - 1, N, cities)
		+ distance(j, N, cities);
		dp[N - 1][j].best_path += to_string(j) + to_string(N) +  to_string(N - 1);
	}

	return findTourDistance(1, 1, dp, cities);
}



int main(int argc, char** argv)
{
	ifstream fin;
	ofstream fout;
	int numberOfCities;
	if (argc != 2) {
		cout << "Missing an Input File" << endl;
		cout << "Args: tsp.exe inputfile.in" << endl;
		return -1;
	}
	string inputFile = argv[1];

	size_t extensionLocation = inputFile.find_last_of(".");
	string outputFile = inputFile.substr(0, extensionLocation) + ".out";

	fin.open(inputFile);
	fout.open(outputFile);

	if (!fin) 
	{
		cout << "Input file could not be opened" << endl;
		cout << "Args: tsp.exe inputfile.in" << endl;
		return -2;
	}
	if (!fout) 
	{
		cout << "Output file could not be opened" << endl;
		return -3;
	}


	fin >> numberOfCities;

	vector<Point> cities = getCities(fin, numberOfCities);

	bests solution = TSP(numberOfCities, cities);

	solution.best_path = correctBestPath(solution.best_path);

	outputSolution(fout, solution);
	fin.close();
	fout.close();
	return 0;
}
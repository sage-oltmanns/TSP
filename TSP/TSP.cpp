#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/******************************************************************************
  * This is a structure which holds the location of a city
  ****************************************************************************/
struct Point {
	float x, y;	/**< x and y coordinates of the city */
};

/******************************************************************************
  * This is a structure which holds the shortest path of a tour and the 
  * distance of that path.
  ****************************************************************************/
struct bests {
	float best_distance;	/**< the distance of the shortest path */
	string best_path;		/**< a string which holds the shortest path */
};

/******************************************************************************
  * Fixes the indexes of the best path so they can be output.
  * 
  * @param best_path A correct path with indexes one too high
  * @return The corrected path with fixed indexes
  ****************************************************************************/
string fixBestPath(string best_path)
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

/******************************************************************************
  * Calculates the distance between two points
  *
  * @param i The index of the first point
  * @param j The index of the second point
  * @param cities A vector which holds all of the points
  * 
  * @return The distance between city i and city j.
  ****************************************************************************/
float distance(int i, int j, vector<Point> cities)
{
	return sqrt(
		(cities[i].x - cities[j].x) * (cities[i].x - cities[j].x)
		+ (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
}

/******************************************************************************
  * Recursive function to find the shortest distance and shortest path
  * to visit each city. Compares a first tour from (i + 1 and j + the 
  * distance from i and i + 1) and a second tour (i + 1 and i + the distance
  * from j to i + 1). If the first tour is shorter it updates the best distance
  * and adds i to the end of the best path. If the second tour is shorter it 
  * updates the best distance and adds j to the end of the best path.
  * 
  * @param i The location it starts from??
  * @param j The location it comes back from??
  * @param dp Dynamic programming table which stores the best tour starting at
  *			  i and coming back from j.
  * @param cities The vector that holds locations of the cities 
  * @return The dp location which holds the best distance and the best path.
  ****************************************************************************/
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

/******************************************************************************
  * Reads in all of the points from the input file
  *
  * @param fin The input file
  * @param numberOfCities The number of cities to be read in
  * @return The vector which holds all of the cities locations
  ****************************************************************************/
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

/******************************************************************************
  * Outputs the correctly formatted solution to the output file
  *
  * @param fout The output file
  * @param solution A structure which holds the best distance and the best path
  ****************************************************************************/
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

/******************************************************************************
  * Finds the distances and paths to the rightmost point and
  * 
  * @params
  * best_path - A correct path with indexes one too high
  * @return The corrected path with fixed indexes
  ****************************************************************************/
bests TSP(int numberOfCities, vector<Point> cities)
{
	vector<vector<bests>> dp(numberOfCities, vector<bests>(numberOfCities, { 0, "" }));

	// Initialize paths to the rightmost point
	for (int j = 1; j < numberOfCities - 1; j++)
	{
		dp[numberOfCities - 1][j].best_distance = distance(numberOfCities - 1, numberOfCities, cities)
		+ distance(j, numberOfCities, cities);
		dp[numberOfCities - 1][j].best_path += to_string(j) + 
			to_string(numberOfCities) +  to_string(numberOfCities - 1);
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

	solution.best_path = fixBestPath(solution.best_path);

	outputSolution(fout, solution);

	fin.close();
	fout.close();

	return 0;
}
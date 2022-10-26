#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <time.h>
#include <random>

using namespace std;

/******************************************************************************
 * This is a structure which holds the location of a city
 ****************************************************************************/
struct Point
{
	float x, y; /**< x and y coordinates of the city */
};

/******************************************************************************
 * This is a structure which holds the shortest path from one point to the end
 * and the shortest return path from another point to the end.
 * As well as the total distance of that path.
 ****************************************************************************/
struct bestDistAndPath
{
	float bestDistance; /**< the distance of the shortest path */
	string bestPath;	/**< a string which holds the shortest path */
};

/******************************************************************************
 * Fixes the indexes of the best path so they can be output.
 *
 * @param bestPath A correct path with indexes one too high
 * @return The corrected path with fixed indexes
 ****************************************************************************/
string fixBestPath(string bestPath)
{
	size_t num_cities = bestPath.length();

	// Fix best path values since indexes are 1 off
	int i;
	for (i = 0; i < num_cities; i++)
	{
		bestPath[i] = int(bestPath[i]) - 1;
	}

	return bestPath;
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
		(cities[i].x - cities[j].x) * (cities[i].x - cities[j].x) + (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
}

/******************************************************************************
 * Recursive function to find the shortest distance and shortest path
 * to visit each city. Compares a first path from (i + 1 and j + the
 * distance from i and i + 1) and a second path (i + 1 and i + the distance
 * from j to i + 1). If the first path is shorter it updates the best distance
 * and adds i to the end of the best path. If the second path is shorter it
 * updates the best distance and adds j to the end of the best path.
 *
 * @param i The point it comes back from
 * @param j The point it starts from
 * @param distanceTable Dynamic programming table which stores the best tour starting at
 *			  		 i and coming back from j.
 * @param cities The vector that holds locations of the cities
 * @return The distanceTable location which holds the best distance and the best path.
 ****************************************************************************/
bestDistAndPath getTotalDistance(int i, int j, vector<vector<bestDistAndPath>> &distanceTable, vector<Point> cities)
{
	// If a distance is already in the table return it
	if (distanceTable[i][j].bestDistance > 0)
		return distanceTable[i][j];

	// Recursive call to find tour distance to add points to a tour
	float distance1 = getTotalDistance(i + 1, j, distanceTable, cities).bestDistance + distance(i, i + 1, cities);
	float distance2 = getTotalDistance(i + 1, i, distanceTable, cities).bestDistance + distance(j, i + 1, cities);

	if (distance1 > distance2)
	{
		// Check if point j is in the path already
		size_t temp = distanceTable[i + 1][i].bestPath.find(to_string(j));

		// If it in the list remove it
		if (temp != -1)
			distanceTable[i + 1][i].bestPath.replace(temp, 1, "");

		// Add j to the end of the chosen best path
		distanceTable[i][j].bestPath = distanceTable[i + 1][i].bestPath + to_string(j);

		// Add the best distance to the point into the distanceTable
		distanceTable[i][j].bestDistance = distance2;
	}
	else
	{
		// Check if point i is in the path already
		size_t temp = distanceTable[i + 1][j].bestPath.find(to_string(i));

		// If it is remove it
		if (temp != -1)
			distanceTable[i + 1][j].bestPath.replace(temp, 1, "");

		// Add i to the end of the chosen best path
		distanceTable[i][j].bestPath = distanceTable[i + 1][j].bestPath + to_string(i);

		// Add the best distance to the distanceTable
		distanceTable[i][j].bestDistance = distance1;
	}

	return distanceTable[i][j];
}

/******************************************************************************
 * Reads in all of the points from the input file
 *
 * @param fin The input file
 * @param numberOfCities The number of cities to be read in
 * @return The vector which holds all of the cities locations
 ****************************************************************************/
vector<Point> getCities(ifstream &fin, int numberOfCities)
{
	vector<Point> cities(numberOfCities + 1);

	// Read in cities from input file
	for (int i = 1; i <= numberOfCities; i++)
	{
		fin >> cities[i].x;
		fin >> cities[i].y;
	}

	/* Uncomment these lines to generate random cities for runtime testing
	 * srand(time(NULL));
	 * vector<Point> cities(numberOfCities + 1);
	 * for (int i = 0; i <= numberOfCities; i++) {
	 *	 cities[i].x = i;
	 *	 cities[i].y = rand() % 100 + 1;
	 * }
	 */

	return cities;
}

/******************************************************************************
 * Outputs the correctly formatted solution to the output file
 *
 * @param fout The output file
 * @param solution A structure which holds the best distance and the best path
 ****************************************************************************/
void outputSolution(ofstream &fout, bestDistAndPath solution)
{
	size_t num_cities = solution.bestPath.length();
	// Output cities backwards since we find them
	int i;
	for (i = int(num_cities) - 1; i >= 0; i--)
	{
		fout << solution.bestPath[i] << "  ";
	}
	fout << endl
		 << solution.bestDistance;
}

/******************************************************************************
 * Finds the distances and paths to the rightmost point and calls the
 * recursive function to find the shortest path.
 *
 * @params
 * bestPath - A correct path with indexes from 1 to n (each index is one greater than than the desired output, 0 to n-1)
 * @return The corrected path with fixed indexes
 ****************************************************************************/
bestDistAndPath TSP(int numberOfCities, vector<Point> cities)
{
	vector<vector<bestDistAndPath>> distanceTable(numberOfCities, vector<bestDistAndPath>(numberOfCities, {0, ""}));

	// Initialize paths to the rightmost point
	for (int j = 1; j < numberOfCities - 1; j++)
	{
		distanceTable[numberOfCities - 1][j].bestDistance = distance(numberOfCities - 1, numberOfCities, cities) + distance(j, numberOfCities, cities);
		distanceTable[numberOfCities - 1][j].bestPath += to_string(j) +
													  to_string(numberOfCities) + to_string(numberOfCities - 1);
	}
	return getTotalDistance(1, 1, distanceTable, cities);
}

int main(int argc, char **argv)
{
	// uncomment this line, as well as 232, to print the runtime
	// clock_t tStart = clock();
	ifstream fin;
	ofstream fout;
	int numberOfCities;
	if (argc != 2)
	{
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

	// uncomment this line to specify the number of cities for runtime testing
	// numberOfCities = 300;

	vector<Point> cities = getCities(fin, numberOfCities);

	bestDistAndPath
		solution = TSP(numberOfCities, cities);

	solution.bestPath = fixBestPath(solution.bestPath);

	outputSolution(fout, solution);

	// uncomment these lines, as well as 198, to print the runtime
	/*fout << endl
		 << "Time taken: ";
	fout << ((double)(clock() - tStart) / CLOCKS_PER_SEC);*/

	fin.close();
	fout.close();

	return 0;
}
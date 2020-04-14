/*COP 3502C Finalterm Assignment 1
This program is written by:Jenna Bates*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h> //remember to use gcc FILENAME -lm when compiling

typedef struct point
{
	int x; //x coordinate of point
	int y; // y coordinate of point
	double hypotenuse; // hypotenuse between the point and the center of the circle used to test if the point is within the circle (to be compared to the radius of the circle)
	//int status; // 0 if out of circle, -1 if in circle
}point;

typedef struct circle
{
	//prefilter - Given
	int numPointsTotal;//total points in the set (N that is originally given)
	int radius; // The radius of the circle
	int centerX; // The center X coordinate of the circle
	int centerY; //The center Y coordinate of the circle
	point* allPoints; //array of all points in the set (array of all poins originally given)

	//post filter - Found through the filter function
	int numOfPointsWithin; //The N after the filter -> number of points within the circle -> the points that are on the line or outside of the circle completely have been removed
	point* pointsWithin; // The array of points that have been filtered -> array of points WITHIN the circle
}circle;

// distance formula function for adjusting for when the center of the circle is not the origin
double distanceFormula(int x1, int y1, int x2, int y2)
{
	double d = 0.0; //distance to be returned - double because square roots are often decimals
	double deltaX, deltaY;

	//recall the distance formula is sqrt( (deltax)^2 + (deltay)^2 ) and it will never be negative because the values are squared and square rooted so no need to account for that

	deltaX = x2 - x1;
	deltaY = y2 - y1;

	//now square them - could use the pow function but I didnt want to look it up 

	deltaX = deltaX * deltaX;
	deltaY = deltaY * deltaY;


	//add them then square root

	d = sqrt(deltaX + deltaY);

	return d;

}

//finds the distance between the center point and the point you're checking
double findHypotenuse(int x, int y, circle *usedCircle) 
{
	double h; //hypotenuse to be returned - double to account for distances that are decimals 
	h =  distanceFormula(usedCircle->centerX,usedCircle->centerY,x,y); // finds distance between the center point and the given point to be compared later to the radius
	return h;
}



//creates a new point structure from given coordinates 
point createPoint(int x, int y, circle* usedCircle) 
{
	point newPoint;// = (point)malloc(sizeof(point)); //the new point that is being created
	newPoint.x = x; //adding the x that was provided
	newPoint.y = y; //adding the y that was provided
	newPoint.hypotenuse = findHypotenuse(x, y, usedCircle); //finds the distance between the center of the given circle and the given point
	//newPoint->status = 0; //defaults status of point to 0 (AKA out of the circle) so that we can filter later
	return newPoint;
}

//creates a circle structure from given N, r, and center coordinates
circle* createCircle(int numPointsTotal, int radius, int centerX, int centerY) 
{
	circle* newCircle = malloc(sizeof(circle));
	newCircle->numPointsTotal = numPointsTotal; // N -> total number of points (unfiltered)
	newCircle->radius = radius; //radius of the circle -> distance from the center of the circle to its outer edge
	newCircle->centerX = centerX; // center x coordinate
	newCircle->centerY = centerY; // center y coordinate
	newCircle->allPoints = malloc(sizeof(point)*numPointsTotal); //Array of all points unfiltered and given in the in.txt

	newCircle->numOfPointsWithin = 0; //Defaults the number of points within the circle to 0
	newCircle->pointsWithin = malloc(sizeof(point)* newCircle->numPointsTotal); //defaults size of array of points within to the max possible size (if all of them were within the circle)

	return newCircle;
}

circle* readData(FILE *in, circle* usedCircle) //this function reads the required data from the in.txt file and return the data that are stored in your  preferred  data  structure. Hints:this  function  might  require  dynamic  memory  allocation  and  return pointer after  reading the  data. Also,  you might need to, one or more variables  when  you call the function so that the function can update those variables.
{
	circle* circleToUse;
	int x;
	int y;
	int r;
	int n;
	fscanf(in, "%d %d %d %d", &x, &y, &r, &n); //scans the file for the center coordinates and radius of the circle as well as the number of points that will be given later
	circleToUse = createCircle(n, r, x, y);
	//creates the circle that will be used in the rest of the code/ that was given in the file
	for (int i = 0; i < circleToUse->numPointsTotal; i++) //scans for new coordinate points N number of times (the number of points that are given) and makes a new point within the list of all points
	{
		int scannedX;
		int scannedY;
		fscanf(in, "%d %d)", &scannedX, &scannedY); //scans for a new x and y coordinate
		//circleToUse->allPoints[i] = createPoint(scannedX,scannedY, usedCircle); //creates a new point for each scanned coordinate
		circleToUse->allPoints[i].x = scannedX;
		circleToUse->allPoints[i].y = scannedY;
	}

	return circleToUse;
}

//this  function takes  the  data  you  received  from  ReadData()  function  and  returns  the  filtered points that are inside or on the line of the circle.
circle* filterData(circle* usedCircle, FILE* out)
{
	usedCircle->numOfPointsWithin = 0;
	for (int i = 0; i < usedCircle->numPointsTotal; i++) //goes through each point and looks to see if they are within the circle (loops N number of times)
	{
		usedCircle->allPoints[i].hypotenuse = findHypotenuse(usedCircle->allPoints[i].x, usedCircle->allPoints[i].y, usedCircle);// finds the hypotenuse to be checked
		
		if (usedCircle->allPoints[i].hypotenuse < usedCircle->radius) //if hypotenuse is less than radius then it is inside the circle
		{
			usedCircle->pointsWithin[usedCircle->numOfPointsWithin] = usedCircle->allPoints[i]; //adds point to the array of points within circle
			usedCircle->numOfPointsWithin++; //adds to the post filter N
		}

	}
	
	return usedCircle;
}

void merge(point* listToMerge, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	/* create temp arrays */
	point* L = malloc(sizeof(point)* n1);
	point* R = malloc(sizeof(point)* n2); //if your compiler does not support this, create them dynamically.    /* Copy data to temp arrays L[] and R[] */
	//int L[n1], R[n2];
	
	for (i = 0; i < n1; i++)
	{
		L[i] = listToMerge[l + i];
		
	}

	
	for (j = 0; j < n2; j++)
	{
		R[j] = listToMerge[m + 1 + j];   
		
		/* Merge the temp arrays back into arr[l..r]*/
	}
	i = 0;
	// Initial index of first subarray  
	j = 0;
	// Initial index of second subarray
	k = l;
	// Initial index of merged subarray    
	while (i < n1 && j < n2)
	{
		//printf("%d %d\n", n1, n2);
		
		if (L[i].x == R[j].x)
		{
			
			if (L[i].y < R[j].y)
			{
				listToMerge[k] = L[i];
				
				i++;
			}
			else //if (L[i].y >= R[j].y)
			{
				listToMerge[k] = R[j];
				j++;
				
			}
			
		}
		else
		{
			if (L[i].x < R[j].x)
			{
				listToMerge[k] = L[i];
				
				i++;
			}
			else //if (L[i].x >= R[j].x)
			{
				listToMerge[k] = R[j];
				
				j++;
			}
		}
		
		k++;
	}
	/* Copy the remaining elements of L[], if there       are any */
	while (i < n1)
	{
		listToMerge[k] = L[i];
		i++;
		k++;
	}    /* Copy the remaining elements of R[], if there       are any */
	while (j < n2)
	{
		listToMerge[k] = R[j];
		j++;
		k++;
	}
	
	
}
/* l is for left index and r is right index of the   sub-array of arr to be sorted */
void mergeSort(point* listToSort, int l, int r, circle* usedCircle, FILE* out)//
{
	//r = r - 1;
	if (l < r)
	{        // get the mid point        
		int m = (l + r) / 2;// Sort first and second halves 
		mergeSort(listToSort, l, m, usedCircle,out);
		mergeSort(listToSort, m + 1, r, usedCircle,out);

		merge(listToSort, l, m, r);
		
	}

}


int BinarySearch(point* listToSearch, int N, int x, int y) 
{
	N = N - 1; //accounts for array starting at 0
	int l = 0; //lower
	int u = N; //upper
	int mid; 

		
		while (l <= u)//!0 == true and 0 == false
		{
			//printf("test\t");
			//printf("%d %d %d\t", l, u, mid);
			mid = (l + u) / 2;
			if (listToSearch[mid].x == x && listToSearch[mid].y == y)
			{
				return mid; //location it was found
			}
			if (listToSearch[mid].x < x || ((listToSearch[mid].x == x && listToSearch[mid].y < y)))
			{
				l = mid + 1;
			}
			if (listToSearch[mid].x > x || ((listToSearch[mid].x == x && listToSearch[mid].y > y)))
			{
				u = mid - 1;
			}
			
			
		}
	
	
	return -1; // -1 means not found
}

void SearchPhase(point* listToSearch, int N)//This  function  takes  your  sorted  data  that  was  modified  by the  merge  sort.  Then  it continuously promptsthe user for search points and show the result or exit based on the provided criteria.
{

	int x, y, location;
	int start = 1;
	while (1) //loop condition
	{
		printf("Search input (x y):"); 
		scanf("%d %d", &x, &y); //changes x and y to inputted values
		if ((x == -999) || (y == -999)) //checks for end condition
		{
			printf("exit\n");
			return;//ends function
		}
		else
		{
			location = BinarySearch(listToSearch, N, x, y); // -1 if not found, index if found2
			if (location == -1) 
			{
				printf("Not Found\n");
			}
			else
			{
				printf("Found at record %d\n", location + 1);
				
			}
		}	
	}
	return;
}




int main()
{
	FILE* in;
	FILE* out;

	in = fopen("in.txt", "r");
	if (in == NULL)
	{
		printf("file error :(");
		exit(1);
	}
	out = fopen("out.txt", "w");
	circle* mainCircle = readData(in,mainCircle);
	mainCircle = filterData(mainCircle, out);
	mergeSort(mainCircle->pointsWithin, 0, (mainCircle->numOfPointsWithin) - 1, mainCircle,out);
	for (int i = 0; i < mainCircle->numOfPointsWithin; i++)
	{
		fprintf(out, "%d %d\n", mainCircle->pointsWithin[i].x, mainCircle->pointsWithin[i].y);
	}
	fclose(out);
	printf("filtered and sorted data written to out.txt\n");
	SearchPhase(mainCircle->pointsWithin, mainCircle->numOfPointsWithin);
	printf("Thanks!\n");
	fclose(in);
	return 0;
}

// Euler.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define bool int
#define true 1
#define false 0
#define mapTile unsigned char

#define TILE_BLANK 0
#define TILE_BAD 1
#define TILE_QUEEN 2
#define TILE_INVALID 3
#define MAPSIZE 15



//Allocates memory space for map
mapTile * createMap(){
  int i = 0;
	mapTile * ptr = (mapTile *)malloc(sizeof(mapTile) * MAPSIZE * MAPSIZE);
	for(i; i < MAPSIZE * MAPSIZE; i++){
		if (false && i == 92){
			ptr[i] = TILE_BAD;
		} else {
			ptr[i] = TILE_BLANK;
		}
	}
	return ptr;
}

//Adds items to a list, increments count and returns size
void addToList(mapTile* list, int value, int index){
	*(list + index) = value;

}

//returns # of invalidated squares
void computeInvalidSquaresBelow(mapTile* startPt, int size, int start, mapTile* invalidList){
	int col = start % size;
	int currCellNum, cellDelta;
	mapTile * tmpPtr;
	//mapTile* currCell = startPt;
	for(currCellNum = (start / size + 1) * size + col, cellDelta = 1; currCellNum < size * size; currCellNum += size, cellDelta++){	
		startPt += size;
		//Move down the column and mark as invalid
		if(*startPt == TILE_BLANK){
			*startPt = TILE_INVALID;
			//(invalidList + start + (cellDelta * size))= start;
			addToList(invalidList, start, currCellNum);
		} 
		//Mark diagonals as invalid
		if(col + cellDelta < size){
			tmpPtr = startPt + cellDelta;
			if(*tmpPtr == TILE_BLANK ){
				*tmpPtr = TILE_INVALID;
				addToList(invalidList, start, currCellNum + cellDelta);
			}
		}
		if(col - cellDelta >= 0){
			tmpPtr = startPt - cellDelta;
			if( *tmpPtr == TILE_BLANK){
				*tmpPtr = TILE_INVALID;
				addToList(invalidList, start, currCellNum - cellDelta);
			} 
		}
	}
}

//Restores invalidated squares to blank
void restoreInvalidSquares(mapTile * startPt, mapTile* list, int start){
	int size = MAPSIZE;
	int col = start % size;
	int currRow, cellDelta;
	mapTile * tmpPtr;
	//mapTile* currCell = startPt;
	mapTile* currList = list + start;
	for(currRow  = start / size + 1, cellDelta = 1; currRow < size; currRow++, cellDelta++){	
		startPt += size;
		currList += size;
		//Move down the column and mark as invalid
		if(*(currList) == start){
			*startPt = TILE_BLANK;
			*currList = 255;
			//*(invalidList + start + (cellDelta * size))= start;
			//addToList(list, start, currCellNum);
		} 
		//Mark diagonals as valid if they were marked invalid by this run
		if(col + cellDelta < size){
			tmpPtr = currList + cellDelta;
			if(*tmpPtr == start ){
				*tmpPtr = 255;
				*(startPt + cellDelta) = TILE_BLANK;
			}
		}
		if(col - cellDelta >= 0){
			tmpPtr = currList - cellDelta;
			if( *tmpPtr == start){
				*tmpPtr = 255;
				*(startPt - cellDelta) = TILE_BLANK;
			} 
		}
	}

	/*int i;
	mapTile* tmpList = list + start;
	mapTile * tmpStart = startPt;
	for(i = start; i < MAPSIZE * MAPSIZE; i++, tmpList++, tmpStart++){
	if(*tmpList == start){
	*tmpStart = TILE_BLANK;
	*tmpList = -1;
	}
	}
	*/
}

bool rowInvalid(mapTile * map, int size, mapTile * startPt){
	int j;
	mapTile * tmpMap = map + (size * size) - 1;
	bool rowValid;
	while(tmpMap > (startPt + size)){
		rowValid = false;
		for(j = size; j > 0; j--, tmpMap--){
			if(*tmpMap == TILE_BLANK){
				rowValid = true;
				tmpMap -= j;
				break;
			}
		}

		if (rowValid == false){
			return true;
		}
	}
	return false;
}

//Recursive function to calculate the # of possibilities
int computePossibilities(mapTile* map, int size, mapTile* startPtr,  int start, mapTile* invalidList, mapTile * endPt){
	int i;
	int sum = 0;
	mapTile* startPt = startPtr;
	if (startPt == endPt){
		return 1;
	}

	if (rowInvalid(map, size, startPt) == true){
		return 0;
	}/**/

	for(i = 0; i < size; i++, startPt++, start++){
		//If tile not empty, move on
		if (*startPt != TILE_BLANK){
			continue;
		}
		//Place a queen

		//*startPt = TILE_QUEEN;
		computeInvalidSquaresBelow(startPt, size, start, invalidList);
		startPt += (size - i);
		sum += computePossibilities(map, size, startPt, ((start / size) + 1) * size,  invalidList, endPt);
		startPt -= (size - i);
		restoreInvalidSquares(startPt, invalidList, start);
		//*startPt = TILE_BLANK;
	}
	startPt -= i;
	return sum;

}

int main(int argc, char* argv[]){

	char userinput;
	mapTile* invalidList;
	int i;
	float start, end;
	mapTile * map, * startPt;
	map = createMap();
	startPt = map;
	invalidList = (mapTile*) malloc(sizeof(mapTile) * MAPSIZE * MAPSIZE);
	for(i = 0; i < MAPSIZE * MAPSIZE; i++){
		invalidList[i] = 255;
	}
	start = clock();
	printf("SUM: %d\n",computePossibilities(map, MAPSIZE, startPt, 0, invalidList, map + (MAPSIZE * MAPSIZE)));
	end = clock();
	free(map);
	free(invalidList);
	printf("TIME %fs \n", (end-start)/1000.0);
	printf("Press enter to end program");
	fflush(stdout);
	userinput = fgetc(stdin);
	return 0;
}

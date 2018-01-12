#include <iostream>
#include <cmath>

using namespace std;

void bubbleSort(float arr[], int size);

float analysis (float * freqList, int avgCnt);

int main ()
{
	float freqList[10] = {50, 50.1, 50.2, 49, 45, 47, 51, 51.5, 52, 53};
	printf ("mean = %f", analysis (freqList, 10));
	return 0;
}

float analysis (float * freqList, int avgCnt)
{
  float mean = 0;
  float sum = 0;
  int newAvgCnt = 0;
  bubbleSort (freqList, avgCnt);//Sort List
  
  for (int k = 0; k < 2; k++)
  {    
    //Get Quartile
    float q3 = 0.75 * (avgCnt + 1);
    float q3val = (q3 - int(q3))*(freqList[int(ceil(q3))-1]-freqList[int(q3)-1]) + freqList[int(q3)-1];
    float q1 = 0.25 * (avgCnt + 1);
    float q1val = (q1 - int(q1))*(freqList[int(ceil(q1))-1]-freqList[int(q1)-1]) + freqList[int(q1)-1];

    //Analyze the freqList and set invalid ones to = 0
    sum = 0;
    newAvgCnt = 0;
    for (int i = 0; i < avgCnt; i++)
    {
      if (freqList[i] <= q3val && freqList[i] >= q1val) newAvgCnt ++;
      else freqList[i] = 0;
    }

    //Create new smaller list
    int start = (avgCnt - newAvgCnt)/2;
    for (int i = 0; i < newAvgCnt; i++)
    {
    	freqList[i] = freqList[i + start];
    	freqList[i + start] = 0;
    }
    avgCnt = newAvgCnt;
  }//Done assessing values
  
  for (int i = 0; i < avgCnt; i++) sum += freqList[i];
  return sum / avgCnt;
}

void swap(float arr[], int a, int b)
{
  if(a != b)
  {
    arr[a] = arr[a] + arr[b];
    arr[b]= arr[a] - arr[b];
    arr[a] = arr[a] - arr[b];
  }
}

void bubbleSort (float arr[], int size)
{ 
  for (int i = size-1; i > 0; i--)
  {
    for ( int j = 0; j < i; j++)
    {
      if(arr[j] > arr [j+1])
        swap (arr, j, j+1);
    }
  }
  return;
}

void bubbleSort(float arr[], int size);

float analysis (float * freqList, int avgCnt)
{
  float mean = 0;
  float sum = 0;
  int newAvgCnt = 0;
  for (int k = 0; k < 2; k++)
  {    
    bubbleSort (freqList, avgCnt);//Sort List
    //Get Box Plot Info
    float q3 = 0.75 * (avgCnt + 1);
    float q3val = (q3 - int(q3))*(freqList[int(ceil(q3))-1]-freqList[int(q3)-1]) + freqList[int(q3)-1];
    float q1 = 0.25 * (avgCnt + 1);
    float q1val = (q1 - int(q1))*(freqList[int(ceil(q1))-1]-freqList[int(q1)-1]) + freqList[int(q1)-1];
    
    sum = 0;
    newAvgCnt = 0;
    for (int i = 0; i < avgCnt; i++)
    {
      if (freqList[i] <= q3val && freqList[i] >= q1val) newAvgCnt ++;
      else freqList[i] = 0;
    }
   
    int index = 0;
    float * array = new float [newAvgCnt];
    for (int i = 0; i < avgCnt; i++)
    {
      if (freqList[i] != 0)
      {
        array[index] = freqList[i];
        index++;
      }
    }
    avgCnt = newAvgCnt;
    freqList = array;
  }//Done assessing values
  
  for (int i = 0; i < avgCnt; i++)
  {
    sum += freqList[i];
  }
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

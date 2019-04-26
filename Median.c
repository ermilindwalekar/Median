/*  THE APPROACH
 *  ************
 *  Based on trading off between
 *    - Need max and min entries, costing 1 pass over the array (count n)
 *    - Worst-case search is binary "chop", costing O(log n) PASSES (O(n log n) (hope I've got that thumb-thuck right;-)
 *    - Improvement maybe possible... let's see how we go
 *  - Note adopted definition: median of 1,2,3,4 (even number of data points) is 2 (n/2) highest value
*/

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

size_t nPasses = 0;

void Range(int *rgint, int *min, int *max, int n)
{
    int i;
    *min = *max = rgint[0];
    for (i = 0; i < n ; i++)
    {
        if (rgint[i] < *min)
            *min = rgint[i];
        else if (rgint[i] > *max)
            *max = rgint[i];
    }
}

void Counting(int *array, DataPointInfo *pguess, int n)
{
    int i;
    
    pguess->countAbove = pguess->countBelow = pguess->countEqual = 0;
    pguess->nearestBelow = INT_MIN;
    pguess->nearestAbove = INT_MAX;

    for (i = 0; i < n; i++)
    {
        if (pguess->Value > array[i])
        {
            pguess->countBelow++;
            if (i > pguess->nearestBelow)
                pguess->nearestBelow = i;
        }
        else if (pguess->Value < array[i])
        {
            pguess->countAbove++;
            if (i < pguess->nearestAbove)
                pguess->nearestAbove = array[i];
        }
        else pguess->countEqual++;
    }
}

int median(int *array, int n)
{
    int min = 0, max = 0;
    Range(array, &min, &max, n);

    //First pass for do
    nPasses++;
    DataPointInfo dpi = { .Value = (min + max) / 2, .low = min, .high = max , 
        .nearestBelow = INT_MIN, .nearestAbove = INT_MAX, .countEqual = 0, .countBelow = 0,
        .countAbove = 0};
    do
    {
        Counting(array, &dpi, n);
        //Just counting passes to see if I am efficient and can do it in least passes
        nPasses++;
        if (dpi.countBelow <= dpi.countAbove + dpi.countEqual && dpi.countBelow + dpi.countEqual >= dpi.countAbove)
            return dpi.countEqual > 0 ? dpi.Value : (dpi.nearestBelow + dpi.nearestAbove) / 2; //  found

        if (dpi.countBelow < dpi.countAbove)  //  must be "to the right"
            dpi.low = dpi.nearestAbove;
        
        else  //  must be "to the left"
            dpi.high = dpi.nearestBelow;
        dpi.Value = (dpi.low + dpi.high) / 2;
    }
    while (true);
}

int main()
{
    int testData[] = { 2,1,3,5,4 };
    int count = 5;
    int med = median(testData, count);
    printf("\n%d", med);
    //std::cout << median << " found in " << nPasses << " passes over the dataset";
}
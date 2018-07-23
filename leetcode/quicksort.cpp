//#include "leetcode.h"

#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

void quicksort( vector<int>& nums, int l, int r )
{
    // return if it's a single item.
    if( l == r ) return;

    // calculate the pivot, avoid overflow by dividing first, add to r as it's not inclusive
    int pivot = l + ( (r+1) - l ) / 2;
    // save values for later
    const int start = l, end = r;

    // sort start, pivot and end item.
    int* v[] = { &nums[start], &nums[pivot], &nums[end] };
    if( *v[2] < *v[1] ) swap( *v[1], *v[2] );
    if( *v[1] < *v[0] ) swap( *v[0], *v[1] );
    if( *v[2] < *v[1] ) swap( *v[1], *v[2] );

    const int pv = nums[pivot];

    // swap items arround pivot.
    while( l < r )
    {   
        if (nums[l] <= pv)
            ++l;
        else if ( nums[r] > pv )
            --r;
		else
        {
			swap( nums[l], nums[r] );
		}
    }

    // recursive call.
    quicksort( nums, start, l-1 );
    quicksort( nums, l, end );
}

template<typename T>
void quick_sort_step(const T left, const T right)
{
    auto pivot = *(left + (right - left) / 2);
    auto l = left;
    auto r = right;

    while (l <= r) {
        while (*l < pivot)
            ++l;

        while (*r > pivot)
            --r;

        if (l <= r)
            std::swap(*l++, *r--);
    }

    if (left < r)
        quick_sort_step(left, r);
    if (l < right)
        quick_sort_step(l, right);
}

template<typename T>
void quick_sort(const T begin, const T end)
{
    if (end - begin > 1)
        quick_sort_step(begin, std::prev(end));
}

int main()
{
    vector<int> nums = {8, 5, 2, 6, 4, 0, 9, 1, 3, 7};
    //quicksort( nums, 0, nums.size()-1 );

	quick_sort( nums.begin(), nums.end() );

    return 0;
}

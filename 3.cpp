#include <iostream>
#include <omp.h>
#include <vector>
#include <climits> // For INT_MAX and INT_MIN
using namespace std;

int main() 
{
    vector<int> data = {12, 34, 56, 23, 78, 90, 11, 45, 67, 89};
    int n = data.size();
    int global_min = INT_MAX;
    int global_max = INT_MIN;
    int global_sum = 0;

    #pragma omp parallel for reduction(min:global_min) reduction(max:global_max) reduction(+:global_sum)
    for (int i = 0; i < n; ++i) 
    {
        global_min = min(global_min, data[i]);
        global_max = max(global_max, data[i]);
        global_sum += data[i];
    }

    
    float average = static_cast<float>(global_sum) / n;
    cout << "Min: " << global_min << endl;
    cout << "Max: " << global_max << endl;
    cout << "Sum: " << global_sum << endl;
    cout << "Average: " << average << endl;
    return 0;
}


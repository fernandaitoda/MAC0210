// CPP program for implementing 
// Newton divided difference formula 
#include <bits/stdc++.h> 
#include <iostream>

using namespace std; 
  
// Function for calculating 
// divided difference table 
void dividedDiffTable(float x[], float y[][10], int n) 
{ 
for (int i = 1; i < n; i++)
        for (int j = 0; j < n-1; j++)
            y[j][i] = (y[j][i-1] - y[j+1] [i-1]) / (x[j] - x[i+j]);
} 
  
// Function for applying Newton's 
// divided difference formula 
float applyFormula(float x0, float x[], 
                   float y[][10], int n) 
{ 
    float product, res;

    // first term in sum f0
    res = y[0][0];

    for (int i = 1; i < n; i++){
        product = 1;
        for (int j = 0; j < i; j++)
            product *= (x0-x[j]);
        res += (product * y[0][i]);
    }
    return res;
}
  
// Driver Function 
int main() 
{ 
    // number of inputs given 
    int n = 4; 
    float value, sum, y[10][10]; 
    float x[] = { 5, 6, 9, 11 }; 
  
    // y[][] is used for divided difference 
    // table where y[][0] is used for input 
    y[0][0] = 12; 
    y[1][0] = 13; 
    y[2][0] = 14; 
    y[3][0] = 16; 
  
    // calculating divided difference table 
    dividedDiffTable(x, y, n); 

    // value to be interpolated 
    value = 5; 
  
    // printing the value 
    cout << "\nValue at " << value << " is "
               << applyFormula(value, x, y, n) << endl; 
    return 0; 
} 

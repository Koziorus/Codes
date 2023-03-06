#include <iostream>
using namespace std;

void print_combinations(string* zbior, string podzbior, int aktualny_elem, int n, int m)
{
    podzbior += zbior[aktualny_elem];

    // jesli jest na glebokosci rownej oryginalnemu m
    if(m == 1)
    {
        cout<<podzbior<<"\n";
        return;
    }

    for(int i = aktualny_elem + 1; i < n; i++)
    {
        print_combinations(zbior, podzbior, i, n, m - 1);
    }
}

int main()
{
    int n, m;
    cin>>n>>m;

    string* zbior = new string[n];
    
    for(int i = 0; i < n; i++)
    {
        zbior[i] = '1' + i;
    }

    for(int i = 0; i < n; i++)
    {
        print_combinations(zbior, "", i, n, m);
    }

    return 0;
}
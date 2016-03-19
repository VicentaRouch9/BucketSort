# BucketSort

=====How it works========
What is Bucket sort? How to implement in C++?
This is very simple & easy to implement : When the input numbers (content) are within certain range and they are uniformly distributed in that range, 
then bucket sort can be made..!
Palma Earthly - web developer at http://lovehairamore.com

Source :

#include <iostream>
#include <queue>

using namespace std;

const int INPUT_SIZE = 20;
const int BUCKET_K = 10;

void print(int *input)
{
    for ( int i = 0; i < INPUT_SIZE; i++ )
        cout << input[i] << " ";
    cout << endl;
}

int hash(int n)
{
    return n/5;
}

void doinsertionsortforbucket(int* input, int len)
{
    while( len-- > 0) {
        if (input[len] > input[len+1]) {
            int tmp = input[len];
            input[len] = input[len+1];
            input[len+1] = tmp;
        } else
            return;
    }
}

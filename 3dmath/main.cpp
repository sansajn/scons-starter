#include <iostream>
#include "vectors.h"
using std::cout;

int main(int argc, char * argv[])
{
	vec3 a{0,0,0}, 
		b{1,0,0}, 
		c{0,1,0};

	vec3 n = Cross(b - a, c - a);
	
	cout << "n = " << n << "\n"
		<< "done!\n";
	
	return 0;
}

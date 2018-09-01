#include <iostream>
#include "operator.h"

using namespace std;
using namespace graph;

int main() {
	NodePtr n1(new Value(1));
	NodePtr n2(new Value(2));
	NodePtr n3 = n1 + n2;
	cout << n3.getValue(false) << endl;
	cout << n1.getDerivative(n3, false) << endl;
	cout << n2.getDerivative(n3, false) << endl;
}
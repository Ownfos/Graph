#include <iostream>
#include "operator.h"

using namespace std;
using namespace graph;

int main() {
	NodePtr n1(new Value(2));
	NodePtr n2(new Value(3));
	NodePtr n3 = n1 && (n2 * n2);
	//NodePtr n4 = n3 || (n1 + n1);
	cout << n3.getValue(false) << endl;
	cout << n1.getDerivative(n3, false) << endl;
	cout << n2.getDerivative(n3, false) << endl;
	system("pause");
}
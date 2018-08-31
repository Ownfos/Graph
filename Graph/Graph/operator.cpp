#include "operator.h"

namespace graph {

	Variable::Variable(double value) {
		this->value = value;
	}

	void Variable::clearValue() {

	}

	double Variable::getValue(bool reuse) {
		return value;
	}

	double Variable::getPartialDerivative(Node* node) {
		return (node == this);
	}


	double Add::getValue(bool reuse) {
		if (!reuse || value == UNDEFINED) {
			value = 0;
			for (Node* node : source) {
				value += node->getValue(reuse);
			}
		}
		return value;
	}

	double Add::getPartialDerivative(Node* node) {
		if (node == this) {
			return 1;
		}
		else {
			int rtn = 0;
			for (Node* input : source) {
				rtn += (input == node);
			}
			return rtn;
		}
	}

}
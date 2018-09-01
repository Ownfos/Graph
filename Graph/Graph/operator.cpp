#include "operator.h"

namespace graph {

	Value::Value(double value) {
		this->value = value;
	}


	Reference::Reference(double& reference) : reference(reference) {

	}

	double Reference::getValue(bool reuse) {
		return reference;
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

	double Add::getPartialDerivative(Node* node, bool reuse) {
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


	double Multiply::getValue(bool reuse) {
		if (!reuse || value == UNDEFINED) {
			value = 1;
			for (Node* node : source) {
				value *= node->getValue(reuse);
			}
		}
		return value;
	}

	double Multiply::getPartialDerivative(Node* node, bool reuse) {
		if (node == this) {
			return 1;
		}
		else {
			int rtn = 0;
			int value = getValue(reuse);
			for (Node* input : source) {
				rtn += (input == node) * value / input->getValue(reuse);
			}
			return rtn;
		}
	}

}
#pragma once

#include "node.h"

namespace graph {

	class Variable : public Node {
	public:
		Variable(double value);

		virtual void clearValue() override;
		virtual double getValue(bool reuse) override;
		virtual double getPartialDerivative(Node* node) override;
	};

	class Add : public Node {
	public:
		virtual double getValue(bool reuse) override;
		virtual double getPartialDerivative(Node* node) override;
	};

}
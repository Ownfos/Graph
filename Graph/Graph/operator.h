#pragma once

#include "node.h"

namespace graph {

	class Value : public IndependentNode {
	public:
		Value(double value);
	};

	class Reference : public IndependentNode {
	private:
		double& reference;

	public:
		Reference(double& reference);

		virtual double getValue(bool reuse) override;
	};

	class Add : public DependentNode {
	public:
		virtual double getValue(bool reuse) override;
		virtual double getPartialDerivative(Node* node, bool reuse) override;
	};

	class Multiply : public DependentNode {
	public:
		virtual double getValue(bool reuse) override;
		virtual double getPartialDerivative(Node* node, bool reuse) override;
	};

}
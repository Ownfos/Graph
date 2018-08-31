#pragma once

#include <map>
#include <vector>

#define UNDEFINED 999999

namespace graph {

	// Interface of node in calculation graph
	class Node {
	protected:
		double value;
		std::vector<Node*> source;// Input nodes
		std::vector<Node*> destination;// Output nodes
		std::map<Node*, double> derivative;// Saves differentiated value for reuse

		static std::vector<Node*> list;// All Node instances created

		void addDestination(Node* node);
		void removeDestination(Node* node);

	public:
		Node();

		void addSource(Node* node);
		void removeSource(Node* node);
		double getTotalDerivative(Node* node, bool reuse);

		virtual void clearValue();
		virtual double getValue(bool reuse) = 0;
		virtual double getPartialDerivative(Node* node) = 0;

		static void clear();
	};

	// Wrapper class of Node pointer
	class NodePtr {
	private:
		Node* node;

	public:
		NodePtr(Node* node);

		NodePtr operator+(NodePtr);
	};

}
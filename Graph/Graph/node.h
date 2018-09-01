#pragma once

#include <map>
#include <vector>

#define UNDEFINED 999999

namespace graph {

	// Interface of node in calculation graph
	class Node {
	protected:
		double value;
		std::vector<Node*> destination;// Other nodes which use this node as their input
		std::map<Node*, double> derivative;// Saves differentiated value for reuse

		static std::vector<Node*> list;// All Node instances created

	public:
		Node();

		double getTotalDerivative(Node* node, bool reuse);

		virtual void clearValue() = 0;
		virtual double getValue(bool reuse) = 0;
		virtual double getPartialDerivative(Node* node, bool reuse) = 0;

		static void clear();
	};

	// Nodes which do not require other nodes for value calculation
	class IndependentNode : public Node {
	public:
		virtual void clearValue() override;
		virtual double getValue(bool reuse) override;
		virtual double getPartialDerivative(Node* node, bool reuse) override;
	};

	// Nodes which require other nodes for value calculation
	class DependentNode : public Node {
	protected:
		std::vector<Node*> source;// Other nodes which are used as input

	public:
		void connect(Node* other);
		void disconnect(Node* other);
		void mergeSource(DependentNode* other);

		virtual void clearValue() override;
	};

	// Wrapper class of Node pointer
	class NodePtr {
	private:
		Node* node;

	public:
		NodePtr(Node* node);

		NodePtr operator+(NodePtr other);
		NodePtr operator*(NodePtr other);
		friend NodePtr operator||(NodePtr node1, NodePtr node2);
		friend NodePtr operator&&(NodePtr node1, NodePtr node2);
	};

}
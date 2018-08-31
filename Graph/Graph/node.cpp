#include "node.h"
#include "operator.h"

namespace graph {

	std::vector<Node*> Node::list;

	Node::Node() : value(UNDEFINED) {
		list.push_back(this);
	}

	// Destination should not be overlapped since getDerivativePartial handles that
	void Node::addDestination(Node* node) {
		if (std::find(destination.begin(), destination.end(), node) == destination.end()) {
			destination.push_back(node);
			derivative[node] = UNDEFINED;
		}
	}

	void Node::removeDestination(Node* node) {
		destination.erase(std::find(destination.begin(), destination.end(), node));
	}

	// automatically adds destination too
	void Node::addSource(Node* node) {
		source.push_back(node);
		node->addDestination(this);
	}

	// automatically removes destination too
	void Node::removeSource(Node* node) {
		source.erase(std::find(source.begin(), source.end(), node));
		node->removeDestination(this);
	}

	double Node::getTotalDerivative(Node* node, bool reuse) {
		if (node == this) {
			return 1;
		}
		else {
			if (!reuse || derivative[node] == UNDEFINED) {
				derivative[node] = 0;
				for (Node* node : destination) {
					derivative[node] += node->getTotalDerivative(node, reuse) * node->getPartialDerivative(this);
				}
			}
			return derivative[node];
		}
	}

	// Clears previously calculated value
	// Non-function nodes(Variable, Constant, Placeholder) must override this and leave it empty
	void Node::clearValue() {
		value = UNDEFINED;
	}

	// Clears all previously calculated ones(value, derivative)
	// Use this method when any value change occurs on calculation graph(assigning different value to Placeholder, updating value of Variable)
	void Node::clear() {
		for (Node* node : list) {
			node->clearValue();
			for (auto i = node->derivative.begin(); i != node->derivative.end(); i++) {
				i->second = UNDEFINED;
			}
		}
	}

	NodePtr::NodePtr(Node* node) : node(node) {

	}

	NodePtr NodePtr::operator+(NodePtr nodePtr) {
		bool isAdd = dynamic_cast<Add*>(node);
		bool isAddOther = dynamic_cast<Add*>(nodePtr.node);
		if (!isAdd && !isAddOther) {
			Add* rtn = new Add();
			rtn->addSource(node);
			rtn->addSource(nodePtr.node);
		}
		else if (isAdd && !isAddOther) {
			node->addSource(nodePtr.node);
			return *this;
		}
		else if (!isAdd && isAddOther) {
			nodePtr.node->addSource(node);
			return nodePtr;
		}
		else if (isAdd && isAddOther) {
			// Merge two Add node

		}
	}

}
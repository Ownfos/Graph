#include "node.h"
#include "operator.h"

namespace graph {

	std::vector<Node*> Node::list;

	/*
		Keeps pointer of every Node instance created
		This list is used on clear() to reset 'value' and 'derivative' of all nodes
	*/
	Node::Node() : value(UNDEFINED) {
		list.push_back(this);
	}

	void Node::addDestination(Node* other) {
		if (std::find(other->destination.begin(), other->destination.end(), this) == other->destination.end()) {
			other->destination.push_back(this);
			other->derivative[this] = UNDEFINED;
		}
	}

	void Node::removeDestination(Node* other) {
		if (std::find(other->destination.begin(), other->destination.end(), this) != other->destination.end()) {
			other->destination.erase(std::find(other->destination.begin(), other->destination.end(), this));
			other->derivative.erase(other->derivative.find(this));
		}
	}

	/*
		Differentiate node with respect to this node and return the derivative using chain rule
		The first part of this function handles differentiating with respect to itself
	*/
	double Node::getTotalDerivative(Node* node, bool reuse) {
		if (node == this) {
			return 1;
		}
		else {
			if (!reuse || derivative[node] == UNDEFINED) {
				derivative[node] = 0;
				for (Node* output : destination) {
					derivative[node] += output->getTotalDerivative(node, reuse) * output->getPartialDerivative(this, reuse);
				}
			}
			return derivative[node];
		}
	}

	/*
		Clears all previously calculated ones(value, derivative)
		Use this method when any value change occurs on calculation graph(assigning different value to Placeholder, updating value of Variable)
	*/
	void Node::clear() {
		for (Node* node : list) {
			node->clearValue();
			for (auto i = node->derivative.begin(); i != node->derivative.end(); i++) {
				i->second = UNDEFINED;
			}
		}
	}


	void IndependentNode::clearValue() {
		// Do nothing
	}

	double IndependentNode::getValue(bool reuse) {
		return value;
	}

	double IndependentNode::getPartialDerivative(Node* node, bool reuse) {
		return (node == this);
	}


	void DependentNode::clearValue() {
		value = UNDEFINED;
	}

	void DependentNode::addSource(Node* other) {
		source.push_back(other);
	}

	void DependentNode::removeSource(Node* other) {
		source.erase(std::find(source.begin(), source.end(), other));
	}

	void DependentNode::connect(Node* other) {
		addSource(other);
		addDestination(other);
	}

	void DependentNode::disconnect(Node* other) {
		removeSource(other);
		removeDestination(other);
	}

	/*
		Merges the source of 'other' to 'this' and delete 'other'
		Note that previously created nodes using 'other' as source can be seriouly affected
	*/
	void DependentNode::mergeSource(DependentNode* other) {
		for (Node* input : other->source) {
			connect(input);
			other->disconnect(input);
		}
		delete other;
	}


	NodePtr::NodePtr(Node* node) : node(node) {

	}

	double NodePtr::getValue(bool reuse) {
		return node->getValue(reuse);
	}

	double NodePtr::getDerivative(NodePtr other, bool reuse) {
		return node->getTotalDerivative(other.node, reuse);
	}

	/*
		Creates a new Add node with two input

		Examples :
			Add(a,b,c) + Multiply(d,e,f) = Add(Add(a,b,c), Multiply(d,e,f))
			Value(1) + Add(a,b) = Add(Value(1), Add(a,b)
	 */
	NodePtr NodePtr::operator+(NodePtr other) {
		Add* rtn = new Add();
		rtn->connect(node);
		rtn->connect(other.node);
		return rtn;// Implicit typecasting occurs
	}

	/*
		Creates a new Multiply node with two input

		Examples :
			Add(a,b,c) * Multiply(d,e,f) = new Multiply(n1(a,b,c), n2(d,e,f))
			Value(1) + Add(a,b) = new Multiply(Value(1), Add(a,b))
	*/
	NodePtr NodePtr::operator*(NodePtr other) {
		Multiply* rtn = new Multiply();
		rtn->connect(node);
		rtn->connect(other.node);
		return rtn;// Implicit typecasting occurs
	}

	/*
	Merges two node as one Add node
	Note that new Add will not be created unless both are not Add

	Examples :
	Add(a,b,c) || Add(d,e,f) = Add(a,b,c,d,e,f)	//the latter Add(d,e,f) will be destroyed
	Add(a,b,c) || Multiply(d,e,f) = Add(a,b,c,Multiply(d,e,f))
	Value(1) || Multiply(a,b) = new Add(Value(1), Multiply(a,b))
	*/
	NodePtr operator||(NodePtr node1, NodePtr node2) {
		if (node1.node == node2.node) {
			return node1 + node2;
		}
		else {
			Add* add1 = dynamic_cast<Add*>(node1.node);
			Add* add2 = dynamic_cast<Add*>(node2.node);

			if (add1 == nullptr) {
				if (add2 == nullptr) {
					return (node1 + node2);
				}
				else {
					add2->connect(node2.node);
					return add2;
				}
			}
			else {
				if (add2 == nullptr) {
					add1->connect(node2.node);
				}
				else {
					add1->mergeSource(add2);
				}
				return add1;
			}
		}
	}

	/*
		Merges two node as one Multiply node
		Note that new Multiply will not be created unless both are not Multiply

		Examples :
			Multiply(a,b,c) && Multiply(d,e,f) = Multiply(a,b,c,d,e,f)	//the latter Multiply(d,e,f) will be destroyed
			Multiply(a,b,c) && Add(d,e,f) = Multiply(a,b,c,Add(d,e,f))
	*/
	NodePtr operator&&(NodePtr node1, NodePtr node2) {
		if (node1.node == node2.node) {
			return node1 * node2;
		}
		else {
			Multiply* multiply1 = dynamic_cast<Multiply*>(node1.node);
			Multiply* multiply2 = dynamic_cast<Multiply*>(node2.node);

			if (multiply1 == nullptr) {
				if (multiply2 == nullptr) {
					return (node1 * node2);
				}
				else {
					multiply2->connect(node1.node);
					return multiply2;
				}
			}
			else {
				if (multiply2 == nullptr) {
					multiply1->connect(node2.node);
				}
				else {
					multiply1->mergeSource(multiply2);
				}
				return multiply1;
			}
		}
	}
}
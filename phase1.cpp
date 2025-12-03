#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

struct Parser {
    const vector<string>& tokens;
    size_t pos = 0;

    Parser(const vector<string>& t) : tokens(t) {}

    bool end() const { return pos >= tokens.size(); }
    const string& peek() const { return tokens[pos]; }
    const string& next() { return tokens[pos++]; }

    Node* parseStart() {
        Node* root = parseOp1Term();
        if (!end()) throw runtime_error("Unexpected tokens after expression");
        return root;
    }

    Node* parseOp1Term() {
        Node* node = parseOp2Term();
        while (!end() && (peek() == "+" || peek() == "-")) {
            string op = next();
            Node* right = parseOp2Term();
            Node* parent = new Node(op);
            parent->left = node;
            parent->right = right;
            node = parent;
        }
        return node;
    }

    Node* parseOp2Term() {
        Node* node = parseFactor();
        while (!end() && (peek() == "*" || peek() == "/")) {
            string op = next();
            Node* right = parseFactor();
            Node* parent = new Node(op);
            parent->left = node;
            parent->right = right;
            node = parent;
        }
        return node;
    }

    Node* parseFactor() {
        if (peek() == "(") {
            next(); 
            Node* inside = parseOp1Term();
            if (peek() != ")") throw runtime_error("Missing ')'");
            next(); 
            return inside;
        }
        return new Node(next());
    }
};

void printPostorder(Node* root) {
    if (!root) return;
    printPostorder(root->left);
    printPostorder(root->right);
    cout << root->value << " ";
}

int evaluate(Node* root) {
    if (!root) return 0;

    if (isdigit(root->value[0])) {
        return stoi(root->value);
    }

    int leftVal = evaluate(root->left);
    int rightVal = evaluate(root->right);

    if (root->value == "+") return leftVal + rightVal;
    if (root->value == "-") return leftVal - rightVal;
    if (root->value == "*") return leftVal * rightVal;
    if (root->value == "/") {
        if (rightVal == 0) throw runtime_error("Division by zero");
        return leftVal / rightVal;
    }

    throw runtime_error("Unknown operator: " + root->value);
}


int main() {
    vector<string> tokens = {"(", "2", "+", "3", "/", "22", ")", "+", "3"};
    Parser P(tokens);
    Node* root = P.parseStart();

    cout << "Postorder: ";
    printPostorder(root);
    cout << "\n";

    cout << "Result: " << evaluate(root) << "\n";
}

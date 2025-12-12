#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <regex>
#include <cmath>

using namespace std;

string normalize(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    string ans="";
    int n=input.size();
    int i=0;

    while(i<n){
        if(input[i]=='+' || input[i]=='-'){

            int mcount=0;
            int pcount=0;
            while(i<n && (input[i]=='+' || input[i]=='-')){
                if(input[i]=='-') mcount++;
                else pcount++;
                i++;
            }

            char final_sign=(mcount%2==0 ? '+':'-');

            bool not_operation=false;
            bool rad=false;
            if(ans.size()>=3 && ans.substr(ans.size()-3,3)=="√") rad=true;
            else rad=false;

            if(ans.empty()){
                not_operation=true;
            }
            else{
                char last=ans.back();
                if(last=='(' || last=='+' || last=='-' || last=='*' || last=='/' || last=='^' || rad) not_operation=true;
            }
            

            if(!not_operation || final_sign=='-') ans.push_back(final_sign);

            continue;
        }

        ans.push_back(input[i]);
        i++;
        //cout<<"..."<<ans<<'\n';
    }
    //√ = alt + 251

    return ans;
}

bool validate_prantesize(string input){
    stack <char> s;
    bool correct=true;
    int n=input.length();
    for(int i=0;i<n;i++){
        if(input[i]=='(') s.push('(');
        if(s.empty() && input[i]==')'){
            for(int j=i;j<n;j++) if(input[j]==')' && input[j]!='(') cout<<" position "<<j<<" extra \')\' \n";
            correct=false;
        }
        if( !s.empty() && input[i]==')') s.pop();
        
    }
    int temp;
    while(!s.empty()){
        temp=s.top();
        s.pop();
        cout<<"extra \'(\' detected \n";
        correct=false;
    }
    return correct;

}

bool validate_regex(const string& s) {
    regex opration_prantesize("([+\\-*/])\\)");
    if (regex_search(s,opration_prantesize)) return false;

    regex leading_zero("(^|[^0-9])0[0-9]");
    if (regex_search(s, leading_zero)) return false;

    regex number_letter_number("[0-9][A-Za-z][0-9]");
    if (regex_search(s, number_letter_number)) return false;

    regex double_mul_div("[*\\/]{2,}");
    if (regex_search(s, double_mul_div)) return false;

    regex plusminus_bad_after("[+\\-][*\\/]");
    if (regex_search(s, plusminus_bad_after)) return false;

    regex muldiv_bad_after("[*\\/][+\\-]");
    if (regex_search(s, muldiv_bad_after)) return false;

    return true;
}

string get_variables(string s){
    string ans="";
    int n;
    for(int i=0;i<s.length();i++){
        if(isalpha(s[i])){
            cout<<" give the value of "<<s[i]<<" variable \n";
            cin>>n;
            ans+=to_string(n);
        }
        else{
            ans+=s[i];
        }
    }
    return ans;
}

vector<string> tokenize(string s){
    vector<string> tok;
    int n = s.size();
    int i = 0;

    while(i<n){
        if(isdigit(s[i])){
            string num="";
            while(i<n && isdigit(s[i])){
                num+=s[i];
                i++;
            }
            tok.push_back(num);
            continue;
        }
        if(i+2<n && s.substr(i,3)=="√"){
            tok.push_back("√");
            i+=3;
            continue;
        }

        if (s[i]=='(' || s[i]==')' || s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' || s[i]=='^'){
            tok.push_back(string(1,s[i]));
            i++;
            continue;
        }
    }

    return tok;
}

// Expression tree implementation

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
        Node* node = parsePowerTerm();
        while (!end() && (peek() == "*" || peek() == "/")) {
            string op = next();
            Node* right = parsePowerTerm();
            Node* parent = new Node(op);
            parent->left = node;
            parent->right = right;
            node = parent;
        }
        return node;
    }

    Node* parsePowerTerm() {
        Node* node = parseFactor();
        while (!end() && peek() == "^") {
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

        if (peek() == "+" || peek() == "-") {
            string op = next();
            Node* child = parseFactor();
            Node* parent = new Node(op);
            parent->left = child;
            return parent;
        }

        if (peek() == "√") {
            next();
            Node* child = parseFactor();
            Node* parent = new Node("√");
            parent->left = child;
            return parent;
        }

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

    if (!root->value.empty() && isdigit((unsigned char)root->value[0])) {
        return stoi(root->value);
    }

    bool isUnary = (root->left != nullptr && root->right == nullptr);

    if (isUnary) {
        int v = evaluate(root->left);
        if (root->value == "+") return v;
        if (root->value == "-") return -v;
        if (root->value == "√") return (int)std::sqrt(v);
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
    if (root->value == "^") {
        double r = std::pow((double)leftVal, (double)rightVal);
        return (int)r;
    }
    if (root->value == "√") {
        return (int)std::sqrt(leftVal);
    }

    throw runtime_error("Unknown operator: " + root->value);
}

int main() {
    string s;
    getline(cin,s);
    string ans = normalize(s);
    bool result=validate_prantesize(ans);
    bool regex_check=validate_regex(ans);
    string varibale_counted=get_variables(ans);
    if(!regex_check) cout<<"...";
    if(result && regex_check){
        //cout<<varibale_counted;
    }
    vector<string> token=tokenize(varibale_counted);
    for(int i=0;i<token.size();i++){
        //cout<<token[i]<<" ";
    }
    
    Parser P(token);
    Node* root = P.parseStart();

    cout << "Postorder: ";
    printPostorder(root);
    cout << "\n";

    cout << "Result: " << evaluate(root) << "\n";

}
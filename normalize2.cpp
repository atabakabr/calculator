#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <regex>

std::string normalize(std::string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    std::string ans="";
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
        //std::cout<<"..."<<ans<<'\n';
    }
    //√ = alt + 251

    return ans;
}

bool validate_prantesize(std::string input){
    std::stack <char> s;
    bool correct=true;
    int n=input.length();
    for(int i=0;i<n;i++){
        if(input[i]=='(') s.push('(');
        if(s.empty() && input[i]==')'){
            for(int j=i;j<n;j++) if(input[j]==')' && input[j]!='(') std::cout<<" position "<<j<<" extra \')\' \n";
            correct=false;
        }
        if( !s.empty() && input[i]==')') s.pop();
        
    }
    int temp;
    while(!s.empty()){
        temp=s.top();
        s.pop();
        std::cout<<"extra \'(\' detected \n";
        correct=false;
    }
    return correct;

}

bool validate_regex(const std::string& s) {
    std::regex opration_prantesize("([+\\-*/])\\)");
    if (std::regex_search(s,opration_prantesize)) return false;

    std::regex leading_zero("(^|[^0-9])0[0-9]");
    if (std::regex_search(s, leading_zero)) return false;

    std::regex number_letter_number("[0-9][A-Za-z][0-9]");
    if (std::regex_search(s, number_letter_number)) return false;

    std::regex double_mul_div("[*\\/]{2,}");
    if (std::regex_search(s, double_mul_div)) return false;

    std::regex plusminus_bad_after("[+\\-][*\\/]");
    if (std::regex_search(s, plusminus_bad_after)) return false;

    std::regex muldiv_bad_after("[*\\/][+\\-]");
    if (std::regex_search(s, muldiv_bad_after)) return false;

    return true;
}

std::string get_variables(std::string s){
    std::string ans="";
    int n;
    for(int i=0;i<s.length();i++){
        if(isalpha(s[i])){
            std::cout<<" give the value of "<<s[i]<<" variable \n";
            std::cin>>n;
            ans+=std::to_string(n);
        }
        else{
            ans+=s[i];
        }
    }
    return ans;
}

std::vector<std::string> tokenize(std::string s){
    std::vector<std::string> tok;
    int n = s.size();
    int i = 0;

    while(i<n){
        if(isdigit(s[i])){
            std::string num="";
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
            tok.push_back(std::string(1,s[i]));
            i++;
            continue;
        }
    }

    return tok;
}


int main() {
    std::string s;
    std::getline(std::cin,s);
    std::string ans = normalize(s);
    bool result=validate_prantesize(s);
    bool regex_check=validate_regex(s);
    std::string varibale_counted=get_variables(ans);
    if(!regex_check) std::cout<<"...";
    if(result && regex_check){
        //std::cout<<varibale_counted;
    }
    std::vector<std::string> token=tokenize(varibale_counted);
    for(int i=0;i<token.size();i++){
        //std::cout<<token[i]<<" ";
    }

    //in token akhar vorodi ghesmat shoma mishe

}
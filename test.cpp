#include<bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammer;
map<char, set<char>> fir, fol;

set<char> findfir(string s){
    set<char> result;
    if(s == "#" || s.empty()){
        return {'#'};
    }

    for(char c : s){
        if(!grammer.count(c)){
            return{c};
        }
        for(char x : fir[c]){
            if(x != '#'){
                result.insert(x);
            }
        }

        if(!fir[c].count('#'))
            return result;
    }
    result.insert('#');
    return result;
}

int main(){
    int n;
    cout << "enter no of p:";
    cin >> n;

    while(n--){
        string s;
        cin >> s;
        char lhs = s[0];

        string ss = s.substr(3);
        string p = "";
        for(char c : ss){
            if(c == '|'){
                grammer[lhs].push_back(p);
                p = "";
            }
            p+=c;
        }
        grammer[lhs].push_back(p);
    }

    bool change = true;
    while (change) {
        change = false;
        for(auto x : grammer){
            char nt = x.first;
            for(auto p : grammer[nt]){
                set<char> temp = findfir(p);

                for(char c : temp){
                    if(fir[nt].insert(c).second){
                        change = true;
                    }
                }
            }
        }
    }
    
}

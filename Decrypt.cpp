/*
 Modern Cryptography Project 1

 Authors: Jay Patel(N10541249)
          Rajat Pawar(N13295898)
 
*/
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

string dictionary1_lines[200];
string dictionary2_words[100];
set<string> possible_plain_text2;
unordered_map<string,vector<int> > possible_plain_text_map2;
string cipher_text;
int key_length;


void initalize_dictionary();
void generate_string(string prefix);
vector<int> get_key(string cipher_text,string current_line);
int checkDictionary1();
int checkDictionary2();
set<int> create_set(vector<int> key);



int main()
{
    
    initalize_dictionary();
    string kl;
     
    cout << "Enter the cipher text : ";
    getline(cin,cipher_text);
    
    while(true){
        cout << "Enter the Key length : ";
        cin >>kl;
        stringstream int_stream(kl);
        if (int_stream >> key_length)
            break;
    }
    int i=checkDictionary1();
    
    if(i<0){
        int j=checkDictionary2();
    }
    
    
}

// Method to search the dictionary 1

int checkDictionary1(){
    int return_value=-1;
    set<string> possible_plain_texts;
    unordered_map<string,vector<int> > possible_plain_text_map;
    
    int parts = 100/key_length;
    string key_parts[20];
    cout<<"Searching Dictionary 1 ..."<<endl;
    
    for (int cnt=0; cnt<200; cnt++) {
        string current_line=dictionary1_lines[cnt];
        string tmp="";
        
        vector<int> key= get_key(cipher_text,current_line);
        set<int> s1=create_set(key);
        if (s1.size()<=key_length) {
            
            if(possible_plain_texts.find(current_line)==possible_plain_texts.end()){
                possible_plain_texts.insert(current_line);
                possible_plain_text_map.insert(pair<string, vector<int> >(current_line,key));
            }
        }
    }
    
    if(possible_plain_texts.size()==1){
        return_value=0;
        set<string>::iterator iter=possible_plain_texts.begin();
        cout<< "Possible Decrypted text : ";
        cout<<*iter<<endl;
    }
    else if(possible_plain_texts.size()==0){
        return_value=-1;
        cout<<"Not found in Dictionary 1."<<endl;
        cout<<"Searching Dictionary 2 ..."<<endl;
    }
    else{
        return_value=1;
        cout<<"Possible guesses:"<<possible_plain_texts.size()<<endl;
        for ( auto it = possible_plain_text_map.begin(); it != possible_plain_text_map.end(); ++it ){
            cout<< it->first << endl;
            vector<int> key=it->second;
            //for(int i=0;i<key.size();i++){
            //    cout<<key[i]<<",";
            //}
            cout<<endl;
            
        }
    }
    return return_value;
    
}


// Method to search dictionary 2

int checkDictionary2(){
    int return_value=-1;
    generate_string(""); // start with empty string and generate all possible combinations 
    if(possible_plain_text2.size()==1){
        return_value=0;
        set<string>::iterator iter=possible_plain_text2.begin();
        cout<< "Possible Decrypted text : ";
        cout<<*iter<<endl;
    }
    else if(possible_plain_text2.size()==0){
        return_value=-1;
        cout<<"Not found in Dictionary 2."<<endl;
    }
    else{
        
        return_value=1;
        cout<<"Possible guesses:"<<possible_plain_text2.size()<<endl;
        for ( auto it = possible_plain_text_map2.begin(); it != possible_plain_text_map2.end(); ++it ){
            cout<< it->first << endl;
            vector<int> key=it->second;
            //for(int i=0;i<key.size();i++){
            //    cout<<key[i]<<",";
            //}
            //cout<<endl;
        }
    }
    return return_value;
    
}


// Method which generate the possible plain text from the words in dictionary 2

void generate_string(string prefix) {
    if (prefix.length()>100) {
        cout<<"###"<<prefix<<endl;
        return;
    }
    for (int i=0;  i < 100; i++) {
        string newPrefix;
        const string current_word=dictionary2_words[i];
        if (prefix.length()>1) {
            newPrefix.clear();
            const string s=prefix+" "+current_word;
            newPrefix=s;
        }
        else{
            newPrefix=current_word;
        }
        vector<int> key=get_key(cipher_text.substr(0,newPrefix.length()),newPrefix);
        set<int> s1=create_set(key);
        if(s1.size()<=key_length){
            if (newPrefix.length()<=100){
                generate_string(newPrefix);
            }
            else{
                possible_plain_text2.insert(newPrefix.substr(0,100));
                possible_plain_text_map2.insert(pair<string, vector<int> >(newPrefix.substr(0,100),key));
                
            }
        }
        
    }
    
}

// Method to create a set from vector

set<int> create_set(vector<int> key){
    
    set<int> unique_key;
    for (int i=0; i<key.size(); i++) {
        
        if(unique_key.find(key[i])==unique_key.end()){
            unique_key.insert(key[i]);
        }
    }
    return unique_key;
    
}

// Method to generate a key from cipher text and plain text

vector<int> get_key(string cipher_text,string current_line){
    
    vector<int> key;
    
    for (int cnt=0; cnt<cipher_text.length(); cnt++) {
        char a=cipher_text.at(cnt);
        char b=current_line.at(cnt);
        int temp;
        
        if(a==' ' && b==' '){
            temp = 0;
        }
        else if(a==' '){
            temp = 0 - (b%96);
        }
        else if(b==' '){
            temp = (a%96) - 0;
        }
        else{
            temp = a - b;
        }
        if (temp<0){
            temp+=27;
        }
        key.push_back(temp);
    }
    
    return key;
}

// Method to initialize two dictionaries

void initalize_dictionary(){
    
    ifstream infile;
    infile.open("Dictionary1.txt");
    int cnt=0;
    while (!infile.eof())
    {
        string str;
        getline(infile, dictionary1_lines[cnt]);
        for (int i=0; i<dictionary1_lines[cnt].length(); i++) {
            if(isprint(dictionary1_lines[cnt].at(i))){
                str+=dictionary1_lines[cnt].at(i);
            }
        }
        dictionary1_lines[cnt]=str;
        cnt++;
    }
    infile.close();
    
    infile.open("Dictionary2.txt");
    cnt=0;
    while (!infile.eof())
    {
        string str;
        getline(infile, dictionary2_words[cnt]);
        for (int i=0; i<dictionary2_words[cnt].length(); i++) {
            if(isprint(dictionary2_words[cnt].at(i))){
                str+=dictionary2_words[cnt].at(i);
            }
        }
        dictionary2_words[cnt]=str;
        cnt++;
    }
    infile.close();
    
}

// Method to generate the test cipher text
/*

 void generate_cipher_text(string plain_text,string key){
 
 string plain_text="awesomeness hearkened aloneness beheld courtship swoops memphis attentional pintsized rustics hermen";
 //"is to actually implement them the remediation may consist of corrections inspections and notificatio";
 //"asures can be used in characterizing relevance across a set of application domains to support data q";
 string key="byd xabcdefghijkl";
 //cout<<cipher_text.length();
 char pt[100];
 for(int i=0;i<100;i++){
 
    char a=cipher_text.at(i);
    char k=key.at(rand()%5);
 
    char temp;
    //cout<<"*"<<rand()<<"*";
    if(a==' ' && k==' '){
        temp=0;
    }
    else if(a==' '){
        temp=k%96;
    }
    else if(k==' '){
        temp=a%96;
    }
    else{
        temp=(a%96)+(k%96);
    }
    if(temp==0){
        temp=32;
    }
    else if(temp>26){
        if (temp%27==0) {
            temp=32;
        } 
        else {
            temp=(temp%27)+96;
        }
 
    }
    else{
        temp=temp+96;
    }
    pt[i]=temp;
    cout<<pt[i];
 }

 
 }
 
 
 
*/


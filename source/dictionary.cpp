//INCLUDED LIBRARIES
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string.h>
#include <fstream>
#include <map>

#define MAX_LENGTH 45


using namespace std;

//CLASS TRIE
class trie
{
  //STRUCTURE OF NODE
  struct node
  {
    map<char, node*> child;
    bool isEnd;
  };

  node* head;
  string ar;
  int count;
  int created;
  int letters;

public:

  trie()
  {
    ar="abcdefghijklmnopqrstuvwxyz\'";
    head=new node();
    head->isEnd=false;
    created=1;
    letters=0;
  }
  ~trie()
  {
    cout << "<*******************************************>\n";
    cout << endl;
    cout << "Total letters: " << letters << endl;
    cout << "Total nodes created: " << created << endl;
    if(letters<created)
      cout << "Memory saved: " << 0 << endl;
    else
      cout << "Memory saved: " << letters-created << endl;
    cout << "Total nodes deleted: " << count << endl;
  }

  void insert(string &s)
  {
    int len = s.size();
    letters+=len;
    node* current = head;
    char temp;
    for(int i=0; i<len; i++)
    {
      temp=tolower(s[i]);
      if(current->child[temp]==NULL)
      {
        current->child[temp]=new node();
        created++;
        current->child[temp]->isEnd=false;
      }
      current=current->child[temp];
    }
    current->isEnd=true;
  }

  void erase_all()
  {
    count=0;
    del_node(head);

  }

  void del_node(node* p)
  {
    char ch;
    for(int z=0; z<=26; z++)
    {
      ch=ar[z];
      if(p->child[ch]!=NULL)
        del_node(p->child[ch]);
    }
    count++;
    delete p;
    return;
  }

  bool search(string &s)
  {
    int len = s.size();
    node* current = head;
    char temp;
    for(int i=0; i<len; i++)
    {
      temp=tolower(s[i]);
      if(current->child[temp]!=NULL)
      {
        current=current->child[temp];
      }
      else
        return false;
    }
    if(current->isEnd==true)
      return true;
    return false;
  }
};

//FUNCTION PROTOTYPES
bool init(trie &dict);
void check_all(trie &dict);
void append_dict(string &str);
void terminate(trie &dict);
bool str_equal(string &s1, string &s2);
bool is_small(string &s1, string &s2);
bool acceptable(char &ch);
int n_digits(int n);

//MAIN
int main()
{
  cout << "WELCOME TO MISSPELLINGS" << endl << endl;
  int i;
  bool initiated;
  trie dict;

  do
  {
      cout << "1. Check file for misspellings. " << endl
      << "2. Spell check a word." << endl
      << "3. Append a word to the dictionary." << endl
      << "0. Exit." << endl;
      cout << "Choose from the options above: ";
      cin >> i;

      if(i==3)
      {
          //APPEND DICTIONARY
          string str;
          cout << "Enter the word that needs to be added: ";
          cin >> str;
          append_dict(str);
          cout << "<*******************************************>\n";
          cout << endl;

      }
      else if(i>=1&&i<=2)
      {
            if(!initiated)
            {
              //LOAD DICTIONARY
              initiated = init(dict);
              if(!initiated)
              {
                cerr << "DICTIONARY COULD NOT BE LOADED!\n";
                return -1;
              }
            }
        switch(i)
        {
          case 1:
              //CHECKING FILE
              cout << "File \"check.txt\" in the current directory shall be "
              << "tested for MISSPELLINGS." << endl;
              check_all(dict);
              cout << "<*******************************************>\n";
              cout << endl;
              break;

          case 2:
              //SEARCH DICTIONARY
              string str;
              cout << "Enter the word that needs to be checked: ";
              cin >> str;
              bool corr = dict.search(str);
              if(corr)
                cout << "The word is spelt correctly. " << endl;
              else
                cout << "The word has been mis-spelt " << endl;
                cout << "<*******************************************>\n";
                cout << endl;
              break;
        }
      }
  }while(i);

  cout << "All records have been logged." << endl
  << "Thank You. " << endl;

  terminate(dict);

  return 0;
}

bool init(trie &dict)
{
  fstream infile("../dictionaries/dict.txt", ios::in);
  if(!infile.good())
    return false;
  string str;
  while(infile.good())
  {
    infile >> str;
    dict.insert(str);
  }
  infile.close();
  return true;
}

void check_all(trie &dict)
{
  fstream infile("check.txt", ios::in);
  if(!infile.good())
  {
    cerr<<"The file to be checked could not be loaded!" << endl
    << "Please copy the file to be checked in the current directory with name"
    << " \"check.txt\" ";
    cout << endl;
    return;
  }
  fstream log("../log/log.txt", ios::app);
  log << "<*******************************************>\n";
  string str;
  char ch;
  int total_words=0, mis_spelt=0;
  bool spelt_corr = false;
  cout << "WORD No.\tMIS-SPELLINGS:\n";
  int n_sp;
  while(infile.good())
  {
    for(char ch; infile.good();)
    {
      infile.get(ch);
      if(!isalpha(ch))
        break;
      str+=ch;
    }

    int len = str.size();
    if(len<=0)
      continue;
    else if(len>=MAX_LENGTH)
    {
      str.erase(0, len);
      continue;
    }

    spelt_corr=dict.search(str);
    ++total_words;
    if(!spelt_corr)
    {
      n_sp=8-n_digits(total_words);
      ++mis_spelt;
      cout << total_words;
      log << total_words;
      while(n_sp--)
      {
        cout << ' ';
        log << ' ';
      }
      cout << '\t' << str << endl;
      log << str << endl;
    }
    str.erase(0,len);
  }
  cout << "Total MISSPELLINGS: " << mis_spelt << endl;
  log << "Total MISSPELLINGS: " << mis_spelt << endl;
  log << "<------------------------------------------->\n";

  infile.close();
  if(mis_spelt==0)
  {
    cout << "<--NO MIS-SPELLINGS-->\n";
    log << "<--NO MIS-SPELLINGS-->\n";
  }
  cout << "Details have been registered in the log file. \n";
  log.close();
}

void append_dict(string &str)
{
  fstream infile("../dictionaries/dict.txt", ios::in);
  fstream offile("../dictionaries/temp", ios::out);
  string temp;
  infile >> temp;
  while((infile.good())&&(is_small(temp,str)))
  {
    offile << temp << endl;
    infile >> temp;
  }
  if(!str_equal(str, temp))
    offile<<str<<endl;

  while(infile.good())
  {
    //cout << "DONE" << endl;
    offile<<temp<<endl;
    infile >> temp;
  }
  cout << str << " has been added to dictionary." << endl;

  infile.close();
  offile.close();
  remove("../dictionaries/dict.txt");
  rename("../dictionaries/temp","../dictionaries/dict.txt");

}

bool str_equal(string &s1, string &s2)
{
  int l=s1.size();
  if(l==s2.size())
  {
    for(int i=0; i<l; i++)
    {
      if(s1[i]!=s2[i])
        return false;
    }
    return true;
  }
  return false;
}

bool is_small(string &s1, string &s2)
{
  for(int i=0; s1[i]!='\0'&&s2[i]!='\0'; i++)
  {
    if(s2[i]>s1[i])
      return false;
  }
  return true;
}

int n_digits(int n)
{
  int count=0;
  while(n)
  {
    count++;
    n/=10;
  }
  return count;
}

void terminate(trie &dict)
{
  dict.erase_all();
}

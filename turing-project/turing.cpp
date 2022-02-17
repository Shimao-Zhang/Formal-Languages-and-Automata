#include<iostream>
#include<algorithm>
#include<cstring>
#include<set>
#include<vector>
#include<fstream>
#include<sstream>
using namespace std;

set<string> Q;      //the finite set of states
set<string> S;      //the finite set of input symbols
set<string> G;      //the complete set of tape symbols
set<string> F;      //the set of final states
vector<vector<string>> transfer;    //the transition functions
typedef string tape[100000];  //one of the tapes of the Turing Machine
int begin_leftmost; //the index of the first sign in input on the first tape
int begin_rightmost;//the index of the last sign in input on the first tape
string q0;          //the start state
char B;             //the blank symbol
int N=0;            //the number of tapes

string char_to_string(char c)
{
    string a;
    stringstream stream;
    stream << c;
    a=stream.str();
    return a;
}

int split_F(string str,string split_str)
{
    if (str=="")
        return -1;
    char *st=new char[str.length()+1];
    char *d=new char[split_str.size()+1];
    strcpy(st,str.c_str());
    strcpy(d,split_str.c_str());
    char *p=strtok(st,d);
    while (p!=NULL)
    {
        string get=p;
        for(auto item=F.cbegin();item!=F.cend();++item)
        {
            if(Q.count(*item)==0)
            {
                cerr<<"syntax error"<<endl;       //Stderr
                return -1;                  //Error Exit Code
            }
        }
        F.insert(get);
        p=strtok(NULL,d);
    }
    return 0;
}

int split_Q(string str,string split_str)
{
    if (str=="")
        return -1;
    char *st=new char[str.length()+1];
    char *d=new char[split_str.size()+1];
    strcpy(st,str.c_str());
    strcpy(d,split_str.c_str());
    char *p=strtok(st,d);
    while (p!=NULL)
    {
        string get=p;
        for(int i=0;i<get.size();++i)
        {
            if(!isdigit(get[i])&&!isalpha(get[i])&&get[i]!='_')
            {
                cerr<<"syntax error"<<endl;       //Stderr
                return -1;                  //Error Exit Code
            }
        }
        Q.insert(get);
        p=strtok(NULL,d);
    }
    return 0;
}

int split_S(string str,string split_str)
{
    if (str=="")
        return -1;
    char *st=new char[str.length()+1];
    char *d=new char[split_str.size()+1];
    strcpy(st,str.c_str());
    strcpy(d,split_str.c_str());
    char *p=strtok(st,d);
    while (p!=NULL)
    {
        string get=p;
        if(strcmp(p," ")==0||strcmp(p,",")==0||strcmp(p,";")==0||strcmp(p,"{")==0||strcmp(p,"}")==0||strcmp(p,"*")==0||strcmp(p,"_")==0)
        {
            cerr<<"syntax error"<<endl;       //Stderr
            return -1;                  //Error Exit Code
        }
        S.insert(get);
        p=strtok(NULL,d);
    }
    return 0;
}

int split_G(string str,string split_str)
{
    if (str=="")
        return -1;
    char *st=new char[str.length()+1];
    char *d=new char[split_str.size()+1];
    strcpy(st,str.c_str());
    strcpy(d,split_str.c_str());
    char *p=strtok(st,d);
    while (p!=NULL)
    {
        string get=p;
        if(strcmp(p," ")==0||strcmp(p,",")==0||strcmp(p,";")==0||strcmp(p,"{")==0||strcmp(p,"}")==0||strcmp(p,"*")==0)
        {
            cerr<<"syntax error"<<endl;       //Stderr
            return -1;                  //Error Exit Code
        }
        G.insert(get);
        p=strtok(NULL,d);
    }
    return 0;
}

int get_tm(string line)
{
    for(int i=0;i<line.size();++i)
    {
        if(line[i]==' ')
            continue;
        else
        {
            line=line.substr(i,line.size()-i);
            break;
        }
    }
    for(int i=0;i<line.size();++i)
    {
        if(line[i]==';')
        {
            line=line.substr(0,i);
            break;
        }
    }
    int loc_last_ascii_can_be_print;
    for(loc_last_ascii_can_be_print=line.size()-1;loc_last_ascii_can_be_print>=0;--loc_last_ascii_can_be_print)
    {
        if(line[loc_last_ascii_can_be_print]>=32&&line[loc_last_ascii_can_be_print]<=126)
        {
            line=line.substr(0,loc_last_ascii_can_be_print+1);
            break;
        }
    }
    line=line.substr(0,loc_last_ascii_can_be_print+1);
    if(line.size()==0)
        return 0;
    else if(line[0]=='#')
    {
        switch(line[1])
        {
            case 'Q':
            {
                int i=2;
                while(line[i]!='{')
                    ++i;
                ++i;
                int len=line.size()-i-1;
                string str=line.substr(i,len);
                if(split_Q(str,",")==-1)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
                break;
            }
            case 'S':
            {
                int i=2;
                while(line[i]!='{')
                    ++i;
                ++i;
                int len=line.size()-i-1;
                string str=line.substr(i,len);
                if(split_S(str,",")==-1)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
                break;
            }
            case 'G':
            {
                int i=2;
                while(line[i]!='{')
                    ++i;
                ++i;
                int len=line.size()-i-1;
                string str=line.substr(i,len);
                if(split_G(str,",")==-1)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
                break;
            }
            case 'q':
            {
                int i=line.find('=');
                i+=2;
                int len=line.size()-i;
                q0=line.substr(i,len);
                if(Q.count(q0)==0)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
                break;
            }
            case 'B':
            {
                int i=line.find('=');
                i+=2;
                B=line[i];
                break;
            }
            case 'F':
            {
                int i=2;
                while(line[i]!='{')
                    ++i;
                ++i;
                int len=line.size()-i-1;
                string str=line.substr(i,len);
                if(split_F(str,",")==-1)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
                break;
            }
            case 'N':
            {
                int i=2;
                while(!isdigit(line[i]))
                    ++i;
                int k=i;
                while(line[k]!=' ')
                    ++k;
                int len=k-i;
                string str=line.substr(i,len);
                for(int m=k;m<line.size();++m)
                {
                    if(line[m]!=' ')
                    {
                        cerr<<"syntax error"<<endl;       //Stderr
                        return -1;                  //Error Exit Code
                    }
                }
                for(int count=0;count<str.size();++count)
                {
                    if(!isdigit(str[count]))
                    {
                        cerr<<"syntax error"<<endl;       //Stderr
                        return -1;                  //Error Exit Code
                    }
                }
                int num=stoi(str);
                N=num;
                break;
            }
            default:
            {
                cerr<<"syntax error"<<endl;       //Stderr
                return -1;                  //Error Exit Code
                break;
            }
        }
    }
    else if(line[0]==';')
        return 0;
    else                            //get transition functions
    {
        vector<string> res;
        char *st=new char[line.length()+2];
        string split_str=" ";
        char *d=new char[split_str.length()+2];
        strcpy(st,line.c_str());
        strcpy(d,split_str.c_str());
        int a=0;
        for(a=0;st[a]!='\0'&&st[a]!=';';++a){}
        st[a]='\0';
        char *p=strtok(st,d);
        int count=0;
        while (p!=NULL)
        {
            string ans=p;
            if(count==0)
            {
                if(Q.count(ans)==0)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
            }
            else if(count==1)
            {
                for(int i=0;i<ans.size();++i)
                {
                    if(G.count(char_to_string(ans[i]))==0)
                    {
                        cerr<<"syntax error"<<endl;       //Stderr
                        return -1;                  //Error Exit Code
                    }
                }
            }
            else if(count==2)
            {
                for(int i=0;i<ans.size();++i)
                {
                    if(G.count(char_to_string(ans[i]))==0)
                    {
                        cerr<<"syntax error"<<endl;       //Stderr
                        return -1;                  //Error Exit Code
                    }
                }
            }
            else if(count==3)
            {
                for(int i=0;i<ans.size();++i)
                {
                    if(ans[i]!='l'&&ans[i]!='r'&&ans[i]!='*')
                    {
                        cerr<<"syntax error"<<endl;       //Stderr
                        return -1;                  //Error Exit Code
                    }
                }
            }
            else if(count==4)
            {
                if(Q.count(ans)==0)
                {
                    cerr<<"syntax error"<<endl;       //Stderr
                    return -1;                  //Error Exit Code
                }
            }
            else if(count>=5)
            {
                cerr<<"syntax error"<<endl;       //Stderr
                return -1;                  //Error Exit Code
            }
            res.push_back(ans);
            p=strtok(NULL,d);
            ++count;
        }
        transfer.push_back(res);
    }
    return 0;
}

int get_next_step(string old_state,string* old_sign,string &new_state,string new_sign[],char* direction)
{
    int flag=0;
    int loc;
    for (int i=0;i<transfer.size();++i)
    {
        if(transfer[i][0]==old_state)
        {
            for(int k=0;k<N;++k)
            {
                if(transfer[i][1][k]!=old_sign[k][0])
                    break;
                else if(k==N-1)
                {
                    flag=1;
                    loc=i;
                }
            }
        }
    }
    if(!flag)
        return -1;
    else
    {
        new_state=transfer[loc][4];
        for(int i=0;i<N;++i)
        {
            new_sign[i]=transfer[loc][2][i];
            direction[i]=transfer[loc][3][i];
        }
    }
    return 0;
}

void print_first_tape(tape* all_tape)
{
    int leftmost=0;
    int rightmost=99999;
    for(;leftmost<rightmost;++leftmost)
    {
        if(all_tape[0][leftmost] != char_to_string(B))
        {
            break;
        }
    }
    for(;rightmost>leftmost;--rightmost)
    {
        if(all_tape[0][rightmost] != char_to_string(B))
        {
            break;
        }
    }
    if(all_tape[0][rightmost]== char_to_string(B))
    {
        cout<<"";
    }
    else
    {
        for(int i=leftmost;i<=rightmost;++i)
        {
            cout<<all_tape[0][i];
        }
    }
}

int get_left(tape tape,int head)
{
    int left=0;
    for(left=0;left<head && tape[left]==char_to_string(B);++left);
    return left;
}

int get_right(tape tape,int head)
{
    int right=99999;
    for(right=99999;right>head && tape[right]==char_to_string(B);--right);
    return right;
}

int run_on_tm(string input)
{
    for(int i=0;i<input.size();++i)
    {
        string a;
        stringstream stream;
        stream<<input[i];
        a=stream.str();
        if(S.count(a)==0)
        {
            cerr<<"illegal input"<<endl;    //Stderr
            return -1;                  //Error Exit Code
        }
    }
    tape* all_tape=new tape[N];
    for(int i=0;i<N;++i)
    {
        for (int k = 0; k < 100000; ++k)
        {
            all_tape[i][k] = B;
        }
    }
    int leftmost=50000-input.size()/2;
    int rightmost=leftmost+input.size()-1;
    for(int i=0;i<input.size();++i)
    {
        all_tape[0][leftmost+i]= char_to_string(input[i]);
    }
    int *head=new int[N];
    head[0]=leftmost;
    for(int i=1;i<N;++i)
    {
        head[i]=25000;
    }
    string old_state=q0;
    string new_state;
    string *old_sign=new string[N];
    old_sign[0]= char_to_string(input[0]);
    for(int i=1;i<N;++i)
    {
        old_sign[i]= char_to_string(B);
    }
    string *new_sign=new string[N];
    char *direction=new char[N];
    while(F.count(old_state)==0)
    {
        if(get_next_step(old_state, old_sign, new_state, new_sign, direction)==-1)
        {
            print_first_tape(all_tape);
            cout<<endl;
            return 0;
        }
        old_state=new_state;
        for(int i=0;i<N;++i)
        {
            all_tape[i][head[i]]=new_sign[i];
        }
        for(int i=0;i<N;++i)
        {
            if(direction[i]=='l')
                head[i]-=1;
            else if(direction[i]=='r')
                head[i]+=1;
            else if(direction[i]=='*')
                head[i]=head[i];
        }
        for(int i=0;i<N;++i)
        {
            old_sign[i]=all_tape[i][head[i]];
        }
    }
    print_first_tape(all_tape);
    cout<<endl;
    return 0;
}

void print_id(int step,tape* all_tape,int* head,string state)
{
    cout<<"Step   : "<<step<<endl;
    for(int i=0;i<N;++i)
    {
        cout<<"Index"<<i<<" : ";
        int left=get_left(all_tape[i],head[i]);
        int right=get_right(all_tape[i],head[i]);
        if(i==0)
        {
            for(int k=left;k<=right;++k)
            {
                cout<<abs(k-begin_leftmost)<<' ';
            }
        }
        else
        {
            for(int k=left;k<=right;++k)
            {
                cout<<abs(k-25000)<<' ';
            }
        }
        cout<<endl;
        cout<<"Tape"<<i<<"  : ";
        for(int k=left;k<=right;++k)
        {
            cout<<all_tape[i][k]<<' ';
        }
        cout<<endl;
        cout<<"Head"<<i<<"  : ";
        for(int k=1;k<=head[i]-left;++k)
        {
            cout<<"  ";
        }
        cout<<'^'<<endl;
    }
    cout<<"State  : "<<state<<endl;
    cout<<"-------------------------------------------------"<<endl;
}

int run_on_tm_v(string input)
{
    cout<<"Input: "<<input<<endl;
    for(int i=0;i<input.size();++i)
    {
        string a;
        stringstream stream;
        stream<<input[i];
        a=stream.str();
        if(S.count(a)==0)
        {
            cerr<<"========================= ERR ========================="<<endl;
            cerr<<"error: '"<<a<<"' was not declared in the set of input symbols"<<endl;
            cout<<"Input: "<<input<<endl;
            for(int k=1;k<=7+i;++k)
                cerr<<' ';
            cerr<<'^'<<endl;
            cerr<<"========================= END ========================="<<endl;
            return -1;                  //Error Exit Code
        }
    }
    cout<<"========================= RUN ========================="<<endl;
    tape* all_tape=new tape[N];
    for(int i=0;i<N;++i)
    {
        for (int k = 0; k < 100000; ++k)
        {
            all_tape[i][k] = B;
        }
    }
    begin_leftmost=50000-input.size()/2;
    begin_rightmost=begin_leftmost+input.size()-1;
    for(int i=0;i<input.size();++i)
    {
        all_tape[0][begin_leftmost+i]= char_to_string(input[i]);
    }
    int *head=new int[N];
    head[0]=begin_leftmost;
    for(int i=1;i<N;++i)
    {
        head[i]=25000;
    }
    string old_state=q0;
    string new_state;
    string *old_sign=new string[N];
    old_sign[0]= char_to_string(input[0]);
    for(int i=1;i<N;++i)
    {
        old_sign[i]= char_to_string(B);
    }
    string *new_sign=new string[N];
    char *direction=new char[N];
    int step=0;
    print_id(step,all_tape,head,old_state);
    while(F.count(old_state)==0)
    {
        if(get_next_step(old_state, old_sign, new_state, new_sign, direction)==-1)
        {
            cout<<"Result: ";
            print_first_tape(all_tape);
            cout<<endl;
            cout<<"========================= END ========================="<<endl;
            return 0;
        }
        old_state=new_state;
        for(int i=0;i<N;++i)
        {
            all_tape[i][head[i]]=new_sign[i];
        }
        for(int i=0;i<N;++i)
        {
            if(direction[i]=='l')
                head[i]-=1;
            else if(direction[i]=='r')
                head[i]+=1;
            else if(direction[i]=='*')
                head[i]=head[i];
        }
        for(int i=0;i<N;++i)
        {
            old_sign[i]=all_tape[i][head[i]];
        }
        ++step;
        print_id(step,all_tape,head,old_state);
    }
    cout<<"Result: ";
    print_first_tape(all_tape);
    cout<<endl;
    cout<<"========================= END ========================="<<endl;
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc==1)
    {
        return 0;
    }
    else if(argc==2)
    {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        {
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
            return 0;
        }
        else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0)
        {
            cerr<<"illegal instruction"<<endl;    //Stderr
            return -1;                      //Error Exit Code
        }
        else
        {
            cerr<<"illegal instruction"<<endl;    //Stderr
            return -1;                      //Error Exit Code
        }
    }
    else if(argc==3)
    {
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        {
            cerr<<"illegal instruction"<<endl;    //Stderr
            return -1;                      //Error Exit Code
        }
        string FileName=argv[1];
        string address="../programs/"+FileName;
        ifstream f;
        f.open(address);
        string get_line;
        if(!f)
        {
            cerr<<"File open fails!"<<endl;     //Stderr
            return -1;                      //Error Exit Code
        }
        else
        {
            while(getline(f,get_line))
            {
                if(get_tm(get_line)==-1)
                {
                    return -1;
                }
            }
            f.close();
            string input;
            input=argv[2];
            int flag=run_on_tm(input);
            if(flag==-1)
                return -1;          //Error Exit Code
        }
    }
    else if(argc==4)
    {
        if(strcmp(argv[1],"-v")!=0&&strcmp(argv[1], "--verbose")!=0)
        {
            cerr<<"illegal instruction"<<endl;    //Stderr
            return -1;                      //Error Exit Code
        }
        else if(strcmp(argv[1],"-v")==0 || strcmp(argv[1], "--verbose")==0)
        {
            string FileName=argv[2];
            string address="../programs/"+FileName;
            ifstream f;
            f.open(address);
            string get_line;
            if(!f)
            {
                cerr<<"File open fails!"<<endl;     //Stderr
                return -1;                      //Error Exit Code
            }
            else
            {
                while (getline(f, get_line))
                {
                    if(get_tm(get_line)==-1)
                    {
                        return -1;
                    }
                }
                f.close();
                string input;
                input = argv[3];
                int flag=run_on_tm_v(input);
                if(flag==-1)
                    return -1;          //Error Exit Code
            }
        }
    }
    else if(argc>4)
    {
        cerr<<"illegal instruction"<<endl;    //Stderr
        return -1;                      //Error Exit Code
    }
    return 0;
}
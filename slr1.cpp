#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

char T[20]={};int tt;
char V[20]={};int tv;
int goto_table[100][100];
int comb[20][20];		//parsing table with only numbers to represent shift and reduce used for parsing input
char follow[20][20];	//to hold follow of non-terminals
char first[20][20];		//to hold first of non-terminals

struct state{
    int tp;
    char P[100][100];
};

struct myStack{
	char ch;
	int no;
} stack[20];
int top=0,pop_no;
char pop_ch;

void add_dots(struct state *I){
    for(int i=0;i<I->tp;i++){
        for (int j=99;j>3;j--)
            I->P[i][j] = I->P[i][j-1];
        I->P[i][3]='.';
    }
}

void augument(struct state *S,struct state *I){
    if(I->P[0][0]=='S')
        strcpy(S->P[0],"Z->.S");
    else{
        strcpy(S->P[0],"S->.");
        S->P[0][4]=I->P[0][0];}
    S->tp++;
}

void get_prods(struct state *I){
    cout<<"Enter the number of non terminals:"<<endl;
    cin>>tv;
    cout<<"Enter the non terminals one by one:"<<endl;
    for(int i=0;i<tv;i++)
        cin>>V[i];
    cout<<"Enter the number of terminals:"<<endl;
    cin>>tt;
    cout<<"Enter the terminals (single lettered) one by one:"<<endl;
    for(int i=0;i<tt;i++)
        cin>>T[i];
    cout<<"Enter the number of productions:\n";
    cin>>I->tp;
    cout<<"Enter the productions one by one in form (S->ABc):\n";
    for(int i=0;i<I->tp;i++){
        cin>>I->P[i];
    }
}

bool is_non_terminal(char a){
   if (a >= 'A' && a <= 'Z')
        return true;
    else
        return false;
}

bool in_state(struct state *I,char *a){
    for(int i=0;i<I->tp;i++){
        if(!strcmp(I->P[i],a))
            return true;
    }
    return false;
}

char char_after_dot(char a[100]){
    char b;
    for(int i=0;i<strlen(a);i++)
        if(a[i]=='.'){
            b=a[i+1];
            return b;}

}

char* move_dot(char b[100],int len){
    char a[100];
    strcpy(a,b);
    for(int i=0;i<len;i++){
        if(a[i]=='.'){
            swap(a[i],a[i+1]);
            break;
        }
    }
    return &a[0];
}

bool same_state(struct state *I0,struct state *I){

    if (I0->tp != I->tp)
        return false;

    for (int i=0; i<I0->tp; i++)
    {
        int flag = 0;
        for (int j=0; j<I->tp; j++)
            if (strcmp(I0->P[i], I->P[j]) == 0)
                flag = 1;
        if (flag == 0)
            return false;
    }
    return true;

}

void closure(struct state *I,struct state *I0){
    char a;
    for(int i=0;i<I0->tp;i++){
        a=char_after_dot(I0->P[i]);
        if(is_non_terminal(a)){
            for(int j=0;j<I->tp;j++){
                if(I->P[j][0]==a){
                    if(!in_state(I0,I->P[j])){
                        strcpy(I0->P[I0->tp],I->P[j]);
                        I0->tp++;
                    }
                }
            }
        }
    }
}

void goto_state(struct state *I,struct state *S,char a){
    int time=1;
    for(int i=0;i<I->tp;i++){
        if(char_after_dot(I->P[i])==a){
                if(time==1){
                    time++;
                }
                strcpy(S->P[S->tp],move_dot(I->P[i],strlen(I->P[i])));
                S->tp++;
        }
    }
}

void print_prods(struct state *I){
    for(int i=0;i<I->tp;i++)
        printf("%s\n",I->P[i]);
    cout<<endl;
}

bool in_array(char a[20],char b){
    for(int i=0;i<strlen(a);i++)
        if(a[i]==b)
            return true;
    return false;
}

char* chars_after_dots(struct state *I){
    char a[20]={};
    for(int i=0;i<I->tp;i++){
        if(!in_array(a,char_after_dot(I->P[i]))){
                a[strlen(a)]=char_after_dot(I->P[i]);
            }
    }
    return &a[0];
}

void cleanup_prods(struct state * I){
    char a[100]={};
    for(int i=0;i<I->tp;i++)
        strcpy(I->P[i],a);
    I->tp=0;
}

int return_index(char a){
    for(int i=0;i<tt;i++)
        if(T[i]==a)
            return i;
    for(int i=0;i<tv;i++)
        if(V[i]==a)
            return tt+i;
}

int get_index(char c,char *a){
for(int i=0;i<strlen(a);i++)
    if(a[i]==c)
    return i;
}

void add_dot_at_end(struct state* I){
    for(int i=0;i<I->tp;i++){
        strcat(I->P[i],".");
    }
}

void add_to_first(int n,char b){
   for(int i=0;i<strlen(first[n]);i++)
        if(first[n][i]==b)
            return;
    first[n][strlen(first[n])]=b;
}

void add_to_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(first[m]);j++){
            if(first[n][i]==first[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_first(m,first[n][i]);
    }
}

void add_to_follow(int n,char b){
   for(int i=0;i<strlen(follow[n]);i++)
        if(follow[n][i]==b)
            return;
    follow[n][strlen(follow[n])]=b;
}

void add_to_follow(int m,int n){
    for(int i=0;i<strlen(follow[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(follow[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,follow[n][i]);
    }
}

void add_to_follow_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(first[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,first[n][i]);
    }
}

void find_first(struct state *I){
    for(int i=0;i<tv;i++){
        for(int j=0;j<I->tp;j++){
            if(I->P[j][0]==V[i]){
                if(!is_non_terminal(I->P[j][3])){
                    add_to_first(i,I->P[j][3]);
                    }

            }
        }
    }
}

void find_follow(struct state *I){
    for(int i=0;i<tv;i++){
        for(int j=0;j<I->tp;j++){
            for(int k=3;k<strlen(I->P[j]);k++){
                if(I->P[j][k]==V[i]){
                    if(I->P[j][k+1]!='\0'){
                        if(!is_non_terminal(I->P[j][k+1])){
                            add_to_follow(i,I->P[j][k+1]);
                        }
                    }
                }
            }
        }
    }
}

int get_index(int *arr,int n){
    for(int i=0;i<tt;i++){
        if(arr[i]==n)
            return i;
    }
    return -1;
}

void parsing_table(int state_count,int *tr,struct state *temp1){
    cout<<"\n**********SLR Parsing Table**********\n\n\t";
    int n;
    int arr[state_count][tt]={-1};
    for(int i=0;i<tt;i++)
        cout<<T[i]<<"\t";
    for(int i=0;i<tv;i++)
        cout<<V[i]<<"\t";
    cout<<endl;
    
    for(int i=0;i<state_count;i++){
        int array[tv+tt]={-1};
        for(int j=0;j<state_count;j++){
            if(goto_table[i][j]!='~'){
                    array[return_index(goto_table[i][j])]= j;
            }
        }
        
        if(i<temp1->tp)
        {
        n=tr[i];
    for(int j=0;j<strlen(follow[return_index(temp1->P[i][0])-tt]);j++)
        for(int k=0;k<tt;k++)
            if(follow[return_index(temp1->P[i][0])-tt][j]==T[k])
                arr[n][k]=i+1;
}
        for(int j=0;j<tv+tt;j++){
            if(i==1&&j==tt-1){
                comb[i][j]=99;
            }
            if(array[j]>0&&array[j]<=state_count)
                {
                comb[i][j]=array[j];
            }
        }
    }
    
     for(int i=0;i<state_count;i++)
    {
     cout<<"I"<<i<<"\t";
        for(int j=0;j<tv+tt;j++){
            if(i==1&&j==tt-1){
                cout<<"ACC";
            }
             if(j<tt&&arr[i][j]>0&&arr[i][j]<=state_count)
            {
            cout<<"R"<<arr[i][j];
           comb[i][j]=-arr[i][j];
        }
            if(comb[i][j]>0&&comb[i][j]<=state_count)
                {
            if(j<tt)
                cout<<"S"<<comb[i][j];
            else
                cout<<comb[i][j];
            }
            cout<<"\t";
        }
        cout<<"\n";
    }
}


void push(char val1,int val2)
{
 ++top;
 stack[top].ch=val1;
 stack[top].no=val2;
}

char pop()
{
	 pop_no=stack[top].no;
 pop_ch=stack[top].ch;
 stack[top].ch='\0';
 --top;
}

int get_tindex(char a){
	for(int i=0;i<tt;i++)
	if(T[i]==a)
	return i;
}

int get_vindex(char a){
	for(int i=0;i<tv;i++)
	if(V[i]==a)
	return i;
}

void parse_input(struct state *temp){
		int index0,val;
		char name[20];
		cout<<"Enter the string to parse: ";
		cin>>name;
		string action;
		stack[0].ch='$';
		stack[0].no=0;
		cout<<"\nParsing input: "<<name<<endl;
		name[strlen(name)]='$';
		cout<<"\nStack\t\t\tInput\t\t\tAction\n";
	while(action!="Accept"&&action!="Reject")
	{
			char x=' ';
			index0=get_tindex(name[0]);
			for(int i=0;x!='\0';i++)
			{
				cout<<stack[i].ch<<stack[i].no;
				x=stack[i+1].ch;
			}
			cout<<"\t\t\t"<<name<<"\t\t\t";
			
			pop();
			push(pop_ch,pop_no);
			
			if(comb[pop_no][index0]==0){
				action="Reject";
				cout<<action;
			}
			else if(comb[pop_no][index0]==99){
				action="Accept";
				cout<<action;
			}
			else if(comb[pop_no][index0]>0)
			{
				cout<<"Shift"<<endl;
				push(name[0],comb[pop_no][index0]);
					for(int i=0;i<strlen(name);i++)
					name[i]=name[i+1];
			}
			else
			{
				pop();
				val=pop_no;
				x=temp->P[-comb[val][index0]-1][0];
				cout<<"Reduce by "<<temp->P[-comb[val][index0]-1]<<endl;
				for(int i=0;i<strlen(temp->P[-comb[val][index0]-1])-4;i++)
				pop();
				val=stack[top].no;
				push(x,comb[val][get_vindex(x)+tt]);
			}
		}
	}

int main(){
    struct state init, temp, temp1,I[50];
    int state_count=1;
    get_prods(&init);
    temp=init;
    temp1=temp;
    add_dots(&init);
    for(int i=0;i<100;i++)
        for(int j=0;j<100;j++)
            goto_table[i][j]='~';

    augument(&I[0],&init);
    closure(&init,&I[0]);
    cout<<"Sets of LR(0) items\nI0:\n";
    print_prods(&I[0]);

    char characters[20]={};
    for(int i=0;i<state_count;i++){
        char characters[20]={};
        for(int z=0;z<I[i].tp;z++)
                if(!in_array(characters,char_after_dot(I[i].P[z])))
                characters[strlen(characters)]=char_after_dot(I[i].P[z]);


        for(int j=0;j<strlen(characters);j++){
            goto_state(&I[i],&I[state_count],characters[j]);
            closure(&init,&I[state_count]);
            int flag=0;
            for(int k=0;k<state_count-1;k++){
                if(same_state(&I[k],&I[state_count])){
                    cleanup_prods(&I[state_count]);
					flag=1;
                    cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<k<<".\n";
                    goto_table[i][k]=characters[j];;
                    break;
                }
            }
            if(flag==0){
                state_count++;
                cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<state_count-1<<":\n";
                goto_table[i][state_count-1]=characters[j];
                print_prods(&I[state_count-1]);
            }
        }
    }

    int tr[temp.tp];		//number of reductions possible i.e no of states
    T[tt]='$';tt++;

    add_dot_at_end(&temp1);
    for(int i=0;i<state_count;i++){
        for(int j=0;j<I[i].tp;j++)
            for(int k=0;k<temp1.tp;k++)
                 if(in_state(&I[i],temp1.P[k]))
                        tr[k]=i;
    }

    find_first(&temp);
    for(int l=0;l<tv;l++){
    for(int i=0;i<temp.tp;i++){
        if(is_non_terminal(temp.P[i][3])){
            add_to_first(return_index(temp.P[i][0])-tt,return_index(temp.P[i][3])-tt);
        }
    }}

    find_follow(&temp);
    add_to_follow(0,'$');
    for(int l=0;l<tv;l++){
        for(int i=0;i<temp.tp;i++){
            for(int k=3;k<strlen(temp.P[i]);k++){
                if(temp.P[i][k]==V[l]){
                        if(is_non_terminal(temp.P[i][k+1])){
                            add_to_follow_first(l,return_index(temp.P[i][k+1])-tt);}
                        if(temp.P[i][k+1]=='\0')
                            add_to_follow(l,return_index(temp.P[i][0])-tt);
                            }
                    }
            }
    }
    
    parsing_table(state_count,&tr[0],&temp1);
	parse_input(&temp);
}








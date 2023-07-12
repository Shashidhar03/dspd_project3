#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define previous_orders_value 5         //no.of food items in a restaurant
#define nearby_loc_value 3               //nearby locations
#define max_order_menu_number 3         //max no.of food items in order
#define menu_value 10                   //total no.of food items in menu file
#define time_of_3_months 7776000
#define no_of_restaurants 10             //total no.of restaurants in restaurant file 

struct Rest_node
{
    int restaurant_id;            //data
    char address[100];
    char restaurant_area[100];
	char no_of_seates[100];
	char special_facilities[100];
	char menu[previous_orders_value][100];
	char type_of_restarunt[100];
	char type_of_food[100];
    char near_by_area[3][100];

    int rest_node_height;       //for avl tree
    struct Rest_node *rest_left;
    struct Rest_node *rest_right;
};

struct Agent_node
{
    int agent_id;
	char agent_name[100];
	unsigned long long int agent_phone_no;
	int commission;
    char agent_area[100];
    int agent_status;

    int agent_node_height;
    struct Agent_node *agent_left;
    struct Agent_node *agent_right;
};

struct User_node
{
    int user_id;
	char user_name[100];
	char user_address[100];
	long long int user_phone_no;

    int user_node_height;
    struct User_node *user_left;
    struct User_node *user_right;
};

struct global_order_Node
{
    int customer_id;
	char customer_name[100];
	char customer_address[100];
    char customer_area[100];
	long long int customer_phone_no;
    char order_menu[max_order_menu_number][100]; 
    time_t time_taken;

    struct Agent_node* agents_order_ptr;     //pointer to agent node who is delivering the order

    int global_order_node_height;
    struct global_order_Node* global_order_left;
    struct global_order_Node* global_order_right;
};

typedef struct Rest_node rest_node;
typedef struct Agent_node agent_node;
typedef struct User_node user_node;
typedef struct global_order_Node global_order_node;

int maxi(int a, int b)
{
    int ret_val;
    if(a>b)
    {
        ret_val=a;
    }
    else{
        ret_val=b;
    }
    return ret_val;
}

int get_rest_height(rest_node *root)           //gives height of each node
{
    int ret_val=0;
    if(root!=NULL)
    {
        ret_val= 1+ maxi(get_rest_height(root->rest_left),get_rest_height(root->rest_right));
    }
    return ret_val;
}

int get_rest_node_balance(rest_node* root)         //it gives balance factor of each node
{
    int ret_val;
    ret_val= ( get_rest_height(root->rest_left)-get_rest_height(root->rest_right) );
    return ret_val;
}

rest_node* rotate_rest_left(rest_node* root)
{
    rest_node *x=root->rest_right;
    root->rest_right=x->rest_left;
    x->rest_left=root;
    return x;
}

rest_node* rotate_rest_right(rest_node* root)
{
    rest_node *x=root->rest_left;
    root->rest_left=x->rest_right;
    x->rest_right=root;
    return x;
}

rest_node* insert_rest_node_child(rest_node* root,rest_node *temp)
{
    if(root==NULL)
    {
        root=temp;
        root->rest_node_height=1;
    }
    else if(temp->restaurant_id<root->restaurant_id)
    {
        root->rest_left=insert_rest_node_child(root->rest_left,temp);
        root->rest_node_height=get_rest_height(root);
    }
    else if(temp->restaurant_id>root->restaurant_id)
    {
        root->rest_right=insert_rest_node_child(root->rest_right,temp);
        root->rest_node_height=get_rest_height(root);
    }
 
    int balance=get_rest_node_balance(root);
	
	if(balance>1 && temp->restaurant_id < root->rest_left->restaurant_id)
	{
		root=rotate_rest_right(root);
	}
	else if(balance>1 && temp->restaurant_id>root->rest_left->restaurant_id)
	{
		root->rest_left=rotate_rest_left(root->rest_left);
		root=rotate_rest_right(root);
	}
	else if(balance<-1 && temp->restaurant_id>root->rest_right->restaurant_id)
	{
		root=rotate_rest_left(root);
	}
	else if(balance<-1 && temp->restaurant_id<root->rest_right->restaurant_id)
	{
		root->rest_right=rotate_rest_right(root->rest_right);
		root=rotate_rest_left(root);
	}
    return root;
}

rest_node* insert_rest_node(rest_node *root)
{
    FILE* fp;
	fp=fopen("restaturant.csv","r");

	char line[1024];
	char *sp;
	while(fgets(line,1024,fp)!=NULL)
    {
        rest_node* temp=(rest_node*)malloc(sizeof(rest_node));       //we store data from files in temp node and then insert it in tree

        sp=strtok(line,",");
        temp->restaurant_id=atoi(sp);

        sp=strtok(NULL,",");
        strcpy(temp->address,sp);

        sp=strtok(NULL,",");
        strcpy(temp->restaurant_area,sp);
    
        sp=strtok(NULL,",");
        strcpy(temp->no_of_seates,sp);
    
        sp=strtok(NULL,",");
        strcpy(temp->special_facilities,sp);

        for(int i=0;i<previous_orders_value;i++)
        {
            sp=strtok(NULL,",");
            strcpy(temp->menu[i],sp);
        }
    
        sp=strtok(NULL,",");
        strcpy(temp->type_of_restarunt,sp);
    
        sp=strtok(NULL,",");
        strcpy(temp->type_of_food,sp);

        for(int i=0;i<nearby_loc_value;i++)
        {
            sp=strtok(NULL,",");
            strcpy(temp->near_by_area[i],sp);
        }

        root=insert_rest_node_child(root,temp);       //inserting node in AVL tree
    }
    fclose(fp);
    return root;
}

void visit_restaurant(rest_node *root)
{
    if(root!=NULL)
    {
        visit_restaurant(root->rest_left);
        printf("restaurant id :%d\n",root->restaurant_id);
        printf("restaurant address : %s\n",root->address);
        printf("restaurant area : %s\n",root->restaurant_area);
        printf("restaurant type : %s\n",root->type_of_restarunt);
        printf("type of food served in restaurant: %s\n",root->type_of_food);
        printf("special facilities in restaurant : %s\n",root->special_facilities);
        printf("number of seates in restaurant : %s\n",root->no_of_seates);
        visit_restaurant(root->rest_right);
    }
}

int get_agent_height(agent_node *root)
{
    int ret_val=0;
    if(root!=NULL)
    {
        ret_val=1 + maxi(get_agent_height(root->agent_left),get_agent_height(root->agent_right));
    }
    return ret_val;
}

int get_agent_node_balance(agent_node* root)
{
    int ret_val;
    ret_val= ( get_agent_height(root->agent_left) - get_agent_height(root->agent_right) );
    return ret_val;
}

agent_node* rotate_agent_left(agent_node* root)
{
    agent_node *x=root->agent_right;
    root->agent_right=x->agent_left;
    x->agent_left=root;
    return x;
}

agent_node* rotate_agent_right(agent_node* root)
{
    agent_node *x=root->agent_left;
    root->agent_left=x->agent_right;
    x->agent_right=root;
    return x;
}

agent_node* insert_agent_node_child(agent_node* root,agent_node* temp)
{
    
    if(root==NULL)
    {
        root=temp;
        root->agent_node_height=1;
        root->agent_status=0;                               //indicates agent is free
    }
    else if(temp->agent_id<root->agent_id)
    {
        root->agent_left=insert_agent_node_child(root->agent_left,temp);
    }
    else if(temp->agent_id>root->agent_id)
    {
        root->agent_right=insert_agent_node_child(root->agent_right,temp);
    }
    root->agent_node_height=get_agent_height(root);

    int balance=get_agent_node_balance(root);
    
	if(balance>1 && temp->agent_id<root->agent_left->agent_id)
	{
		root=rotate_agent_right(root);
	}
	else if(balance>1 && temp->agent_id>root->agent_left->agent_id)
	{
		root->agent_left=rotate_agent_left(root->agent_left);
		root=rotate_agent_right(root);
	}
	else if(balance<-1 && temp->agent_id>root->agent_right->agent_id)
	{
		root=rotate_agent_left(root);
	}
	else if(balance<-1 && temp->agent_id<root->agent_right->agent_id)
	{
		root->agent_right=rotate_agent_right(root->agent_right);
		root=rotate_agent_left(root);
	}
    return root;
}


agent_node* insert_agent_node(agent_node *root)
{
    char line[1024];
    char *sp;
    FILE *fp=fopen("agent.csv","r");
    while(fgets(line,1024,fp)!=NULL)
    {
        agent_node *temp=(agent_node*)malloc(sizeof(agent_node));      //reading data from file and storing in node
        
        sp=strtok(line,",");
        temp->agent_id=atoi(sp);

        sp=strtok(NULL,",");
        strcpy(temp->agent_name,sp);

        sp=strtok(NULL,",");
        temp->agent_phone_no=atoi(sp);

        sp=strtok(NULL,",");
        temp->commission=atoi(sp);

        sp=strtok(NULL,",");
        strcpy(temp->agent_area,sp);

        root=insert_agent_node_child(root,temp);     //inserting node in AVL tree
    }

    fclose(fp);
    return root;
}

void visit_agent(agent_node *root)
{
    if(root!=NULL)
    {
        visit_agent(root->agent_left);
        printf("agent id : %d \n",root->agent_id);
        printf("agent name : %s\n",root->agent_name);
        printf("agent area : %s\n",root->agent_area);
        printf("agent commission : %d\n",root->commission);
        printf("\n");
        visit_agent(root->agent_right);
    }
}

int get_user_height(user_node *root)
{
    int ret_val=0;
    if(root!=NULL)
    {
        ret_val=1 + maxi(get_user_height(root->user_left),get_user_height(root->user_right));
    }
    return ret_val;
}

int get_user_node_balance(user_node* root)
{
    int ret_val;
    ret_val= ( get_user_height(root->user_left) - get_user_height(root->user_right) );
    return ret_val;
}

user_node* rotate_user_left(user_node* root)
{
    user_node *x=root->user_right;
    root->user_right=x->user_left;
    x->user_left=root;
    return x;
}

user_node* rotate_user_right(user_node* root)
{
    user_node *x=root->user_left;
    root->user_left=x->user_right;
    x->user_right=root;
    return x;
}

user_node* insert_user_node_child(user_node* root,user_node* temp)
{
    
    if(root==NULL)
    {
        root=temp;
        root->user_node_height=1;
    }
    else if(temp->user_id<root->user_id)
    {
        root->user_left=insert_user_node_child(root->user_left,temp);
    }
    else if(temp->user_id>root->user_id)
    {
        root->user_right=insert_user_node_child(root->user_right,temp);
    }
    root->user_node_height=get_user_height(root);

    int balance=get_user_node_balance(root);
    
    if(balance>1 && temp->user_id<root->user_left->user_id)
    {
        root=rotate_user_right(root);
    }
    else if(balance>1 && temp->user_id>root->user_left->user_id)
    {
        root->user_left=rotate_user_left(root->user_left);
        root=rotate_user_right(root);
    }
    else if(balance<-1 && temp->user_id>root->user_right->user_id)
    {
        root=rotate_user_left(root);
    }
    else if(balance<-1 && temp->user_id<root->user_right->user_id)
    {
        root->user_right=rotate_user_right(root->user_right);
        root=rotate_user_left(root);
    }
    return root;
}

user_node* insert_user_node(user_node *root)
{
    char line[1024];
    char *sp;
    FILE *fp=fopen("user.csv","r");
    while(fgets(line,1024,fp)!=NULL)
    {
        user_node *temp=(user_node*)malloc(sizeof(user_node));    //reading data from file and storing in node
        
        sp=strtok(line,",");
        temp->user_id=atoi(sp);

        sp=strtok(NULL,",");
        strcpy(temp->user_name,sp);

        sp=strtok(NULL,",");
        strcpy(temp->user_address,sp);

        sp=strtok(NULL,",");
        temp->user_phone_no=atoi(sp);

        root=insert_user_node_child(root,temp);          //inserting node in AVL tree
    }

    fclose(fp);
    return root;
}

void visit_user(user_node *root)
{
    if(root!=NULL)
    {
        visit_user(root->user_left);
        printf("user id : %d\n",root->user_id);
        printf("user name : %s\n",root->user_name);
        printf("user address : %s\n",root->user_address);
        visit_user(root->user_right);
    }
}

int get_order_node_height(global_order_node *root)
{
    int ret_val=0;
    if(root!=NULL)
    {
        ret_val=1 + maxi(get_order_node_height(root->global_order_left),get_order_node_height(root->global_order_right));
    }
    return ret_val;
}

int get_order_node_balance(global_order_node* root)
{
    int ret_val;
    ret_val= ( get_order_node_height(root->global_order_left) - get_order_node_height(root->global_order_right) );
    return ret_val;
}

global_order_node* rotate_order_left(global_order_node* root)
{
    global_order_node *x=root->global_order_right;
    root->global_order_right=x->global_order_left;
    x->global_order_left=root;
    return x;
}

global_order_node* rotate_order_right(global_order_node* root)
{
    global_order_node *x=root->global_order_left;
    root->global_order_left=x->global_order_right;
    x->global_order_right=root;
    return x;
}

agent_node* assign_agent(agent_node* root)         //assigning agent to particular order
{
    agent_node *ret_val;
    int flag=0;
    while(root!=NULL&&flag==0)
    {
        if(root->agent_left!=NULL&&root->agent_left->agent_status==0)
        {
            root=root->agent_left;
        }
        else if(root->agent_right!=NULL&&root->agent_right->agent_status==0)
        {
            root=root->agent_right;
        }
        else{
            ret_val=root;
            root->agent_status=1;
            flag=1;
        }
    }
}

global_order_node* insert_order_node_child(global_order_node* root,global_order_node* temp,agent_node* agent_root)
{
    if(root==NULL)
    {
        root=temp;
        root->global_order_node_height=1;
        root->agents_order_ptr=assign_agent(agent_root);           //assigning agent to every order
    }
    else if(temp->customer_id<root->customer_id)
    {
        root->global_order_left=insert_order_node_child(root->global_order_left,temp,agent_root);
    }
    else if(temp->customer_id>root->customer_id)
    {
        root->global_order_right=insert_order_node_child(root->global_order_right,temp,agent_root);
    }
    root->global_order_node_height=get_order_node_height(root);

    int balance=get_order_node_balance(root);
    
    if(balance>1 && temp->customer_id<root->global_order_left->customer_id)
    {
        root=rotate_order_right(root);
    }
    else if(balance>1 && temp->customer_id>root->global_order_left->customer_id)
    {
        root->global_order_left=rotate_order_left(root->global_order_left);
        root=rotate_order_right(root);
    }
    else if(balance<-1 && temp->customer_id>root->global_order_right->customer_id)
    {
        root=rotate_order_left(root);
    }
    else if(balance<-1 && temp->customer_id<root->global_order_right->customer_id)
    {
        root->global_order_right=rotate_order_right(root->global_order_right);
        root=rotate_order_left(root);
    }
    return root;
}


global_order_node* insert_global_order_node(global_order_node *root, agent_node* agent_root)
{
    char line[1024];
    char *sp;
    FILE* fp=fopen("order.csv","r");
    while(fgets(line,1024,fp)!=NULL)
    {
        time_t start=time(NULL);
        global_order_node *temp;
        temp=(global_order_node*)malloc(sizeof(global_order_node));

        sp=strtok(line,",");
        temp->customer_id=atoi(sp);
        
        sp=strtok(NULL,",");
        strcpy(temp->customer_name,sp);

        sp=strtok(NULL,",");
        strcpy(temp->customer_address,sp);

        sp=strtok(NULL,",");
        strcpy(temp->customer_area,sp);

        sp=strtok(NULL,",");
        temp->customer_phone_no=atoi(sp);

         for(int i=0;i<max_order_menu_number;i++)
        {
            sp=strtok(NULL,",");
            strcpy(temp->order_menu[i],sp);
        }

        root=insert_order_node_child(root,temp,agent_root);
        
        time_t end=time(NULL);
        temp->time_taken=end-start;
    }
    fclose(fp);
    return root;
}

void  visit_global_order(global_order_node *root,agent_node *agent_root)
{
    if(root!=NULL&&root->agents_order_ptr!=NULL)
    {
        visit_global_order(root->global_order_left,agent_root);
        printf("order details\n");
        printf("customer id : %d\n",root->customer_id);
        printf("customer name : %s\n",root->customer_name);
        printf("customer address : %s\n",root->customer_address);
        printf("customer area : %s\n",root->customer_area);
        printf("agent details\n");
        printf("agent ID : %d\n",root->agents_order_ptr->agent_id);
        printf("agent name : %s\n",root->agents_order_ptr->agent_name);
        printf("\n");
        visit_global_order(root->global_order_right,agent_root);
    }
}

void search_function(rest_node* root, char to_search[])
{
    if(root!=NULL)
    {
        search_function(root->rest_left,to_search);
        if(strcmp(root->type_of_restarunt,to_search)==0)
        {
            printf("restaurant id : %d\n",root->restaurant_id);
            printf("address : %s\n",root->address);
            printf("area : %s\n",root->restaurant_area);
            printf("type of restaurant : %s\n",root->type_of_restarunt);
            printf("\n");
        }
        else if(strcmp(root->type_of_food,to_search)==0)
        {
            printf("restaurant id : %d\n",root->restaurant_id);
            printf("address : %s\n",root->address);
            printf("area : %s\n",root->restaurant_area);
            printf("type of food served : %s\n",root->type_of_food);
            printf("\n");
        }
        else if(strcmp(root->restaurant_area,to_search)==0)
        {
            printf("restaurant id : %d\n",root->restaurant_id);
            printf("address : %s\n",root->address);
            printf("area : %s\n",root->restaurant_area);
            printf("nearby restaurants\n");
            for(int i=0;i<nearby_loc_value;i++)
            {
                printf("%d.%s\n",i+1,root->near_by_area[i]);
            }
            printf("\n");
        }
        search_function(root->rest_right,to_search);
    }
}

global_order_node* delivery_order(global_order_node* global_root)
{
    global_order_node *ret_val=global_root;
    global_order_node* prev;
    int flag=0;
    if(global_root==NULL||global_root->agents_order_ptr==NULL)
    {
        printf("no orders are there at present\n");
        flag=1;
    }
    while(global_root!=NULL&&flag==0)
    {
        if(global_root->global_order_left!=NULL&&global_root->agents_order_ptr!=NULL)
        {
            prev=global_root;
            global_root=global_root->global_order_left;
        }
        else if(global_root->global_order_right!=NULL&&global_root->agents_order_ptr!=NULL)
        {
            prev=global_root;
            global_root=global_root->global_order_right;
        }
        else
        {
            flag=1;
            global_order_node *xptr=global_root;
            printf("delivery details\n");
            int prev_comission=global_root->agents_order_ptr->commission;
            global_root->agents_order_ptr->commission=(1.1)*prev_comission;
            printf("customer id : %d\n",global_root->customer_id);
            printf("customer name : %s\n",global_root->customer_name);
            printf("customer address : %s\n",global_root->customer_address);
            printf("order menu\n");
            for(int i=0;i<max_order_menu_number;i++)
            {
                printf("%s\n",global_root->order_menu[i]);
            }
            printf("agent id : %d\n",global_root->agents_order_ptr->agent_id);
            printf("agent name : %s\n",global_root->agents_order_ptr->agent_name);
            printf("previous comission %d\n",prev_comission);
            printf("current comission %d\n",global_root->agents_order_ptr->commission);

            xptr->agents_order_ptr->agent_status=0;                      //setting status back to zero(indicates agent is free)
            xptr->agents_order_ptr=NULL;

            if(prev->global_order_left==xptr)       //setting its previous pointers to null,so there can be no dangling pointers
            {
                prev->global_order_left=NULL;
            }
            else
            {
                prev->global_order_right=NULL;
            }

            free(xptr);
            xptr=NULL;

            printf("\n");
        }
    }
    global_root=ret_val;
    return global_root;
}

global_order_node* cancel_order(global_order_node *global_root)
{
    int flag=0;
    global_order_node* prev;
    global_order_node *ret_val=global_root;
    if(global_root==NULL||global_root->agents_order_ptr==NULL)
    {
        printf("no orders are there to cancel at present\n");
        flag=1;
    }
   while(global_root!=NULL&&flag==0)
   {
        if(global_root->global_order_left!=NULL&&global_root->agents_order_ptr!=NULL)
        {
            prev=global_root;
            global_root=global_root->global_order_left;
        }
        else if(global_root->global_order_right!=NULL&&global_root->agents_order_ptr!=NULL)
        {
            prev=global_root;
            global_root=global_root->global_order_right;
        }
        else
        {
            flag=1;
            global_order_node *xptr=global_root;
            printf("order details to be cancelled\n");
            printf("customer id : %d\n",global_root->customer_id);
            printf("customer name : %s\n",global_root->customer_name);
            printf("customer address : %s\n",global_root->customer_address);
            printf("order menu\n");
            for(int i=0;i<max_order_menu_number;i++)
            {
                printf("%s\n",global_root->order_menu[i]);
            }
            printf("agent id : %d\n",global_root->agents_order_ptr->agent_id);
            printf("agent name : %s\n",global_root->agents_order_ptr->agent_name);
            printf("comission %d\n",global_root->agents_order_ptr->commission);

            xptr->agents_order_ptr->agent_status=0;
            xptr->agents_order_ptr=NULL;

            if(prev->global_order_left==xptr)
            {
                prev->global_order_left=NULL;
            }
            else
            {
                prev->global_order_right=NULL;
            }

            free(xptr);
            xptr=NULL;
            printf("\n");
        }
   }
   global_root=ret_val;
    return global_root;
}

void find_favorite_food(global_order_node *global_root,int search_user_id)
{
    char str[menu_value][100];
    FILE* fp=fopen("menu.csv","r");
    char line[1024];
    char *sp;
    int k=0,flag=0;

    while(fgets(line,1024,fp)!=NULL)
    {
        sp=strtok(line,",");
        strcpy(str[k],sp);
        k++;
    }

    int count[menu_value];
    for(int i=0;i<menu_value;i++)
    {
        count[i]=0;
    }

    while(global_root!=NULL&&search_user_id!=global_root->customer_id&&global_root->time_taken<time_of_3_months)
    {
      if(search_user_id<global_root->customer_id)
      {
        global_root=global_root->global_order_left;
      }
      else 
      {
        global_root=global_root->global_order_right;
      }
     
    }
    
    if(global_root!=NULL)
    {
    if(search_user_id==global_root->customer_id)
    {
        flag=1;
        for(int i=0;i<max_order_menu_number;i++)
        {
            for(int j=0;j<menu_value;j++)
            {
                if(strcmp(global_root->order_menu[i],str[j])==0)
                {
                    count[j]++;                                     //increasing count of each food item
                } 
            }
        }

        for(int i=0;i<menu_value;i++)
        {
            for(int j=0;j<menu_value&&j+1<menu_value;j++)
            {
                int temp_count;
                char temp_str[100];
                if(count[j]<count[j+1])                   //sorting counts in decreasing order, so most ordered food is on the top
                {
                    temp_count=count[j];
                    count[j]=count[j+1];
                    count[j+1]=temp_count;

                    strcpy(temp_str,str[j]);
                    strcpy(str[j],str[j+1]);
                    strcpy(str[j+1],temp_str);
                }
            }
        }
    }
    }
    else
    {
        printf("no user with id %d\n",search_user_id);
    }
    
    if(flag==1)
    {
         printf("top 3 food of user id %d\n",search_user_id);
        for(int i=0;i<max_order_menu_number;i++)
        {
            printf("%s\n",str[i]);
        }
    }
   
}

void find_favorite_restaurant_traversal(global_order_node *global_root,int no_of_days,int *count, char str[no_of_restaurants][100])
{
    if(global_root!=NULL)
    {
        find_favorite_restaurant_traversal(global_root->global_order_left,no_of_days,count,str);
        if(global_root->time_taken<no_of_days*24*60*60)
        {
            for(int i=0;i<no_of_restaurants;i++)
            {
                if(strcmp(global_root->customer_area,str[i])==0)
                {
                    count[i]++;                                  //increasing count of each restaurant
                } 
            }
        }
        find_favorite_restaurant_traversal(global_root->global_order_right,no_of_days,count,str);
    }
}

void find_favorite_restaurant(global_order_node *global_root,int no_of_days)
{
    char str[no_of_restaurants][100];
    FILE* fp=fopen("restaturant_area.csv","r");
    char line[1024];
    char *sp;
    int k=0,flag=0;

    while(fgets(line,1024,fp)!=NULL)
    {
        sp=strtok(line,",");
        strcpy(str[k],sp);
        k++;
    }

    int count[no_of_restaurants];
    for(int i=0;i<no_of_restaurants;i++)
    {
        count[i]=0;
    }

    find_favorite_restaurant_traversal(global_root,no_of_days,count,str);

    for(int i=0;i<no_of_restaurants;i++)
    {
        for(int j=0;j<no_of_restaurants&&j+1<no_of_restaurants;j++)
        {
            int temp_count;
            char temp_str[100];
            if(count[j]<count[j+1])
            {
                temp_count=count[j];
                count[j]=count[j+1];
                count[j+1]=temp_count;

                strcpy(temp_str,str[j]);
                strcpy(str[j],str[j+1]);
                strcpy(str[j+1],temp_str);
            }
        }
    }

    printf("top 3 restaturants that got most orders are\n");
    for(int i=0;i<3;i++)
    {
        if(count[i]!=0)
        {
            printf("%s\n",str[i]);
            flag=1;
        }
    }
    if(flag==0)
    {
        printf("no orders in last %d days\n",no_of_days);
    }
}

void find_favorite_food_across_restaurant_traversal(global_order_node *global_root,int no_of_days,int *count, char str[no_of_restaurants][100])
{
    if(global_root!=NULL)
    {
        find_favorite_food_across_restaurant_traversal(global_root->global_order_left,no_of_days,count,str);
        if(global_root->time_taken<no_of_days*24*60*60)
        {
            for(int i=0;i<max_order_menu_number;i++)
            {
                for(int j=0;j<menu_value;j++)
                {
                    if(strcmp(global_root->order_menu[i],str[j])==0)
                    {
                        count[i]++;                      //increasing count of each food in all restaurant
                    } 
                }
                
            }
        }
        find_favorite_food_across_restaurant_traversal(global_root->global_order_right,no_of_days,count,str);
    }
}

void find_favorite_food_across_restaurant(global_order_node *global_root,int no_of_days)
{
    char str[menu_value][100];
    FILE* fp=fopen("menu.csv","r");
    char line[1024];
    char *sp;
    int k=0,flag=0;

    while(fgets(line,1024,fp)!=NULL)
    {
        sp=strtok(line,",");
        strcpy(str[k],sp);
        k++;
    }

    int count[menu_value];
    for(int i=0;i<menu_value;i++)
    {
        count[i]=0;
    }

    find_favorite_food_across_restaurant_traversal(global_root,no_of_days,count,str);

    for(int i=0;i<menu_value;i++)
    {
        for(int j=0;j<menu_value&&j+1<menu_value;j++)
        {
            int temp_count;
            char temp_str[100];
            if(count[j]<count[j+1])
            {
                temp_count=count[j];
                count[j]=count[j+1];
                count[j+1]=temp_count;

                strcpy(temp_str,str[j]);
                strcpy(str[j],str[j+1]);
                strcpy(str[j+1],temp_str);
            }
        }
    }

    printf("top 3 foods that have been most orders are\n");
    for(int i=0;i<3;i++)
    {
        if(count[i]!=0)
        {
            printf("%s\n",str[i]);
            flag=1;
        }
    }
    if(flag==0)
    {
        printf("no orders in last %d days\n",no_of_days);
    }
}

void range_search(global_order_node *root, int min, int max)
{
    if(root!=NULL)
    {
        range_search(root->global_order_left,min,max);
        if(root->customer_id<=max && root->customer_id >=min)
        {
            printf("order details\n");
            printf("customer id : %d\n",root->customer_id);
            printf("customer name : %s\n",root->customer_name);
            printf("customer address : %s\n",root->customer_address);
            printf("customer area : %s\n",root->customer_area);
            printf("agent details\n");
            printf("agent ID : %d\n",root->agents_order_ptr->agent_id);
            printf("agent name : %s\n",root->agents_order_ptr->agent_name);
            printf("\n");
        }
        range_search(root->global_order_right,min,max);
    }
}

void visit_area_wise_agent(agent_node *root)
{
    if(root!=NULL)
    {
        visit_area_wise_agent(root->agent_left);
        printf("agent name %s and area %s \n",root->agent_name,root->agent_area);
        visit_area_wise_agent(root->agent_right);
    }
}

void search_with_restaurant_details(rest_node *rest_root,char restaurant_name[],char restaurant_address[])
{
    if(rest_root!=NULL)
    {
        search_with_restaurant_details(rest_root->rest_left,restaurant_name,restaurant_address);
        if( strcmp(rest_root->restaurant_area,restaurant_name)==0 && strcmp(rest_root->address,restaurant_address)==0 )
        {
            printf("restaurant id :%d\n",rest_root->restaurant_id);
            printf("restaurant address : %s\n",rest_root->address);
            printf("restaurant area : %s\n",rest_root->restaurant_area);
            printf("restaurant type : %s\n",rest_root->type_of_restarunt);
            printf("type of food served in restaurant: %s\n",rest_root->type_of_food);
            printf("special facilities in restaurant : %s\n",rest_root->special_facilities);
            printf("number of seates in restaurant : %s\n",rest_root->no_of_seates);
        }
        search_with_restaurant_details(rest_root->rest_right,restaurant_name,restaurant_address);
    }
}

void free_rest_node(rest_node *root)
{
    if(root!=NULL)
    {
        free_rest_node(root->rest_left);
        free_rest_node(root->rest_right);
        free(root);
    }
    root=NULL;
}

void free_agent_node(agent_node *root)
{
    if(root!=NULL)
    {
        free_agent_node(root->agent_left);
        free_agent_node(root->agent_right);
        free(root);
    }
    root=NULL;
}

void free_user_node(user_node *uroot)
{
    if(uroot!=NULL)
    {
        free_user_node(uroot->user_left);
        free_user_node(uroot->user_right);
        free(uroot);
    }
    uroot=NULL;
}

void free_global_order_node(global_order_node *root)
{
    if(root!=NULL)
    {
        free_global_order_node(root->global_order_left);
        free_global_order_node(root->global_order_right);
        free(root);
    }
    root=NULL;
}

int main()
{
    rest_node *rest_root=NULL;
    rest_root=insert_rest_node(rest_root);
    // visit_restaurant(rest_root);

    agent_node *agent_root=NULL;
    agent_root=insert_agent_node(agent_root);
    //  visit_agent(agent_root);

    user_node *user_root=NULL;
    user_root=insert_user_node(user_root);
    // visit_user(user_root);

    global_order_node *global_root=NULL;
    // global_root=insert_global_order_node(global_root,agent_root);
    // visit_global_order(global_root,agent_root);

    int match=-1;
    while(match!=0)
    {
        printf("\n");
        printf("1.enter 1 to search \n");
        printf("2.enter 2 to place order\n");
        printf("3.enter 3 to delivery order\n");
        printf("4.enter 4 to cancel order\n");
        printf("5.enter 5 to Find Favorite Foods Of user-id\n");
        printf("6.enter 6 to Find Favorite Restaurants\n");
        printf("7.enter 7 to Find Favorite Foods Across Restaurants\n");
        printf("8.enter 8 to  search \n");
        printf("9.enter 9 to print agent details\n");
        printf("10.enter 10 to print live orders\n");
        printf("11.enter 11 to print area wise agents\n");
        printf("12.enter 12 to print restaurant details(given restaurant name and address)\n");
        printf("0.enter 0 to exit\n");
        scanf("%d",&match);
        printf("\n");
        if(match==1)
        {
            int sub_match;
            printf("1. enter 1 to search based on category (restaurant, cafe, pub etc) \n");
            printf("2. enter 1 to search based on category of food(northIndian, southIndian, continental)\n");
            printf("3. enter 1 to search based on area\n");
            scanf("%d",&sub_match);

            char to_search[100];
            if(sub_match==1)
            {
                printf("enter the category to search\n");
                scanf("%s",to_search);
                search_function(rest_root,to_search);
            }
            else if(sub_match==2)
            {
                printf("enter the category of food to search\n");
                scanf("%s",to_search);
                search_function(rest_root,to_search);
            }
            else if(sub_match==3)
            {
                printf("enter the area to search\n");
                scanf("%s",to_search);
                search_function(rest_root,to_search);
            }

        }
        else if(match==2)
        {
            printf("ORDER IS PLACED\n");
            global_root=insert_global_order_node(global_root,agent_root);
            // visit_global_order(global_root,agent_root);
        }
        else if(match==3)
        {
            global_root=delivery_order(global_root);
        }
        else if(match==4)
        {
            global_root=cancel_order(global_root);
        }
        else if(match==5)
        {
            int search_user_id;
            printf("enter the user id\n");
            scanf("%d",&search_user_id);
            find_favorite_food(global_root,search_user_id);
        }
        else if(match==6)
        {
            int no_of_days;
            printf("enter the number of days\n");
            scanf("%d",&no_of_days);
            find_favorite_restaurant(global_root,no_of_days);
        }
        else if(match==7)
        {
            int no_of_days;
            printf("enter the number of days\n");
            scanf("%d",&no_of_days);
            find_favorite_food_across_restaurant(global_root,no_of_days);
        }
        else if(match==8)
        {
            int min,max;
            printf("enter the minimum range\n");
            scanf("%d",&min);
            printf("enter the maximum range\n");
            scanf("%d",&max);
            range_search(global_root,min,max);
        }
        else if(match==9)
        {
            visit_agent(agent_root);
        }
        else if(match==10)
        {
            visit_global_order(global_root,agent_root);
        }
        else if(match==11)
        {
            visit_area_wise_agent(agent_root);
        }
        else if(match==12)
        {
            char restaurant_name[100],restaurant_address[100];
            printf("enter restaurant name(area) and address to search\n");
            scanf("%s",restaurant_name);
            scanf("%s",restaurant_address);
            search_with_restaurant_details(rest_root,restaurant_name,restaurant_address);
        }
    }

    if(match==0)
    {
        printf("exited successfully by freeing all nodes\n");
        free_rest_node(rest_root);
        free_agent_node(agent_root);
        free_user_node(user_root);
        free_global_order_node(global_root);
    }

    return 0;
}

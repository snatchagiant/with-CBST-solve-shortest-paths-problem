#include "Graph_Instance.h"

void graph_instance::initialize_CompleteInstance(int n)throw(Exception_about_memory)
{//initialize_GridInstance
    if(type != Complete || n <= 1){
        string str ="\n the type is not to match or, the nodes is less, so the job of initializing a complete instance is not through\n";
        Exception_about_memory e(14); e.adding_info(str); throw e;
    }
    else{
        nodes = n; Interval = nodes - 1; Arcs = nodes * Interval;
        MaketheComppleteInst();//make instance
    }
}//initialize_GridInstance

void graph_instance::initialize_GridInstance(int x, int y)throw(Exception_about_memory)
{//initialize_GridInstance
    if(type != Grid || x <= 1 || y <= 1){
        string str = "\n the type is not to match and the x,y paramters in error, so the job of initializing a complete instance is not through\n";
        Exception_about_memory e(14); e.adding_info(str); throw e;
    }
    else{
        x_dimension = x; y_dimension = y;  nodes = x * y; Interval = 4;
        if(nodes < 4){Exception_about_memory e(14); throw e;}
        Arcs = 0;

        int f = MaketheGridInst();
        if(f <= 0){
            string str = "\n failture fot initialize a grid instance.\n"; cout<<"\n the error position => "<<f<<endl;
            Exception_about_memory e(14); e.adding_info(str); throw e;
        }
    }
}//initialize_GridInstance

void graph_instance::initialize_RondomInstance(int n, int i, int *a_1, int *a_2, CBST *C)throw(Exception_about_memory)
{//initialize_RondomInstance

    if(type != Random || n <= 1 || i < 1 || i > n){//

        Exception_about_memory e(11); throw e;
    }
    else{//
        nodes = n; Interval = i; aid_array_1 = a_1; aid_array_2 = a_2; cbst = C;
        Arcs = nodes * Interval;
        try{
            MaketheRandomInst();
        }catch(Exception_about_memory &e){
            string str = "\n failure to initlialize a random instance";
            e.adding_info(str); throw e;
        }
    }//
}//initialize_RondomInstance


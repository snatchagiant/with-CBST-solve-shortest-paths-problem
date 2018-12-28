#include <stdlib.h>
#include "Graph_Instance.h"


using namespace std;
#define Max_cost 9223372036854775807
void print_time(double time_data);// timing function
void clean_memory(int **array_memory, int gauge); //release those memories

int main()
{
    Cost_Range cost; cost.Initialize(); int genus = 0;

    // A. ---------------------- + Input those paramters + ---------------------- //
    cout<<"Type in three numerics. \n The first is type of graph, there three shape: 1 means grid instance; 2 for random one;"<<
    "3 be the complete.\n\n   The two successors are used to define the domain of cost that will be assigned to each arc, the former is low "<<
    "bound, the later as up bound, they must nonegative. \n The format can be xx, xx, xx (there is space to separate each other). \n\n";

    // Confirm the type of instance, and domain of cost you want
    char *a = new char[10]; char *b = new char[10]; char *c = new char[10];
    cin>>a>>b>>c;
    genus = atoi(a); cost.Low_bound = atoi(b); cost.Up_bound = atoi(c);

    if(cost.Low_bound < 0 || cost.Up_bound < 1 || cost.Low_bound >= cost.Up_bound){
        cout<<"\n error for setting cost range!"<<endl; return - 10;
    }
    else if(genus < Grid || genus > Complete){
        cout<<"\n error for setting type of instance!"<<endl; return - 20;
    }//check three items

    // B. ---------------------- + Construct various parmeters + ---------------------- //
    // Herein those intsance we pretreated are simple one whereto for a pair of connected nodes, there are two arcs between them inversed to each other

    // grid instance: the numerics of nodes on row (x) and column (y).
    // random: the size of leaf set less than complete instance that with n-1 size of leaf set.
    // complete: the strongest connection for anyone always connects with any another in graph.

    // Of course, you can define your instance by youself but that data structure for descibe connective relationship on graph must comply our fashion:
    // root --> leaf set whereto using the ordered pair to represent an arc between two nodes.

    int x = 0; int y = 0; int Nodes = 1; int gap = -1;
    if(genus == Grid){
        cout<<"\n Type the two numbers of nodes respectively on row and column: ";
        cin>>a>>b; x = atoi(a); y = atoi(b);
        if(x < 2 || y < 2) {cout<<"\n failure to initialize a grid instance"<<endl; return - 30;}
        gap = 4; Nodes = x * y;
    }//grid
    else if(genus == Random){
        cout<<"\n Type the two numbers respectively on nodes in instance and size of leaf set: ";
        cin>>a>>b; Nodes = atoi(a); gap = atoi(b);
        if(Nodes <= 1 || gap <= 0){cout<<"\n failure to initialize a Random instance"<<endl; return - 40;}
    }//random
    else{
        cout<<"\n Tap the number of nodes: ";
        cin>>c; Nodes = atoi(c); if(Nodes <= 2) {cout<<"\n failure to initialize a Complete instance"<<endl; return - 50;}
        gap = Nodes - 1;
    }//complete
    INT64 start_time = 0, end_time = 0, diff = Max_cost, Complexity = 0, num_Deletion = 0, num_Insertion = 0, num_shortArces = 0, num_scanningArcs = 0;

    int index = 0;
    if(genus == Grid){
        index = x + y - 2; diff = diff/index;
        if(diff < (INT64)cost.Up_bound) index = 1; else index = 0;
    }
    else if(genus == Random || genus == Complete){
        index = Nodes - 1; diff = diff/index;
        if(diff < (INT64)cost.Up_bound) index = 1; else index = 0;
    }
    if(index > 0)cout<<"\n Warnning: the cost up bound maybe result to incorrect!"<<endl;
    diff = 0;

    // C. ----------------------- + Define a frame for graph instance + ----------------------- //
    // Define a frame with a 2D array for carrying a graph instance: root --> leaf set;
    // For example, 1 --> (2, 3, 4,...); 2 -->(1, 5, 6 ...)

    int **Instance = new int*[Nodes + 1]; int **Weight_list = new int*[Nodes + 1];
    for(int n = 0; n <= Nodes; n++){
        Instance[n] = new int[gap+ 1]; Weight_list[n] = new int[gap + 1];
        for(int m = 0; m <= gap; m++){Instance[n][m] = 0; Weight_list[n][m] = 0;}
    }

    // D. ----------------------- + Define a group of auxiliary arrays  + ----------------------- //
    // This memory spaces works for assisting computing.

    // Keys_list_x: record those total weight about a concrete node in graph
    // Nodes_list: where a tree reside in, those indices in it as node pointing to its predeceasor in tree.
    // Candidate: store those items we name origin.
    // Status, Region will in use of assisting graph partitions.

    const int size_ = 6; index = size_; int **Auxiliary_array = new int*[size_ + 1];
    for(int n = 0; n <= size_; n++){
        Auxiliary_array[n] = new int[Nodes + 1];
        for(int m = 0; m <= Nodes; m++) Auxiliary_array[n][m] = 0;
    }
    int *Keys_list_01; if(index >= 0) Keys_list_01 = Auxiliary_array[index]; else return (- 101); index --;
    int *Keys_list_02; if(index >= 0) Keys_list_02 = Auxiliary_array[index]; else return (- 101); index --;

    int *Nodes_list; if(index >= 0) Nodes_list = Auxiliary_array[index]; else return (- 101); index --;

    int *Status; if(index >= 0) Status = Auxiliary_array[index]; else return (- 101); index --;
    int *Region; if(index >= 0) Region = Auxiliary_array[index]; else return (- 101); index --;

    int *Candidate; if(index >= 0) Candidate = Auxiliary_array[index]; else return (- 101); index --;
    int *CandidateX; if(index >= 0) CandidateX = Auxiliary_array[index]; else return (- 101);

    // E. ----------------------- + Define a frame of CBST + ----------------------- //

    //  The CBST will as a priority heap for online operations.
    int **CBST_Tree = new int*[Nodes + 1];
    for(int n = 0; n <= Nodes; n ++) CBST_Tree[n] = new int[Auxiliary + 1];//define an instance of CBST.

    CBST cbst_inst(CBST_Tree, Keys_list_01, Nodes); // instantiate a CBST
    if(!cbst_inst.InitializeAllItems()){cout<<"\n failure to initialize a CBST instance"<<endl; return - 200;}// reset those cellulars.

    // F.----------------------- + Materialize a weighted graph + ----------------------- //

    Info_collection statistics;
    //initialize the relationship frame
    graph_instance Graph(Instance, Weight_list, cost, genus); index = Graph.get_Genus();

    if(index < 0) return - 300;

    if(index == Grid)
        try{
            Graph.initialize_GridInstance(x, y);
        }catch(Exception_about_memory &e){e.what(); return - 301;}
    else if(index == Complete)
        try{
            Graph.initialize_CompleteInstance(Nodes);
        }catch(Exception_about_memory &e){e.what(); return - 302;}
    else
        try{
            // utilize the flexion system on CBST to do the job the stochastic assignment for leaf set.
            int i = 2;
            for(int n = 1; n <= Nodes; n++){
                if(n == i){cbst_inst.set_TheKey(n, (n*(-1))); i = i + 2;}//at the event position, key will be negative
                else
                    cbst_inst.set_TheKey(n, n);
                Nodes_list[n] = n;
            }

            Nodes_list[0] = Nodes; statistics.initialize();
            index = cbst_inst.cbst_FlexionBuildingCBST(Nodes_list, &statistics);//make a CBST in flexion model
            Graph.initialize_RondomInstance(Nodes, gap, Status, Region, &cbst_inst);// materialize instance

            cbst_inst.InitializeAllItems();// reset all items in CBST.
            for(int n = 0; n <= Nodes; n++){Status[n] = 0; Region[n] = 0; Nodes_list[n] = 0; cbst_inst.set_TheKey(n, 0);}//reset arrays
        }catch(Exception_about_memory &e){e.what(); return - 303;}

    //print instance // cout<<endl; Graph.printInstance();
    cout<<endl; cout<<endl;
    cout<<"<Information of Instance>"<<endl;
    cout<<"node quantity = "<<Graph.get_Nodes()<<"; arc quantity = "<<Graph.get_Arcs()<<"; type = "<<Graph.get_Genus()<<"; ";
    cout<<" weight domain = ["<<Graph.get_LowBound()<<", "<<Graph.get_UpBound()<<"].";
    cout<<endl;

    // Complexity, in use of recording whole efforts relative to computing complexity on CBST
    // num_Deletion, the times of deletions for this computing
    // num_Insertion the number of insertion.
    // num_shortArces the number of SAs
    // num_scanningArcs the amount of scanning leaves.

    //print weight list //cout<<endl; Graph.printWeightList();

    // G. ----------------------- + Do a HDM (Hybird Dijkstra's Method) + ----------------------- //
start_time = GetTickCount();
    statistics.initialize(); index = 1; // index is the source on graph for search
    index = Graph.DoHybirdDijkstraMethodX(&cbst_inst, Nodes_list, Candidate, Region, Status, index, &statistics);
end_time = GetTickCount(); diff = end_time - start_time;

    if(index <= 0 ){
        if(!index) cout<<"\n the instance is empty!"<<endl;
        else if(index < 0) cout<<"\n there is leaf be ineligible! or error in pushing someone into Nodes_list. "<<endl;
        return - 400;
    }
    //print result of HDM //cout<<endl; Graph.printGraphPartition(Region, Status);

    cout<<endl; cout<<endl;
    cout<<"<Hybird Dijktra Method>"<<endl;
    cout<<"Number of steps is: "<<statistics.Length<<"; the number of candidates is: "<<Candidate[0]<<endl;
    cout<<"The geometircal longest path is containing "<<index<<" nodes and, HDM runtime is:";print_time(diff);
    cout<<endl;

    if(Candidate[0] <= 0){cout<<"\n Computation has been through by HDM."<<endl; return - 401;}
    for(int n = 0; n <= Nodes; n ++){
        Keys_list_02[n] = cbst_inst.get_TheKey(n); CandidateX[n] = Candidate[n];
    }//backup for CA_B

    // H. ----------------------- + Make a CBST on Candidate + ----------------------- //

start_time = GetTickCount(); // This point is that one for start CA up.
    try{
        statistics.initialize();
        index = cbst_inst.cbst_BuildingCBST(Candidate, &statistics); // building a CBST by Pyramid Method
        if(index <= 0){cout<<"\n error => "<<index<<endl; return - 500;}
    }catch(Exception_about_memory &e){e.what(); return - 501;}
    Complexity = Complexity + statistics.Length;

    //print results
/*
    cout<<endl; cout<<endl;
    cout<<"<Make a CBST for candidaites>"<<endl;
    cout<<"\n Scalar of effort = "<<Complexity<<endl;
    cbst_inst.printOverallInfo();
    cout<<endl;

    Region[0] = 0; Status[0] = 0; statistics.initialize();
    cout<<cbst_inst.printTree(Region, Status, &statistics);
*/
    // I. ----------------------- + CA, A version + ----------------------- //
    int *Backup_array = Region;  Candidate[0] = 0; int Loop = 0; int num_nodes_in = 0;
    index = Graph.get_Arcs();
    for(int N = 0; N <= index; N ++)
    {//CA loop
        int accounter = 0; Loop = N; Candidate[0] = 0; statistics.initialize();
        //I.1. Collecting all min items and bagging them with list for search
        try{
             accounter = cbst_inst.Enumeration_MinList(Candidate, &statistics); if(accounter <= 0) break;
        }catch(Exception_about_memory &e){e.what(); return - 600;}

        num_nodes_in = num_nodes_in + accounter; num_Deletion = num_Deletion + accounter;
        Complexity = Complexity + statistics.Length;

        //I.2. Optimizing those origins' leaves and make them become a new origin.
        Backup_array[0] = 0; statistics.initialize(); int t = 0;
        if(accounter == 1)
            try{
                t = Graph.OptimizingAnItem(Backup_array, Candidate[1], Nodes_list, &cbst_inst, &statistics);
            }catch(Exception_about_memory &e){e.what(); return - 601;}
        else
            try{
                t = Graph.ContestMethod(Backup_array, Candidate, Nodes_list, &cbst_inst, &statistics);
            }catch(Exception_about_memory &e){e.what(); return - 602;}
        if(t < 0) return - 603;

        num_scanningArcs = num_scanningArcs + statistics.Accounter;
        num_shortArces = num_shortArces + statistics.Quantity;
        num_Deletion = num_Deletion + statistics.other;
        Complexity = Complexity + statistics.Length;

        if(Backup_array[0] <= 0){
            if(cbst_inst.get_MinEtremum() <= 0) break; //reckening the CBST
            else
                continue;
        }

        //I.3. Pushing those new origins into CBST.
        statistics.initialize();
        try{
            if(Backup_array[0] == 1){
                if(cbst_inst.insertion(Backup_array[1], &statistics) < 0) return - 604;
                cbst_inst.set_AuxiliaryOnItem(Backup_array[1], 0); num_Insertion ++;
            }
            else{
                int t = cbst_inst.insertionAList(Backup_array, &statistics); if(t < 0) return - 605;
                num_Insertion = num_Insertion + t;
            }
            Complexity = Complexity + statistics.Length;
        }catch(Exception_about_memory &e){e.what(); return - 606;}
    }//CA

end_time = GetTickCount(); diff = end_time - start_time;

    //print result
    cout<<endl;
    cout<<"<Contest Method A>"<<endl;
    cout<<"  The number of CA big loop: "<<Loop<<"."<<endl;
    cout<<"  The amount of nodes was investigated: "<<num_nodes_in<<"."<<endl;
    cout<<"  The amount of insertion: "<<num_Insertion<<"."<<endl;
    cout<<"  The amount of deletion: "<<num_Deletion<<"."<<endl;
    cout<<"  The amount of scanning scale: "<<num_scanningArcs<<"."<<endl;
    cout<<"  The amount of SAs: "<<num_shortArces<<"."<<endl;
    cout<<"  The scalar of operation on CBST: "<<Complexity<<"."<<endl;
    cout<<"  The runtime is:"; print_time(diff);

    // test result
    index = 0;
    for(int n = 1; n <= Nodes; n++){//1
        int root = n;
        for(int m = 1; m <= Graph.get_theSizeOfLeafSet(root); m++){//2
            int leaf = Graph.get_leafFromSubgraph(root, m);
            int weight = cbst_inst.get_TheKey(root) + Graph.get_cost(root, m);
            if(weight < cbst_inst.get_TheKey(leaf)) index ++;
        }//2
    }//1
    cout<<endl;
    if(index > 0) cout<<"Failure amount = "<<index<<"."<<endl; else cout<<"Succeed!"<<endl;

    // anaylise the difference among HDM and Contest Method
    index = 0; float ratio_percent = -1.0;
    for(int n = 0; n <= Nodes; n ++){
        int increment = Keys_list_02[n] - Keys_list_01[n];
        if(increment > 0){
            index ++;  float temp = (float)increment/Keys_list_02[n];
            if(ratio_percent < temp) ratio_percent = temp;//get the max value
        }
    }
    ratio_percent = ratio_percent*100;
    cout<<"  The max improvement on total weight is: "<<ratio_percent<<"\%."<<endl;

    ratio_percent = ((float)index/Nodes)*100;
    cout<<"  There are "<<ratio_percent<<"\% nodes improved by CA."<<endl;

    // J. ----------------------- + CA, B version + ----------------------- //
    if(!cbst_inst.InitializeAllItems()) return - 700;//reset CBST
    for(int n = 0; n <= Nodes; n ++) Keys_list_01[n] = Keys_list_02[n];
    Complexity = 0; index = 0; num_Deletion = 0; num_Insertion = 0;

start_time = GetTickCount();
    // a. Re-organize a CBST with CandidateX.
    try{
        statistics.initialize();
        index = cbst_inst.cbst_BuildingCBST(CandidateX, &statistics);
        if(index <= 0){cout<<"\n error => "<<index<<endl; return - 701;}
        Complexity = Complexity + statistics.Length;
    }catch(Exception_about_memory &e){e.what(); return - 702;}

    //print tree//Region[0] = 0; Status[0] = 0; statistics.initialize();
    //cout<<cbst_inst.printTree(Region, Status, &statistics);

    // b. Optimizing those results out from HDM again.
    index = Graph.get_Arcs();
    for(int N = 0; N <= index; N ++)
    {//big loop

        // b.1. fetch the min agency with its own CL underlying the Doubly Linked List
        statistics.initialize(); int Agency = 0;
        Agency = cbst_inst.gain_theMinCL(&statistics);
        if(Agency < 0) return - 703; else if(!Agency) break;

        num_Deletion ++;
        Complexity = Complexity + statistics.Length;

        Backup_array[0] = 0; int t = 0;
        // b.2. optimizing
        try{
            statistics.initialize();
            t = Graph.ContestMethodCL(Backup_array, Agency, Nodes_list, &cbst_inst, &statistics);
            if(t < 0) return - 704;
        }catch(Exception_about_memory &e){e.what(); return - 705;}

        num_Deletion = num_Deletion + statistics.other;
        Complexity = Complexity + statistics.Length;

        if(Backup_array[0] <= 0){
            if(cbst_inst.get_MinEtremum() <= 0) break; // recken CBST
            else
                continue;
        }

        // b.3. transact those new origins whose key had been updated
        statistics.initialize();
        try{
            if(Backup_array[0] == 1){
                if(cbst_inst.insertion(Backup_array[1], &statistics) < 0) return - 706;
                cbst_inst.set_AuxiliaryOnItem(Backup_array[1], 0); num_Insertion ++;
            }
            else{
                int t = cbst_inst.insertionAList(Backup_array, &statistics); if(t < 0) return - 707;
                num_Insertion = num_Insertion + t;
            }
            Complexity = Complexity + statistics.Length;
        }catch(Exception_about_memory &e){e.what(); return - 708;}
    }//big loop

end_time = GetTickCount(); diff = end_time - start_time;

    //print result
    cout<<endl;
    cout<<"<Contest Method B>"<<endl;
    cout<<"  The amount of insertion: "<<num_Insertion<<"."<<endl;
    cout<<"  The amount of deletion: "<<num_Deletion<<"."<<endl;
    cout<<"  The scalar of operation on CBST: "<<Complexity<<"."<<endl;
    cout<<"  The runtime is:"; print_time(diff);

    // test result
    index = 0;
    for(int n = 1; n <= Nodes; n++){//1
        int root = n;
        for(int m = 1; m <= Graph.get_theSizeOfLeafSet(root); m++){//2
            int leaf = Graph.get_leafFromSubgraph(root, m); int weight = cbst_inst.get_TheKey(root) + Graph.get_cost(root, m);
            if(weight < cbst_inst.get_TheKey(leaf)) index ++;
        }//2
    }//1
    cout<<endl;
    if(index > 0) cout<<"Failure amount = "<<index<<"."<<endl; else cout<<"Succeed!"<<endl;

    // Finally reckon the average of all segments' weights on these paths.
    diff = 0;
    for(int n = 1; n <= Nodes; n++)
    {
        int son = n, parent = Nodes_list[n];
        for(int m = 1; m <= Graph.get_theSizeOfLeafSet(parent); m++)
        {
            if(Graph.get_leafFromSubgraph(parent, m) != son) continue;
            diff = diff + (INT64)Graph.get_cost(parent, m); break;
        }
    }
    diff = diff/(Nodes - 1);
    cout<<"  The average cost on each segment is: "<<diff<<"."<<endl;

    /* ----------------------- + Clean memory + ----------------------- */
    clean_memory(Instance, Nodes);
    clean_memory(Weight_list, Nodes);
    clean_memory(CBST_Tree, Nodes);
    clean_memory(Auxiliary_array, size_);
    return 0;
}

void print_time(double time_data)
{//print_time
    cout<<"  "<<time_data<<"ms; ";
    INT64 count_1 = (INT64) time_data / 1000; INT64 millisecond = (INT64)time_data % 1000;

    INT64 hour = 3600; INT64 minute = 60; cout<<"  H:M:S:ms --> ";

    if(count_1 >= hour){
        cout<<(count_1/hour)<<" : "; count_1 = count_1 % hour;
    }else cout<<"0 : ";

    if(count_1 >= minute){
        cout<<(count_1/minute)<<" : "; count_1 = count_1 % minute;
    }else cout<<"0 : ";

    if(count_1 >= 1) cout<<count_1<<" : ";
    else cout<<"0 : ";

    cout<<millisecond<<";";
}//print_time

void clean_memory(int **array_memory, int gauge)
{
    for(int n = 0; n <= gauge; n++)
    {
        delete array_memory[n]; array_memory[n] = NULL;
    }
    delete array_memory;
}


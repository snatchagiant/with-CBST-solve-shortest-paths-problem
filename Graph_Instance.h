#ifndef GRAPH_INSTANCE_H_INCLUDED
#define GRAPH_INSTANCE_H_INCLUDED

#include <time.h>
#include "CBST_Tree_Company.h"

class graph_instance
{
private:
    int **inst_pointer; // describe the relationship by sungraph structure: root -> leaf set
    int **weight_list; // the similar data structure to carry the cost on each arc, the cost: root->leaf
    Cost_Range cost_range; // the range of cost
    int type; // the type of graph: the grid, the complete, the random with stochastic numerics on number of nodes and the size of leaf set.

    int nodes; // the quantity of nodes
    int Interval; // the size of leaf set.

    int *aid_array_1, *aid_array_2; CBST *cbst; // the auxiliary arries and a CBST pointer for produce a random instance.

    int Arcs; // the amount of arcs

    int *total_weight_list; int *search_tree_list; //two lists for Hybird Dijkstra Mehtod

    int x_dimension; int y_dimension; // two parameters for a grid instance

    // ----- + essential kits + ----- //
    void updateInstanceRelation(int root, int offset, int item){inst_pointer[root][offset] = item;}
    void assign_arcWeight(int index, int offset, int value){weight_list[index][offset] = value;}
    int get_Leaf(int root, int address){return inst_pointer[root][address];}
    int get_leafset_size(int root){return inst_pointer[root][0];}
    int get_arcWeight(int root, int address){return weight_list[root][address];};

    // ----- + Graph Basic Operations + ----- //
    int push_ANodeIntoAList(int node, int *nodelist); // the scalar of nodelist is nodes
    int backupAnItem(int item, int *item_list, int limit); // the third parameter is the scalar of item_list
    int weightMaker(); // make a cost by random way
    int gain_theItemByOrdinal(CBST *cbst, int pos, Info_collection *info); // retrieval an item by ordinal that is the position in an item queue by sorting their cost.
    int get_offsetInLeafSet(int root, int leaf);// return the offset in leaf set occupied by leaf with that root and leaf

    int fillTwoPositions(int *nodes_list, int root); // in making random instance session, fill two contiguously vacant positions in a leaf set
    bool fillTwoDistinctPositions(int *nodes_list, int root_1, int root_2); // two vacant positions across two leaf sets.

    // ---- + Graph_Instance + -----//
    // make three types of instance
    void MaketheComppleteInst();
    int MaketheGridInst();
    int MaketheRandomInst()throw(Exception_about_memory);

public:
    graph_instance(int **graph, int **weights, Cost_Range cost, int t): inst_pointer(graph), weight_list(weights), cost_range(cost), type(t)
    {
        if(cost.Low_bound < 0 || cost.Up_bound <= 0 || cost.Low_bound >= cost.Up_bound){
            cout<<"\n the weight range is error"<<endl;
            type = -1; graph = NULL; weight_list = NULL; exit(-2);
        }
        else if(t < Grid || t > Complete){
            cout<<"\n the type is error"<<endl;
            type = -1; graph = NULL; weight_list = NULL; exit(-2);
        }
        Arcs = 0;
    }
    // ----- + Graph_Initialization + ----- //
    //materialize instance
    void initialize_CompleteInstance(int n)throw(Exception_about_memory);
    void initialize_GridInstance(int x, int y)throw(Exception_about_memory);
    void initialize_RondomInstance(int n, int i, int *a_1, int *a_2, CBST *C) throw(Exception_about_memory);

    // ---- + Graph_HDM_Module + ---- //
    int DoHybirdDijkstraMethodX(CBST *cbst, int *Search_tree, int *Node_list, int *region_list, int *aid_list, int source, Info_collection *info);
    int DoHybirdDijkstraMethod(int *Total_weight_set, int *Search_tree, int *Node_list, int *region_list, int *aid_list, int source, Info_collection *info);
    // Execute the Hybird Dijkstra Method and, return the number of regions

    // ---- + Graph_ContestMethod_Module + ---- //
    int OptimizingAnItem(int *Origins_list, int Item, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory);
    // The two is kernel engines for optimizing item's leaf set.
    // Wherein there are some functions including seeking SA; on that if leaf in CBST then delete it off and backup for next step.
    // What functor returns is the number of new origins, that will in list Origins_list.

    int Test_OptimizingAnItem(int *Origins_list, int Item, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory);
    // Function is as same as functor OptimizingAnItem.
    // But there is a module will be executed before deleting leaf, that is to determine if deleting leaf.
    // At the complexity theory, for this module there is not a big aid to reduce efforts than OptimizingAnItem
    // Just as a way for optimizing test.

    int ContestMethod(int *Origins_list, int *Search_list, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory);
    // Functor addresses a group of origns that in Search_list with OptimizingAnItem as kernel engine.
    // Retrun the number of new origins in Origins_list, negative returns means error
    // Atteched information:
    // states->Accounter -- the number of scanning;
    // states->Quantity -- the number of shart arcs;
    // states-> Length -- the efforts;

    int ContestMethodCL(int *Origins_list, int Agency_of_CL, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory);
    // functor's task likes ContestMethod, but the input is the head of a sequence whose structure is doubly linked list.

    // --- + general operations + ---- //
    int push_operation(int node, int *nodelist){return push_ANodeIntoAList(node, nodelist);};
    int get_Nodes(){return nodes;};
    int get_Arcs(){return Arcs;};
    int get_Genus(){return type;};
    int get_LowBound(){return cost_range.Low_bound;}; int get_UpBound(){return cost_range.Up_bound;};

    int get_theSizeOfLeafSet(int root){if(root > 0 && root <= nodes) return get_leafset_size(root); return -1;};
    int get_leafFromSubgraph(int root, int address){if(root > 0 && root <= nodes) return get_Leaf(root, address); return -1;};
    int get_cost(int root, int address){if(root > 0 && root <= nodes) return get_arcWeight(root, address); return -1;};

    // -------------------------+ Graph_Printing_Module +------------------------------- //
    void printGraphPartition(int *region_set, int *status_set);
    void printInstance();
    void printWeightList();
};

#endif // GRAPH_INSTANCE_H_INCLUDED

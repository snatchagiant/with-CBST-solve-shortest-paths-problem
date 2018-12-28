#include "Graph_Instance.h"

int graph_instance::OptimizingAnItem(int *Origins_list, int Item, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory)
{//OptimizingAnItem
    int root = Item; int counter = 0;
    int offset = get_theSizeOfLeafSet(root); states->Accounter = states->Accounter + offset;
    for(int m = 1; m <= offset; m++){//scan leaf set
        int leaf = get_leafFromSubgraph(root, m);
        if(!cbst->checkexitence(leaf)){
            states -> Kind = -1; cout<<"\n leaf error in function OptimizingAnItem"; return -1;
        }
        int Path_cost = cbst->get_TheKey(root) + get_cost(root, m);
        if(cbst->get_TheKey(leaf) <= Path_cost) continue;

        //2.1 if sa, delete leaf off CBST if resides in CBST
        Info_collection info;
        try{
            info.initialize(); int t = cbst->deletion(leaf, &info);
            if(t < 0){
                string str = "\n deletion, function ContestMethod";
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }
            else if(type > 0) states->other ++;

            states->Length = states->Length + info.Length;
        }catch(Exception_about_memory &e){e.what(); throw e;}

        // 2.2 update the total weight and parent-child relation
        cbst->set_TheKey(leaf, Path_cost); Search_tree[leaf] = root; states->Quantity ++;

        // 2.3 backup the new origins
        if(cbst->get_info(leaf, Auxiliary) < 1){//backup, if leaf being clean
            if(push_ANodeIntoAList(leaf, Origins_list) <= 0 || !cbst->set_AuxiliaryOnItem(leaf, 1)){
                string str = "\n push_ANodeIntoAList or set_AuxiliaryOnItem, function ContestMethod";
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }
            counter ++;
        }//bcakup
    }//leaf
    // states->Accounter: the quantity of scanning leaves
    // states->Quantity: the tally of finding out SAs
    // states->Length: the efforts involving to compleixty reckening
    // states->other: the times of deletions.
    return counter;
}//OptimizingAnItem

int graph_instance::ContestMethod(int *Origins_list, int *Search_list, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory)
{//ContestMethod
    if(Search_list[0] <= 0) return -1;

    Origins_list[0] = 0; int index = 0;
    while(Search_list[0] > 0)
    {//loop
        //1. enumerate each origin
        int root = cbst->popItem(Search_list, cbst->get_MemoryScale());
        if(root < 0){
            string str = "\n there is ineligible item in Search_list, function ContestMethod";
            Exception_about_memory e(1); e.adding_info(str); throw e;
        }
        else if(!root) break;

        //the kernal engine OptimizingAnItem for optimizing
        try{
            index = OptimizingAnItem(Origins_list, root, Search_tree, cbst, states);
            if(index < 0){cout<<"\n leaf in error, function ContestMethod"; return index;}
        }catch(Exception_about_memory &e){
            string str = "\n OptimizingAnItem, function ContestMethod";
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }
    }//loop

    return index; // the new origins's number
}//ContestMethod

int graph_instance::ContestMethodCL(int *Origins_list, int Agency_of_CL, int *Search_tree, CBST *cbst, Info_collection *states)throw(Exception_about_memory)
{//ContestMethodCL
    if(Agency_of_CL <= 0) return 0;

    int root = Agency_of_CL; int num_cousin = 0;
    while(root){//search loop

        //1. enumerate item
        if(!cbst->checkexitence(root)){
            string str ="\n root, function ContestMethodCL";
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }
        //test

        //2. search
        try{
            int num_origins = OptimizingAnItem(Origins_list, root, Search_tree, cbst, states);
            if(num_origins < 0){
                cout<<"\n leaf in error, function ContestMethodCL"; return num_origins;
            }
        }catch(Exception_about_memory &e){
            string str = "\n OptimizingAnItem error, function ContestMethodCL";
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }

        root = cbst->travelCL(root);
        if(root <= 0) break; else{ num_cousin ++; cbst->itemdecrement();}
    }//loop
    return num_cousin;
}//ContestMethodCL


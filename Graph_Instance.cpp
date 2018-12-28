#include "Graph_Instance.h"

using namespace std;

void graph_instance::MaketheComppleteInst()
{//MaketheComppleteInst
    srand((unsigned int)(time(NULL)));
    for(int n = 1; n <= nodes; n++){//for root
        int root = n; updateInstanceRelation(root, 0, (nodes - 1));
        int index = 0;
        for(int m = 1; m <= nodes; m++){//for leaf
            if(m != root){
                index ++; updateInstanceRelation(root, index, m);
                int cost = weightMaker(); if(cost <= 0) cost = 1;
                assign_arcWeight(root, index, cost);
            }
        }//leaf
    }//root
}//MaketheComppleteInst

int graph_instance::MaketheGridInst()
{//MaketheGridInst

    if(x_dimension < 0 || x_dimension > nodes || y_dimension < 0 || y_dimension > nodes) return -1; //check x,y for shape of grid

    if(Interval != 4) return -2;//check gap for leaf set

    srand((unsigned int)(time(NULL)));//deposit a seed for random assignment

    int cursor = 1; int LeftMost_Post = 1; int RightMost_Post = x_dimension;
    int Counter = 0;

    for(int n = 1; n <= y_dimension; n++)
    {//on row
        while(cursor <= RightMost_Post)
        {// on column

            //1. make the left-right relationship
            int left_node = cursor - 1; int right_node = cursor + 1;
            int offset = 0; int cost = 0;

            if(left_node >= LeftMost_Post){//stretch to left
                offset ++; updateInstanceRelation(cursor, offset, left_node);
                cost = weightMaker();
                if(cost)
                    assign_arcWeight(cursor, offset, cost);
                else
                    assign_arcWeight(cursor, offset, 1);
            }
            if(right_node <= RightMost_Post){
                offset ++; updateInstanceRelation(cursor, offset, right_node);
                cost = weightMaker();
                if(cost)
                    assign_arcWeight(cursor, offset, cost);
                else
                    assign_arcWeight(cursor, offset, 1);
            }

            //2. make the up-down relationship
            int upper_node = cursor - x_dimension; int lower_node = cursor + x_dimension;

            if(upper_node > 0){
                offset ++; updateInstanceRelation(cursor, offset, upper_node);
                cost = weightMaker();
                if(cost)
                    assign_arcWeight(cursor, offset, cost);
                else
                    assign_arcWeight(cursor, offset, 1);
            }

            if(lower_node <= nodes){
                offset ++; updateInstanceRelation(cursor, offset, lower_node);
                cost = weightMaker();
                if(cost)
                    assign_arcWeight(cursor, offset, cost);
                else
                    assign_arcWeight(cursor, offset, 1);
            }
            Arcs = Arcs + offset;
            updateInstanceRelation(cursor, Counter, offset);// record the numeric of quantity of leaves

            //3. set the rest to zero
            while(offset < Interval){
                offset++;
                updateInstanceRelation(cursor, offset, 0);
                assign_arcWeight(cursor, offset, 0);
            }
            cursor ++;
        }// on column
        RightMost_Post = RightMost_Post + x_dimension; LeftMost_Post = LeftMost_Post + x_dimension;
    }//on row

    return 1;
}//MaketheGridInst

int graph_instance::MaketheRandomInst()throw(Exception_about_memory)
{//MaketheRandomInst
    if(cbst->get_RootInCBST() <= 0) return 0;

    //make a weight list
    for(int n = 1; n <= nodes; n++){
        for(int m = 1; m <= Interval; m++){
            int cost = weightMaker(); if(cost <= 0) cost = 1;
            assign_arcWeight(n, m, cost);
        }
    }

    //make relationships of connection among those nodes
    int *partial_list = new int[Interval + 1]; partial_list[0] = 0;
    int *aid_list = new int[Interval + 1]; aid_list[0] = 0; //for auxiliary

    Info_collection info; info.initialize(); srand((unsigned int)(time(NULL))); //make a random seed

    int counter = 0;
    // method introduction
    // A. randomly get a root from CBST and delete it off
    // B. delete those leaves in leaf set off from CBST, the set belongs to root.
    // C. elect the new leaf out from CBST, meanwhile wad the rest vacant position in root's leaf set. Meanwhile to record them.
    // D. re-insert those new leaves into CBST if its own number of insertion into leaf sets does not up to Interval.
    // E. adjust the a few of vacant position in those leaf sets.
    // -------------------+-------------------------+------------------- //

    for(int n = 1; n <= nodes; n++){//loop nodes
        // A step: select the root and delete root off
        int root = n; int leaf = 0;
        try{
            cbst->Flexion_deletion(root, &info);
        }catch(Exception_about_memory &e){
            string str = "root Flexion_deletion in MaketheRandomInst"; e.adding_info(str); throw e;
        }//delete root off

        // B. delete those leaves whom had been resided in leaf set and that say those no longer to present in election
        int leaf_set_size = get_leafset_size(root);
        if(leaf_set_size > 0){//if leaf set nonempty
            for(int n = 1; n <= leaf_set_size; n++){//delete leaves which have been in leaf set
                leaf = get_Leaf(root, n);
                try{
                    leaf = cbst->Flexion_deletion(leaf, &info);
                }catch(Exception_about_memory &e){
                    string str = "leaf 1 Flexion_deletion in MaketheRandomInst"; e.adding_info(str); throw e;
                }//delete leaf off
                if(leaf < 0){
                    cout <<"\n leaf = "<<leaf<<endl;
                    Exception_about_memory e(0);
                    string str = "leaf 2 Flexion_deletion in MaketheRandomInst"; e.adding_info(str); throw e;
                }
            }//deletion
        }//leaf set

        //C. election the wads
        leaf_set_size ++; if(leaf_set_size > Interval) continue;
        counter = 0;
        for(int m = leaf_set_size; m <= Interval; m ++)
        {//ele
            int pos = cbst->get_NumberOfAgencies(); if(pos <= 0) break;
            pos ++;  pos = rand()%pos;

            int item = gain_theItemByOrdinal(cbst, pos, &info);
            if(item <= 0){
                string str ="\n item Sql_inquiryWithOrdinal error in MaketheRandomInst";
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }
            else
                try{
                    cbst->Flexion_deletion(item, &info);
                }catch(Exception_about_memory &e){
                    string str = "item Flexion_deletion in MaketheRandomInst"; e.adding_info(str); throw e;
                }
            updateInstanceRelation(root, m, item); updateInstanceRelation(root, 0, m);
            aid_array_1[item] ++; counter ++;

            pos = get_leafset_size(item); pos ++; updateInstanceRelation(item, 0, pos);
            updateInstanceRelation(item, pos, root); counter ++;

            if(aid_array_1[item] < Interval){
                if(backupAnItem(item, aid_list, Interval) <= 0){
                    string str = "\n backupAnItem in MaketheRandomInst";
                    Exception_about_memory e(0); e.adding_info(str); throw e;
                }
            }
            counter = m;
        }//ele
        if(counter < Interval){
            if(backupAnItem(root, partial_list, Interval) <= 0){
                string str = "\n partial_list in MaketheRandomInst";
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }// record those roots that with a partial leaf set
        }

        // D. re-insertion
        if(aid_list[0] > 0){
            for(int m = 1; m <= aid_list[0]; m++){
                if(!cbst->Flexion_insertion(aid_list[m], &info)){//2
                    string str = "aid_list[m] Flexion_insertion in MaketheRandomInst";
                    Exception_about_memory e(0); e.adding_info(str); throw e;
                }//inserting
            }
        }
        aid_list[0] = 0; if(cbst->get_NumberOfAgencies() <= 0) break;
    }//loop
    // E. adjustment to fill some vacant position in leaf set

    cout<<endl;

    if(partial_list[0] > 0){//E
        for(int n = 1; n <= partial_list[0]; n++){//loop
            int node = partial_list[n]; int num_vacant = Interval - get_leafset_size(node);
            if(num_vacant >= 2){// > 2
                int limit = num_vacant;
                for(int m = 1; m <= limit; m ++){//
                    int t = fillTwoPositions(aid_array_2, node);
                    if(t < 0){
                        string str ="\n fillTwoPositions error in MaketheRandomInst";
                        Exception_about_memory e(0); e.adding_info(str); throw e;
                    }
                    else
                        num_vacant = Interval - get_leafset_size(node);
                    if(num_vacant <= 1) break;
                }//
            }//2
            int next = 0;
            if(n < partial_list[0]) next = partial_list[n + 1];
            if(num_vacant == 1){// == 1
                if(next > 0){
                    if(!fillTwoDistinctPositions(aid_array_2, node, next)){
                        string str ="\n fillTwoDistinctPositions error in MaketheRandomInst";
                        Exception_about_memory e(0); e.adding_info(str); throw e;
                    }
                }
                else{
                    string str ="\n next error in MaketheRandomInst";
                    Exception_about_memory e(0); e.adding_info(str); throw e;
                }
            }//1
        }//loop
    }//E

    return 1;
}//MaketheRandomInst




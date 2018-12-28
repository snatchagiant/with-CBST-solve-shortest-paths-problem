#include "Graph_Instance.h"

int graph_instance::push_ANodeIntoAList(int node, int *nodelist)
{//push_ANodeIntoAList
    int address = nodelist[0]; if(address < 0) address = 0;
    address ++; if(address > nodes) return 0;
    nodelist[0] = address; nodelist[address] = node;
    return address;
}//push_ANodeIntoAList

int graph_instance::backupAnItem(int item, int *item_list, int limit)
{//backupAnItem
    int address = item_list[0]; if(address < 0) address = 0;
    address ++; if(address > limit) return 0;

    item_list[0] = address; item_list[address] = item;

    return address;
}//backupAnItem

int graph_instance::weightMaker()
{//weightMaker
    int t = cost_range.Up_bound - cost_range.Low_bound; t ++; t = rand()% t;
    return t + cost_range.Low_bound;
}//weightMaker

int graph_instance::gain_theItemByOrdinal(CBST *cbst, int pos, Info_collection *info)
{//gain_theItemByOrdinal
    if(cbst->get_NumberOfAgencies() <= 0) return 0;

    int node = 0;
    if(pos <= 1 || pos > cbst->get_NumberOfAgencies()) node = cbst->get_MinEtremum();
    else
        node = cbst->Sql_inquiryWithOrdinal(pos, info);

    return node;
}//gain_theItemByOrdinal

int graph_instance::get_offsetInLeafSet(int root, int leaf)
{//get_offsetInLeafSet
    int limit = get_leafset_size(root); int pos = -1;
    for(int n = 1; n <= limit; n++){
        if(inst_pointer[root][n] != leaf) continue;
        pos = n; break;
    }
    return pos;
}//get_offsetInLeafSet

int graph_instance::fillTwoPositions(int *nodes_list, int root)
{//fillTwoPositions
    int pos_1 = 0; int pos_2 = 0; int leaf = 0; nodes_list[root] = root;
    for(int t = 1; t <= Interval; t ++){//read through leaf set
        leaf = get_Leaf(root, t);
        if(leaf > 0) nodes_list[leaf] = root;
        else if(!pos_1) pos_1 = t;
        else if(!pos_2){pos_2 = t; break;}
    }

    for(int n = 1; n <= nodes; n++){//loop
        int node = n;
        if(nodes_list[node] == root) continue; // those in root's leaf set too.

        bool flag = false;
        for(int m = 1; m <= get_leafset_size(node); m ++){
            leaf = get_Leaf(node, m);
            if(nodes_list[leaf] == root) continue;

            //find out an apt couple of root and leaf
            updateInstanceRelation(root, pos_1, node); updateInstanceRelation(node, m, root);
            updateInstanceRelation(root, pos_2, leaf);

            int i = get_leafset_size(root); i = i + 2; updateInstanceRelation(root, 0, i);

            i = get_offsetInLeafSet(leaf, node);
            if(i < 0) return i; else updateInstanceRelation(leaf, i, root);
            flag = true;
            break;
        }// they a perfect couple for root two vacant positions
        if(flag) break;
    }//loop

    return pos_2;
}//fillTwoPositions

bool graph_instance::fillTwoDistinctPositions(int *nodes_list, int root_1, int root_2)
{//fillTwoDistinctPositions

    nodes_list[root_1] = root_1; nodes_list[root_2] = root_2;
    int pos_1 = 0; int pos_2 = 0; int leaf = 0;
    for(int m = 1; m <= Interval; m ++){//read through leaf set
        leaf = get_Leaf(root_1, m);
        if(leaf > 0) nodes_list[leaf] = root_2; else if(!pos_1) pos_1 = m;

        leaf = get_Leaf(root_2, m);
        if(leaf > 0) nodes_list[leaf] = root_2; else if(!pos_2) pos_2 = m;

        if(pos_1 > 0 && pos_2 > 0) break;
    }//read through the leaf set of root_1: set the lable with root_2, find out the vacant position

    bool flag = false;
    for(int n = 1; n <= nodes; n ++){//loop
        int root = n;
        if(nodes_list[root] == root_2) continue;

        for(int m = 1; m <= get_leafset_size(root); m++){//look up approiate couple

            leaf = get_Leaf(root, m); if(nodes_list[leaf] == root_2) continue;

            updateInstanceRelation(root_1, pos_1, root); updateInstanceRelation(root, m, root_1);
            int index = get_leafset_size(root_1); index ++; updateInstanceRelation(root_1, 0, index);

            updateInstanceRelation(root_2, pos_2, leaf);
            index = get_offsetInLeafSet(leaf, root);
            if(index < 0) return false;
            else {
                updateInstanceRelation(leaf, index, root_2);
                index = get_leafset_size(root_2); index ++; updateInstanceRelation(root_2, 0, index);
                flag = true;
                break;
            }
        }//kooking up

        if(flag) return flag;
    }//loop

    return flag;
}//fillTwoDistinctPositions

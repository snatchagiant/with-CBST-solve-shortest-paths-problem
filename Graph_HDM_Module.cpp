#include "Graph_Instance.h"


int graph_instance::DoHybirdDijkstraMethod(int *Total_weight_set, int *Search_tree, int *Node_list, int *region_list, int *aid_list, int source, Info_collection *info)
{//DoHybirdDijkstraMethod

    if(type < Grid || type > Complete || source <= 0 || source > nodes) return 0;

    /* Method instroduction

        1. Total_weight_set: node -> relative total weight on node.

        2. Search_tree: the parent pointed array: index of array is node; member is the parent of node whom be pointed to, the preceding one.

        3. Node_list is an empty array when as input, its task is to carry those origins.

        4. region_list is an empty array to carry those regions.

        5. aid_list will record the region id for node, the form: node -> id.

    ********* ********* ********* ********* ********* ********* ********* ********* */
    int index = 1; region_list[1] = source; region_list[0] = 1; int startup = 1;
    aid_list[source] = 1;

    for(int n = 0; n <= nodes; n++){//loop: scan the all arcs Nodes

        int amount = 0; int offset = region_list[0]; int address = 0;
        for(int m = startup; m <= offset; m++){//scan region
            int root = region_list[m];//get the node in subregion
            bool signal = true; int gap = get_leafset_size(root);
            for(int k = 1; k <= gap; k++){//scan leaf set
                int leaf = get_Leaf(root, k); info -> Length ++;
                if(leaf <= 0 || leaf > nodes) return -1;

                int total_weight = Total_weight_set[root] + get_arcWeight(root, k);
                bool flag = false;
                if(aid_list[leaf] <= 0){//has not been partitioned
                    aid_list[leaf] = index + 1;
                    amount ++; address = offset + amount; region_list[address] = leaf; //record the node in region.

                    flag = true;
                }//partitioned
                else if(aid_list[leaf] > aid_list[root] && total_weight < Total_weight_set[leaf]) flag = true;//Distance function
                else if(total_weight < Total_weight_set[leaf] && signal){
                    if(push_ANodeIntoAList(root, Node_list) <= 0) return -2;//record the origin
                    else
                        signal = false;
                }
                if(flag){Search_tree[leaf] = root; Total_weight_set[leaf] = total_weight;}
            }//leaf set
        }//region

        if(amount <= 0)break;
        else{
            startup = offset + 1; region_list[0] = address; index ++;
        }
    }//loop

    info ->Accounter = index;
    return index;
}//DoHybirdDijkstraMethod next

int graph_instance::DoHybirdDijkstraMethodX(CBST *cbst, int *Search_tree, int *Node_list, int *region_list, int *aid_list, int source, Info_collection *info)
{//DoHybirdDijkstraMethodX
    if(type < Grid || type > Complete || source <= 0 || source > nodes) return 0;

    int index = 1; region_list[1] = source; region_list[0] = 1; int startup = 1;
    aid_list[source] = 1;

    for(int n = 0; n <= nodes; n++){//loop: scan the all arcs Nodes

        int amount = 0; int offset = region_list[0]; int address = 0;
        for(int m = startup; m <= offset; m++){//scan region
            int root = region_list[m];//get the node in subregion

            bool signal = true; int gap = get_leafset_size(root);
            for(int k = 1; k <= gap; k++){//scan leaf set
                int leaf = get_Leaf(root, k); info -> Length ++;
                if(leaf <= 0 || leaf > nodes) return -1;

                int total_weight = cbst->get_TheKey(root) + get_arcWeight(root, k);
                bool flag = false;
                if(aid_list[leaf] <= 0){//has not been partitioned
                    aid_list[leaf] = index + 1;
                    amount ++; address = offset + amount; region_list[address] = leaf; //record the node in region.

                    flag = true;
                }//partitioned
                else if(aid_list[leaf] > aid_list[root] && total_weight < cbst->get_TheKey(leaf)) flag = true;//Distance function
                else if(total_weight < cbst->get_TheKey(leaf) && signal){
                    if(push_ANodeIntoAList(root, Node_list) <= 0) return -2;//record the origin
                    else
                        signal = false;
                }
                if(flag){Search_tree[leaf] = root; cbst->set_TheKey(leaf, total_weight);}
            }//leaf set
        }//region

        if(amount <= 0)break;
        else{
            startup = offset + 1; region_list[0] = address; index ++;
        }
    }//loop

    info ->Accounter = index;
    return index;
}//DoHybirdDijkstraMethodX

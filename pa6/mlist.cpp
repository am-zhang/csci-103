#include <iostream>
#include "mlist.h"
using namespace std;

//Constructor, construct an empty doubly linked list.
MList::MList(){
    ntop = NULL;
    nbottom = NULL;
    _size = 0;
}

//Destructor, properly clears and deallocates everything of current list.
//simply call the clear function if you have already implemented that.
MList::~MList(){
    clear();
}

//this function properly clears and deallocates everything of current list.
void MList::clear(){
    if(isEmpty())
    {
        return;
    }
    else
    {
        pop_bottom();
        clear();
    }
}

//returns a boolean true if the list is empty, false if otherwise.
bool MList::isEmpty(){
    return (_size == 0);
}

/*
	Add or insert a new node with d_item as its data at the top of the list.
*/
void MList::push_top(Data* d_item){
    Node* temp = new Node;
    temp->next = NULL;
    temp->prev = NULL;
    temp->nodeData = d_item;
    if (isEmpty())
    {
        ntop = temp;
        nbottom = temp;
    }
    else
    {
        temp->next = ntop;
        ntop->prev = temp;
        ntop = temp;
    }
    _size++;
}

/*
	Add or insert a new node with d_item as its data at the bottom of the list.
*/
void MList::push_bottom(Data* d_item){
    Node* temp = new Node;
    temp->next = NULL;
    temp->prev = NULL;
    temp->nodeData = d_item;
    if (isEmpty())
    {
        ntop = temp;
        nbottom = temp;
    }
    else
    {
        temp->prev = nbottom;
        nbottom->next = temp;
        nbottom = temp;
    }
    _size++;
}

/*
	Delete or remove the top node of the list.
*/
void MList::pop_top(){
    if(!isEmpty())
    {
        Node* temp = ntop;
        ntop = ntop->next;
        
        if(_size > 1)
        {
            ntop->prev = NULL;
        }
        else // if one item left
        {
            ntop = NULL;
            nbottom = NULL;
        }
        
        _size--;
        delete temp;
    } 
}

/*
	Delete or remove the bottom node of the list.
*/
void MList::pop_bottom(){
    if(!isEmpty())
    {
        Node* temp = nbottom;
        nbottom = nbottom->prev;
        
        if(_size > 1)
        {
            nbottom->next = NULL;
        }
        else // if one item left
        {
            nbottom = NULL;
            ntop = NULL;
        }
        
        _size--;
        delete temp;
    } 
}

/*
	Search a node in the list that has data with the same name as the parameter.
	If one is found, return the pointer to the node.
	If one is not found, return NULL.
*/
Node* MList::search(Node* start, string name){
    if (start == NULL)
    {
        return NULL;
    }
    else if(name == (start->nodeData)->name)
    {
        return start;
    }
    else
    {
        return search(start->next, name);
    }
       
}


//Swapping node a with node b.
void MList::swapNode(Node* a, Node*b){

    if (a == b) return;
    if ((b == NULL) && (_size > 0)) // pushes a to front
    {
        insertAfter(a,b);
    }
    else if (a->next == b) // if b is right after a
    {
        removeNode(a);
        insertAfter(a, b);
    }
    else if (b->next == a) // if a is right after b
    {
        removeNode(b);
        insertAfter(b, a);
    }
    else if(a == ntop) // if a is first
    {
        removeNode(a);
        insertAfter(a, b);
        removeNode(b);
        insertAfter(b, NULL);
    }
    else
    {
        Node* a_prev = a->prev;
        removeNode(a);
        insertAfter(a, b);
        removeNode(b);
        insertAfter(b, a_prev);
    }
}

/*
	This function PERMANENTLY deletes a node from the list given a pointer to that node.
*/
void MList::deleteNode(Node* a){ // remove + delete
    if ((a->next == NULL) && (a->prev == NULL) && (_size == 1)) // if a is only node
    {
        ntop = NULL;
        nbottom = NULL;
    }
    else if (a->next == NULL) // if a is the last in the list
    {
        nbottom = a->prev;
        (a->prev)->next = NULL;
    }
    else if(a->prev == NULL) // if a is the first in the list
    {
        ntop = a->next;
        (a->next)->prev = NULL;
    }
    else
    {
        (a->prev)->next = a->next;
        (a->next)->prev = a->prev;
    }
    
    a->prev = NULL;
    a->next = NULL;
    _size--;
    delete a;
}

/*
	Unlike the delete function above, this function does NOT permanently delete the node.
	Note: this function will be useful
        when we want to insert the given node later at another position after you remove it from the list.
*/
void MList::removeNode(Node* a){
    if ((a->next == NULL) && (a->prev == NULL)) // if a is only node
    {
        ntop = NULL;
        nbottom = NULL;
    }
    else if (a->next == NULL) // if a is the last in the list
    {
        nbottom = a->prev;
        (a->prev)->next = NULL;
    }
    else if(a->prev == NULL) // if a is the first in the list
    {
        ntop = a->next;
        (a->next)->prev = NULL;
    }
    else
    {
        (a->prev)->next = a->next;
        (a->next)->prev = a->prev;
    }
    
    a->prev = NULL;
    a->next = NULL;
    _size--;
}

/*
	Insert node a after node b.
*/
void MList::insertAfter(Node *a, Node* b){
    if((b == NULL) && (_size == 0)) // a is only item in list
    {
        ntop = a;
        nbottom = a;
        a->next = NULL;
        a->prev = NULL;
    }
    else if ((b == NULL) && (_size >= 0)) // push a to front
    {
        a->next = ntop;
        ntop->prev = a;
        ntop = a;
        a->prev = NULL;
    }
    else if(b->next == NULL) // if b is end of list
    {
        nbottom = a;
        a->prev = b;
        a->next = NULL;
        b->next = a;
    }
    else
    {
        a->prev = b;
        a->next = b->next;
        (b->next)->prev = a;
        b->next = a;
    }
    _size++;
}


/*
	Implement a SELECTION SORT algorithm using recursion.
	The function takes in a start node.
*/
void MList::sortSelection(Node* start, bool mode){
    if (isEmpty())
    {
        return;
    }
    
    if (mode) // alphabetical order
    {
        if(start == nbottom)
        {
            return;
        }
        else
        {
            Node* min = start;
            Node* search_min = start;
            while(search_min != NULL) 
            {
                if (search_min->nodeData->name < min->nodeData->name)
                {
                    min = search_min; // finds min from rest of list
                }
                search_min = search_min->next;
            }

            swapNode(start,min);
            
            sortSelection(min->next, mode); 
        }
    }
    else // size ascending
    {
        if(start == nbottom || start->next == NULL)
        {
            return;
        }
        else
        {
            Node* search_min = start;
            Node* min = start;
            while(search_min != NULL)
            {
                if (search_min->nodeData->size < min->nodeData->size)
                {
                    min = search_min; // finds min from rest of list
                }
                search_min = search_min->next;
            }
           
            swapNode(min, start); // swaps with start 
            
            sortSelection(min->next, mode);
        }
    }
}


/*
	Implement an INSERTION SORT algorithm using recursion.
	The function takes in a start node.
*/
void MList::sortInsertion(Node* start){
    if (isEmpty())
    {
        return;
    }
    
    if(start == NULL)
    {
        return;
    }
    else
    {
        Node* following = start;
        Node* start_next = start->next;
        
        while(following != NULL)
        {
            if(following->nodeData->name < start->nodeData->name) // searches through sorted for location
            {
                break;
            }
            following = following->prev; // moves backwards
        }      
        
        removeNode(start);
        insertAfter(start, following);
        
        sortInsertion(start_next);
    }
}


/*
	Traverse and print our list from node n to the top.
	we will seperate each node printed by the passed-in delimiter.
    This function is private
*/
void MList::traverseToTop(Node* n, string delim){
    if (n == NULL)
    {
        return;
    }
    else
    {
        if((n->nodeData)->isFolder) // if folder type
        {
            cout << (n->nodeData)->name << delim;
        }
        else // if file type
        {
            cout << (n->nodeData)->name << "(" << (n->nodeData)->size << ")" << delim;
        }
        traverseToTop(n->prev, delim);
    }
}

/*
	Traverse and print our list from node n to the bottom.
	we will separate each node printed by the passed-in delimiter.
    This function is private
*/
void MList::traverseToBottom(Node* n,string delim){
    if (n == NULL)
    {
        return;
    }
    else
    {
        if((n->nodeData)->isFolder) // if folder type
        {
            cout << (n->nodeData)->name << delim;
        }
        else // if file type
        {
            cout << (n->nodeData)->name << "(" << (n->nodeData)->size << ")" << delim;
        }
        traverseToBottom(n->next, delim);
    }
}

//getting the pointer to the top node.
Node* MList::top(){
	return ntop;
}

//getting the pointer to the bottom node.
Node* MList::bottom(){
	return nbottom;
}

//call traverseToBottom to print all item in the list from bottom node to top node
//the list of items is separated by a given delimiter
void MList::printBtT(string delim){
	//create a temp pointer to hold bottom
	Node* tmp = nbottom;
	traverseToTop(tmp,delim);
}

//call traverseToBottom to print all item in the list from top node to bottom node
//the list of items is separated by a given delimiter
void MList::printTtB(string delim){
	Node* tmp = ntop;
	traverseToBottom(tmp,delim);
}

//call sortSelection function, mode = true = sort by name
//public member
void MList::sortByNameSelection(){
    bool mode = true;
    Node *tmp = ntop;
    sortSelection(tmp,mode);
}

//call sortSelection function, mode = false = sort by size
//public member
void MList::sortBySizeSelection(){
    bool mode = false;
    Node *tmp = ntop;
    sortSelection(tmp,mode);
}

//call sortInsertion function
//public member
void MList::sortByNameInsertion(){
    Node *tmp = ntop;
    sortInsertion(tmp);
}
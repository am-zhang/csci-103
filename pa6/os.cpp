#include <iostream>
#include "os.h"
using namespace std;

/*
Constructor
	Dynamically allocate root data.
	set isFolder = true.
	dynamically allocate Mlist for our root data as well (since it's a directory type).
    push our newly created data object pointer to wd and dataStack from the top.
*/
OS::OS(){
    root_data = new Data;
    root_data->name = "root";
    root_data->isFolder = true;
    root_data->childList = new MList;
    dataStack.push_top(root_data);
    wd.push_top(root_data);
}

/*
Destructor to clean-up memory.
*/
OS::~OS(){
    Node* temp = dataStack.top();
    while(temp != NULL) // deletes everything in the datastack
    {
        if(temp->nodeData->isFolder == true)
        {
            delete temp->nodeData->childList;
        }
        delete temp->nodeData;
        temp = temp->next;
    }
    wd.clear();
    dataStack.clear();
}

/*
Search a node in the current directory
	If one is found, return a pointer to that node
	If one is not found, return NULL
*/
Node* OS::search_item(string fname){
    return wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname);
}

/*
Delete a node in the current directory
	Note: this function only deletes (permanently) the node, not the Data obj the node points to
*/
void OS::del(string fname){
    if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname) == NULL)
    {
        cout << "Error: cannot find file or directory '" << fname << "'" << endl;
        return;
    }
    else
    {
        wd.top()->nodeData->childList->deleteNode(wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname));
    }
}

/*
Create a file or a folder, use boolean isFolder to tell (true if folder, false if file)
*/
void OS::create_item(string fname, bool isFolder){
    if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname) != NULL)
    {
        cout << "Directory or file name '" << fname << "' already exists" << endl;
        return;
    }
    else
    {
        Data* newItem = new Data;
        newItem->size = 0;
        newItem->name = fname;
        if (isFolder) // if folder, allocate memory for childlist
        {
            newItem->childList = new MList;
            newItem->isFolder = true;
        }
        else
        {
            newItem->isFolder = false;
        }
        dataStack.push_top(newItem);
        wd.top()->nodeData->childList->push_bottom(newItem);
        wd.top()->nodeData->childList->sortByNameSelection();
    }
}

/*
Read or write a file according to the boolean isRead (true = read, false = write)
*/
void OS::file(string fname, bool isRead){
    if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname) == NULL)
    {
        cout << "Error: cannot find file '" << fname << "'" << endl;
        return;
    }
    else if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData->isFolder == true)
    {
        cout << "Error: '" << fname << "' is not a file" << endl;
        return;
    }  
    
    if(isRead)
    {
        cout << wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData->text << endl;
    }
    else
    {
        cout<<"$> ";
	    string input;
		getline(cin,input);
        wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData->text = input;
        wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData->size = input.size();
    }
}


//Change directory
void OS::cd_dir(string fname){
	if(fname == ".."){
		//this option takes you up one directory (move the directory back one directory)
        if (wd.top()->nodeData != root_data)
        {
            wd.pop_top();
        }

	}else if(fname == "~"){
		//this option takes you directly to the home directory (i.e., root).
        if (wd.top()->nodeData == root_data)
        {
            return;
        }
        while (wd.top()->nodeData != root_data)
        {
            wd.pop_top();
        }  

	}else{
        if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname) == NULL)
        {
            cout << "Error: cannot find directory '" << fname << "'" << endl;
            return;
        }
        else if (wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData->isFolder == false)
        {
            cout << "Error: '" << fname << "' is not a directory" << endl;
            return;
        }  
        else
        {
            wd.push_top(wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname)->nodeData);
        }
	}
}

//Print list of item in the current directory, the way you print it will be according to the passed-in option
void OS::ls(string option){
	if(option == "-d"){
		//Default option - print the list of items in the current directory from top to bottom.
		//use a single space as delimiter.
        wd.top()->nodeData->childList->printTtB(" ");
	}

	else if(option == "-sort=name"){
		//Use Insertion Sort to sort items in the current directory and print from top to bottom (alphabetically A-Za-z).
		//use a single space as delimiter.
        wd.top()->nodeData->childList->sortByNameInsertion();
        wd.top()->nodeData->childList->printTtB(" ");

	}else if(option == "-r"){
		//Reverse print the list of items in the current directory (i.e., print form bottom to top).
		//use single space as delimiter.
        wd.top()->nodeData->childList->printBtT(" ");
        
	}else if(option == "-sort=size"){
		//Sort list by size and print the list of items in the current directory from top to bottom.
		//use single space as delimiter.
        wd.top()->nodeData->childList->sortBySizeSelection();
        wd.top()->nodeData->childList->printTtB(" ");

	}
	else{
		cout<<"usage: ls [optional: -r for reverse list, -sort=size to sort list by file size, -sort=name to soft list by name]";
	}
}

void OS::present_working_dir(){
    Node* temp = wd.bottom();
    while (temp != NULL)
    {
        cout << temp->nodeData->name << '/';
        temp = temp->prev;
        
    } 
}
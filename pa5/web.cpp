#include "web.h"

Web::Web() 
{ 
    
}

Web::~Web() 
{ 

}

bool Web::read_graph(const char *filename) 
{ 
    ifstream ifile(filename);
    int total_pages;
    ifile >> total_pages;
    if (ifile.fail())
    {
        return false;
    }
    num_pages = total_pages;
   
    int temp_id;
    string temp_url;
    double temp_rank;
    
    int id_single;
    
    for (int i=0; i<total_pages; i++) // get id/url/rank of page
    {
        ifile >> temp_id >> temp_url >> temp_rank;
        if (ifile.fail())
        {
            return false;
        } 
        Page temp(temp_id, temp_url, temp_rank); // makes new page 
        
        string id_str;
        getline(ifile, id_str); // use for parsing out ids
        getline(ifile, id_str);
        if (ifile.fail())
        {
            return false;
        } 
        stringstream id_ss(id_str);
        
        id_ss >> id_single;
        
        while (!id_ss.fail())
        {
            temp.set_links(id_single);
            id_ss >> id_single; 
        }
        pagelist.push_back(temp); // copies new page into vector
    }
    ifile.close();
    return true;
}

bool Web::write_graph(const char *filename)
{ 
    ofstream ofile(filename);
    if(ofile.fail())
    {
        return false;
    }
    
    ofile << num_pages << endl;
    for (int i=0; i<num_pages; i++) // outputs the same info for each page
    {
        vector<int> temp = pagelist[i].get_links();
        
        ofile << pagelist[i].get_id() << endl;
        ofile << '\t' << pagelist[i].get_url() << endl;
        ofile << '\t' << pagelist[i].get_rank() << endl;
        ofile << '\t';
        for (int i=0; i<(int)temp.size(); i++)
        {
            ofile << temp[i] << " ";
        }
        ofile << endl;
    }
    
    ofile.close();
    return true;
}

void Web::calculate_rank(int S,int N)
{
    int pages[num_pages]; // two vectors to keep track of the walkers
    int pages_last[num_pages];
    double temp;
    int divided = N/num_pages;
    int num_links;
    
    for (int i=0; i<num_pages; i++)
    {
        pages_last[i] = divided; // divides the walkers equally
        pages[i] = 0; // initializes to zero
    }
    
    for (int i=0; i<S; i++)
    {
        for (int j=0; j<num_pages; j++)
        {
            vector<int> temp = pagelist[j].get_links(); // copies over the links to new vector
            num_links = temp.size(); 
            for (int k=0; k<pages_last[j]; k++)
            {
                pages[temp[rand() % num_links]]++; // randomly selects one of the links and increases count
            }
        }
        for (int r=0; r<num_pages; r++)
        {
            pages_last[r] = pages[r];
            pages[r] = 0; // reuses the counter arrays
        }
    }
    
    for (int i=0; i<num_pages; i++)
    {
        temp = pages_last[i]/(double)N; // gets pagerank as a decimal value
        pagelist[i].set_rank(temp);
    }
}
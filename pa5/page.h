#include <string>
using std::string;
#include<vector>
using std::vector;
class Page { 
public:
  Page(int id, string url, double rank);
  //setters and getters
  int get_id();
  void set_id(int);
  string get_url();
  void set_url(string);
  double get_rank();
  void set_rank(double);
  vector<int> get_links();
  void set_links(int);
private:
  //Data members: id, url, pagerank, links , .. etc
  int id;
  string url;
  double rank;
  vector<int> links;
};  

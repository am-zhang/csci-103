#include "page.h"

Page::Page(int id, string url, double rank)
{
    this->id = id;
    this->url = url;
    this->rank = rank;
}

int Page::get_id()
{
    return id;
}

void Page::set_id(int new_id)
{
    id = new_id;
}

string Page::get_url()
{
    return url;
}

void Page::set_url(string new_url)
{
    url = new_url;
}

double Page::get_rank()
{
    return rank;
}

void Page::set_rank(double new_rank)
{
    rank = new_rank;
}

vector<int> Page::get_links()
{
    return links;
}

void Page::set_links(int new_links)
{
    links.push_back(new_links);
}
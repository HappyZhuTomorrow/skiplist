#include <iostream>
#include "skiplist.hpp"


int main()
{
	srand((unsigned int)time(NULL));
	SkipList<int, std::string> skiplist(6);
	skiplist.insert_element(1, "main.cpp"); 
	skiplist.insert_element(3, "test.cpp"); 
	skiplist.insert_element(7, "link.cpp"); 
	skiplist.insert_element(8, "a.cpp"); 
	skiplist.insert_element(9, "b.cpp"); 
	skiplist.insert_element(19, "2.cpp"); 
	skiplist.insert_element(19, "456.cpp"); 
	skiplist.insert_element(5, "456.cpp"); 
	skiplist.insert_element(4, "456.cpp"); 
	skiplist.insert_element(12, "456.cpp"); 
	skiplist.insert_element(21, "456.cpp"); 
	skiplist.insert_element(5, "456.cpp"); 
	skiplist.insert_element(6, "456.cpp"); 
	skiplist.insert_element(2, "456.cpp");
	skiplist.display_list();
	skiplist.search_element(2);
	skiplist.search_element(400);
	skiplist.delete_element(1);
	skiplist.delete_element(7);
	skiplist.delete_element(400);
	skiplist.display_list();
	std::cout << skiplist.size() << std::endl;
	// for(int i = 0; i < 1000; i++)
	// {
	// 	std::string str = "to be or not to be";
	// 	skiplist.insert_element(i,str);
	// }
	// skiplist.display_list();
}
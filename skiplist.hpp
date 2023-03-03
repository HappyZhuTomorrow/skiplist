#ifndef _SKIPLIST_
#define _SKIPLIST_

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <time.h>

template<typename K, typename V>
class Node
{
private:
    K key;
    V val;
public:
    // Node();
    ~Node();
    Node(K key, V val, int level);
    K get_key() const;
    V get_value() const;
    void set_value(V val);
public:
    int node_level; /*0开始*/
    Node<K,V> **forward;
};

// template<typename K, typename V>
// Node<K,V>::Node(/* args */)
// {
// }

template<typename K, typename V>
Node<K,V>::~Node()
{
    delete[] this->forward;
}

template<typename K, typename V>
Node<K,V>::Node(K key, V val, int level)
{
    this->key = key;
    this->val = val;
    this->node_level = level;
    this->forward = new Node<K,V>*[level+1];
    memset(this->forward, 0, sizeof(Node<K,V>*)*(level+1));
}

template<typename K, typename V>
K Node<K,V>::get_key() const
{
    return this->key;
}

template<typename K, typename V>
V Node<K,V>::get_value() const
{
    return this->val;
}

template<typename K, typename V>
void Node<K,V>::set_value(V val)
{
    this->val = val;
}


template<typename K, typename V>
class SkipList
{
private:
    int _max_level; /*最大层数*/
    int _current_level; /*当前层数*/
    Node<K,V>* _head; /*头节点*/

    /*文件读取*/
    // std::ofstream _file_writer;
    // std::ifstream _file_reader;

    int _element_count; /*节点个数*/
    

public:
    SkipList();
    ~SkipList();
    SkipList(int max_level);
    int get_random_level();
    void display_list();
    int insert_element(K, V);
    bool search_element(K);
    void delete_element(K);
    int size();
};

template<typename K, typename V>
SkipList<K,V>::SkipList()
{
    this->_max_level = 6;
    this->_current_level = 0;
    K k;
    V v;
    this->_head = new Node<K,V>(k,v,this->_max_level);
    this->_element_count = 0;
    std::cout << "the default skiplist created successfully" << std::endl;
}

template<typename K, typename V>
SkipList<K,V>::~SkipList()
{
    Node<K,V>* cur = this->_head->forward[0];
    Node<K,V>* curptr;
    while(cur)
    {
        curptr = cur->forward[0];
        cur->~Node();
        cur = curptr;
    }
    delete this->_head;
    std::cout << "deleted the skip list successfully" << std::endl;
}

template<typename K, typename V>
SkipList<K,V>::SkipList(int max_level)
{
    this->_max_level = max_level;
    this->_current_level = 0;
    K k;
    V v;
    this->_head = new Node<K,V>(k,v,this->_max_level);
    this->_element_count = 0;
    std::cout << "the skiplist created successfully" << std::endl;
}


template<typename K, typename V>
int SkipList<K,V>::get_random_level()
{
    // srand((unsigned int)time(NULL));
    int k = 1;
    while(rand() % 2)
    {
        k++;
    }
    return (k < this->_max_level) ? k : this->_max_level;
}

template<typename K, typename V>
void SkipList<K,V>::display_list()
{
    std::cout << "\n=====display the skip list start=====\n";
    for(int i = this->_current_level; i >= 0; i--)
    {
        Node<K,V>* node = this->_head->forward[i];
        std::cout << "level " << i << ": ";
        while(node != nullptr)
        {
            std::cout << node->get_key() << ":" << node->get_value() << "; ";
            node = node->forward[i];
        }
        std::cout << '\n';
    }
    std::cout << "=====display the skip list end =====\n\n";
    return;
}



/* 
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+
                                    
                                        40 = turn_point[0] , 60 = curptr

*/
/*0 success, 1 fail*/
template<typename K, typename V>
int SkipList<K,V>::insert_element(K key, V val)
{
    // std::cout <<"\ndebug inserting the key is " << key <<std::endl;
    Node<K,V>* curptr = this->_head;


    Node<K,V> *turn_points[this->_max_level+1];
    memset(turn_points, 0, sizeof(Node<K,V>*)*(this->_max_level + 1));

    // std::cout << "test1" << std::endl;
    for(int i = this->_current_level; i >= 0; i--)
    {
        while ( (curptr->forward[i]) && (curptr->forward[i]->get_key() < key) )
            curptr = curptr->forward[i];
        turn_points[i] = curptr;
    }
    // std::cout << "test2" << std::endl;
    curptr = curptr->forward[0];
    // std::cout << "test3" << std::endl;
    // std::cout << "debug " << (int)(curptr == nullptr) << std::endl;
    // std::cout << "debug" << std::endl;
    /*如果判断条件改为 curptr->get_key() == key && curptr != nullptr 会报错*/
    /*可能是因为先执行curptr->get_key() == key ，但此时 curptr为nullptr，出现报错*/
    if(curptr != nullptr && curptr->get_key() == key)
    {
        std::cout << "the key " << key << " exist,insert failed!" << std::endl;
        return 1;
    }
    // std::cout << "test4" << std::endl;
    if (curptr == nullptr || curptr->get_key() != key )
    {

        int insert_level = this->get_random_level();
        // std::cout << "debug the insert_level is " << insert_level << std::endl;
        if(insert_level > this->_current_level)
        {
            for(int i = this->_current_level+1; i < insert_level+1; i++)
            {
                turn_points[i] = this->_head;
            }
            this->_current_level = insert_level;
        }

        Node<K,V>* insert_node = new Node<K,V>(key, val, insert_level);
        // std::cout << "test45" << std::endl;
        for(int i = 0; i <= insert_level; i++)
        {
            insert_node->forward[i] = turn_points[i]->forward[i];
            turn_points[i]->forward[i] = insert_node;
            
        }
        std::cout << "the key:" << key <<" and the value:" << val << " are successfully inserted" << std::endl;
        this->_element_count++;
    }
    
    return 0;
}

/* 
                           +------------+
                           |  search 60 |
                           +------------+
level 4     +-->1+                                                    100
                 |
                 |                      
level 3         1+-------->10+                               70       100
                                                     
                                                     
level 2         1          10         30                     70       100
                                                     
                                                     
level 1         1    4     10         30                     70       100
                                                     
                                                     
level 0         1    4   9 10         30   40        60      70       100
                                               
                                    
                                        

*/
template<typename K, typename V>
bool SkipList<K,V>::search_element(K key)
{
    Node<K,V>* cur = this->_head;
    for(int i = this->_current_level; i >= 0; i--)
    {
        while( (cur->forward[i] != nullptr) && (cur->forward[i]->get_key() < key) )
        {
            cur = cur->forward[i];
        }
    }
    cur = cur->forward[0];

    if(cur != nullptr && cur->get_key() == key)
    {
        std::cout << "successfully found the key: " << key << ",value: " << cur->get_value() << std::endl;
        return true;
    }
    else
    {
        std::cout << "not found the key: " << key << std::endl;
        return false;
    }

}

/* 
                           +------------+
                           |  delete 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      delete 
level 3         1+-------->10+-------------------------------> 70       100
                                                 
                                               +----+
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+
                                    
                                        

*/
template<typename K, typename V>
void SkipList<K,V>::delete_element(K key)
{
    Node<K,V>* curptr = this->_head;
    Node<K,V> *turn_points[this->_max_level+1];
    memset(turn_points, 0, sizeof(Node<K,V>*)*(this->_max_level + 1));

    for(int i = this->_current_level; i >= 0; i--)
    {
        while( (curptr->forward[i] != nullptr) && (curptr->forward[i]->get_key() < key) )
        {
            curptr = curptr->forward[i];
        }
        turn_points[i] = curptr;
    }

    curptr = curptr->forward[0];
    
    if(curptr != nullptr && curptr->get_key() == key)
    {
        /*found the key*/
        int level = curptr->node_level;
        for(int i = 0; i <= this->_current_level; i++)
        {
            /*update the turn_point*/
            if(turn_points[i]->forward[i] == curptr)
            {
                turn_points[i]->forward[i] = curptr->forward[i];
            }
            else
            {
                break;
            }
        }

        /*update the _current_level*/
        if(level >= this->_current_level)
        {
            while(this->_current_level >=0 && this->_head->forward[this->_current_level] == nullptr)
            {
                this->_current_level -= 1;
            }
        }

        this->_element_count--;
        std::cout << "successfully deleted the key: " << key << std::endl;
        return;
    }
    std::cout << "not found the key: " << key << ",can't delete" <<std::endl;
    return;
}

template<typename K, typename V>
int SkipList<K,V>::size()
{
    return this->_element_count;
}
#endif
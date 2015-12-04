#ifndef __HH_DFT_
#define __HH_DFT_

#include <map>
#include <vector>
#include <string>

#include "headers.h"
#include "Graph.hh"
#include "functions.h"

class node {
public:
  node(int a,node *b)
  {
    number=a;
    next=b;
  }

  /* virtual ~node() { */
  /*   delete next; */
  /* } */

  int number;
  node *next;
};
 
class stacks {
public:
  stacks(node * a=NULL)
  {
    top=NULL;
  }

  /* virtual ~stacks() { */
  /*   delete top; */
  /* } */

  void push(int a)
  {
    if (top==NULL)
      top =new node(a,NULL);
    else top=new node(a,top);
  }

  void pop()
  {
    node *b=top;
    top=top->next;
    delete b;
  }

  node * top;
}; //保存已加入路径结点的栈

// std::vector<StrPath> dft(const int &n, const int &v, Graph &graph);
void dft(const int &n, const int &v, Graph &graph);
node* neighbour(int a);// ,int b

#endif	// __HH_DFT_

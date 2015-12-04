#include "dft.hh"

map<int,int> map_next;//每个结点的下一个邻接点，即刚从栈顶弹出的结点
int states[maxnum];//保存结点状态的数组
stacks aray[maxnum];//={stacks(NULL),stacks(NULL),stacks(NULL),stacks(NULL),stacks(NULL),stacks(NULL),stacks(NULL),stacks(NULL)};

// std::vector<StrPath> dft(const int &n, const int &v, Graph &graph) {
void dft(const int &n, const int &v, Graph &graph) {
  for (int i = 0; i < maxnum; ++i) {
    aray[i] = stacks(NULL);
  }

  for (std::size_t i = 1; i < n; ++i)
    for (std::size_t j = i + 1; j <= n; ++j)
      if (0 < graph.matrix[i][j] && graph.matrix[i][j] < inf) {
	aray[i].push(j);
	aray[j].push(i);
      }

  int start=v;
  int end=n;
  int cur_node;//当前结点，即为栈顶的结点
  // int next_node=8;//当前结点的下一个邻接点，即刚从栈顶弹出的结点，初始化为8
  stacks stack(NULL);

  // int i, j; 

  vector<StrPath> res;

  //初始化map_next
  for(int i=0;i<maxnum;i++) {
    map_next[i] = -1;
  }

  node* neighbour(int a);// ,int b // HOW COULD IT BE???????????????????????????
  stack.push(start);//将起点入栈
  states[start]=1;//将起点标记为入栈状态
  int min = inf;
  while(NULL != stack.top) { //栈不为空
    int sum = 0;
    // int counter = 0;
    vector<int> stk;
    if (stack.top->number==end) {
      // cout<<"end";
      node* abc=stack.top;
      while(abc->number != start) {
	// cout<<abc->number<<" <- ";
	int y = abc->number;
	stk.push_back(y);
	abc=abc->next;
	int x = abc->number;
	sum += graph.matrix[x][y];
      }
      if (sum < min) {
	min = sum;
      }
      stk.push_back(start);

      StrPath strPath;
      // std::string str;
      // std::bitset<maxnum> nodes;
      for (int i = stk.size()-1; i > 0; --i) {
	strPath.nodes[stk[i]] = 1;
	strPath.path.push_back(stk[i]);
	// strPath.path += std::to_string(stk[i]);
	// strPath.path += "-";
      	// cout << stk[i] << " -> ";
      }
      strPath.nodes[stk[0]] = 1;
      strPath.path.push_back(stk[0]);
      // strPath.path += std::to_string(stk[0]);
      res.push_back(strPath);
      // cout << stk[0] << " : " << sum << endl;

      // cout << start << endl;//输出已找到的路径
      stack.pop();//将栈顶结点弹出
      states[end]=0;//清除终点的状态
      map_next[end]=-1;
    }
    else {
      cur_node=stack.top->number;
      if(neighbour(cur_node) != NULL) {//邻居不为空
	node *d =neighbour(cur_node);
	map_next[cur_node] = d->number;
	cur_node=d->number;
	stack.push(cur_node);
	states[cur_node]=1;
      }
      else {
	stack.pop();
	states[cur_node]=0;
	map_next[cur_node] = -1;
      }
    }
  }
  // cout << endl << "min: " << min << endl;
  graph.v_possible_paths[v][n].swap(res);
  // return res;
}

node* neighbour(int a) {//,int b
  node *abc=aray[a].top;
  while ((NULL!=abc)) {//结点abc不空
    if( states[abc->number]==1 ) {//已经在栈stack里了
      abc=abc->next;
    }
    else {//不在栈stack里
      if(-1 == map_next[a]) {//就要abc作为返回值
	while(NULL!=abc && states[abc->number]==1) {
	  abc = abc->next;
	}
	return abc;
      }
      else if(abc->number == map_next[a]) {
	abc=abc->next;
	while(NULL!=abc && states[abc->number]==1) {
	  abc = abc->next;
	}
	return abc; //将abc的下一个结点返回
      }
      else {
	abc=abc->next;
      }
    }
  }
  return NULL;
}


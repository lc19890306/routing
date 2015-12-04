Findleastelement(Vertexes)
for each node x in Vertexes
for each unchecked node y other than x in Vertexes
compare w[x] and w[y]
if w[x] > w[y] then
u = y
x = y
break
remove node u from Vertexes
return u

Dijkstra(Graph, weight, source)
for each node v in all Vertexes
w[v] = inf
path[v] = NULL
end for
V = all Vertexes
w[s] = 0
path[s] = s
  while V is not empty
  node u = FindLeastElement(V)
for each node x in node u's all nerghbors
compare w[x] and weight(path[u] + (u, x))
if w[x] > weight(path[u] + (u, x)) then
w[x] = weight(path[u] + (u, x))
path[x] = path[u] + (u, x)

end if
end for
end while

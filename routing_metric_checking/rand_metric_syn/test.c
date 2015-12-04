
int weight1(int path[3]) {
return (((!path[0]) ? (!(path[1]--)) : path[2]) ? ((!path[1]) - (~path[0])) : ((path[1] ? path[1] : path[1]) > (~(path[1]--))));
}

int weight2(int path0[3], int path1[3]) {
return (((!path0[1]) ? (!(path1[1]--)) : path1[1]) ? ((!path1[2]) - (~path1[1])) : ((path0[0] ? path1[1] : path1[2]) > (~(path0[2]--))));
}

int main() {
int path0[3];
int path1[3];
int path2[3];

__ESBMC_assume(0 < path0[0] && path0[0] < 100
&& 0 < path1[0] && path1[0] < 100
&& 0 < path2[0] && path2[0] < 100
&& 0 < path0[1] && path0[1] < 100
&& 0 < path1[1] && path1[1] < 100
&& 0 < path2[1] && path2[1] < 100
&& 0 < path0[2] && path0[2] < 100
&& 0 < path1[2] && path1[2] < 100
&& 0 < path2[2] && path2[2] < 100);

int res0 = weight1(path0);
int res1 = weight1(path1);
int res2 = weight2(path0, path2);
int res3 = weight2(path1, path2);
int res4 = weight2(path2, path0);
int res5 = weight2(path2, path1);

__ESBMC_assume(res0 > res1);
__ESBMC_assert(res4 >= res5, "left-isotonicity violated!\n");

return 0;
}

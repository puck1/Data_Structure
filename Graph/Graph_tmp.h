
// - - - - - 基本操作的算法描述 - - - - -
Status CreateGraph(Graph *G,VertexType *V,AdjMatrix VR){
    //V是图的顶点集，VR是图中弧的集合，按V和VR的定义构造图G

}//CreateGraph

Status DestroyGraph(Graph *G){
    //销毁图G

}//DestroyGraph

int LocateVex(Graph G,VertexType u){
    //u和图G中顶点有相同特征，若图G中存在顶点u，则返回该顶点在图中位置，否则返回“空”

}//LocateVex

VertexType GetVex(Graph G,int i){
    //i是图G中某个顶点的位置，返回i的值

}//GetVex

Status PutVex(Graph *G,VertexType v,VertexType value){
    //v是图G中某个顶点，对v赋值valve

}//PutVex

VertexType FirstAdjVex(Graph G,VertexType v){
    //v是图G中某个顶点，返回v的第一个邻接顶点，若顶点在G中没有邻接顶点，则返回“空”

}//FirstAdjVex

VertexType NextAdjVex(Graph G,VertexType v,VertexType w){
    //v是图G中某个顶点，w是v的邻接顶点，返回v的（相对于w的）下一个邻接顶点；
    //若顶点在G中没有邻接顶点，则返回“空”

}//NextAdjVex

Status InsertVex(Graph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中增添新顶点v

}//InsertVex

Status DeleteVex(Graph *G,VertexType v){
    //v和图G中顶点有相同特征，在图G中删除顶点v及其相关的弧

}//DeleteVex

Status InsertArc(Graph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v>

}//InsertArc

Status DeleteArc(Graph *G,VertexType v,VertexType w){
    //v和w是图G中两个顶点，在G中删除弧<v,w>，若G是无向的，则还删除对称弧<w,v>

}//DeleteArc

Status DFSTraverse(Graph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行深度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

Status BFSTraverse(Graph G,Status (*Visit)(VertexType)){
    //Visit是顶点的应用函数，对图G进行广度优先遍历，在遍历过程中
    //对每个顶点调用函数Visit一次且仅一次。一旦Visit()失败，则操作失败

}//DFSTraverse

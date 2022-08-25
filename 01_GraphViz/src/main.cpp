#include <iostream>
#include "SimpleGraph.h"

#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
const double PI = 3.14159265358979323;

using std::cout;	using std::endl;

void Welcome();

SimpleGraph* input();
void InitNode(SimpleGraph& G);
void UpdateGraph(SimpleGraph& G);

// Main method
int main() {
    SimpleGraph* p_G = nullptr;

    Welcome();

    p_G = input();
    InitNode(*p_G);
    DrawGraph(*p_G);

    double excuteTimeInS;
    std::cout << "input excute time in seconds:" << std::endl;
    std::cin >> excuteTimeInS;

    time_t startTime = time(NULL);
    while(difftime(time(NULL),startTime) <= excuteTimeInS){
        UpdateGraph(*p_G);
        DrawGraph(*p_G);
    }
    DrawGraph(*p_G);

    std::cout << "finish!" << std::endl;

    delete p_G;
    return 0;


//    while(1)
//    {
//        UpdateGraph(*p_G);
//        DrawGraph(*p_G);
////        cout << "drawing..." << endl;
////        getchar();
//    }

//    getchar();

    return 0;
}


double CalcSquareDistance(const SimpleGraph& G,int start,int end){
    Node startNode = G.nodes[start];
    Node endNode = G.nodes[end];

    return pow((startNode.x - endNode.x),2) + pow((startNode.y - endNode.y),2);
}

#define K_REPL 0.001
#define K_ATTR 0.001
void UpdateGraph(SimpleGraph& G){
    int N_nodes = G.nodes.size();
    std::vector<Node> nodes_forces(N_nodes,{0.0,0.0});

    int start,end;
    double force,angle;

    //排斥力
    for(int i=0;i<N_nodes;i++)
        for(int j=i+1;j<N_nodes;j++){
            start = i;
            end = j;

            force = K_REPL/sqrt(CalcSquareDistance(G,start,end));
            angle = atan2(G.nodes[end].y - G.nodes[start].y,G.nodes[end].x - G.nodes[start].x);
            nodes_forces[start].x -= force*cos(angle);
            nodes_forces[start].y -= force*sin(angle);
            nodes_forces[end].x += force*cos(angle);
            nodes_forces[end].y += force*sin(angle);
        }


    //吸引力
    for(Edge edge:G.edges)
    {
        start = edge.start;
        end = edge.end;

        force = CalcSquareDistance(G,start,end) * K_ATTR;
        angle = atan2(G.nodes[end].y - G.nodes[start].y,G.nodes[end].x - G.nodes[start].x);
        nodes_forces[start].x += force*cos(angle);
        nodes_forces[start].y += force*sin(angle);
        nodes_forces[end].x -= force*cos(angle);
        nodes_forces[end].y -= force*sin(angle);
    }

    //根据力进行调整位置
    for(int i=0;i<N_nodes;i++){
        G.nodes[i].x += nodes_forces[i].x;
        G.nodes[i].y += nodes_forces[i].y;
    }
}

void InitNode(SimpleGraph& G){

    std::size_t n_nodes = G.nodes.size();

    for(std::size_t i=0;i<n_nodes;i++){
        G.nodes[i].x = cos(2*PI*i/n_nodes);
        G.nodes[i].y = sin(2*PI*i/n_nodes);
    }
}


SimpleGraph* input()
{
    std::string fileName;
    std::ifstream is;

    while(1)
    {
        cout << "Please enter graph file name: ";
        std::getline(std::cin, fileName);

        is.open(fileName);
        if(is)
            break;
        else
        {
            cout << "Invalid file name" << endl;
        }
    }

    int n_nodes;
    Edge edge;
    SimpleGraph* p_G = new SimpleGraph();

    is >> n_nodes;
    p_G->nodes.resize(n_nodes);
    while (is >> edge.start >> edge.end) {
        p_G->edges.push_back(edge);
    }
    return p_G;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

// Frederick Luke Jamar
// graph.cpp
// Performance Evaluation
#include <iostream>
#include <queue>
#include <omp.h>
using namespace std;


class Graph {
public:
	Graph(int n=0);         // A correction was made here, default parameters
	~Graph();               // should be in the prototype. Not the definition
	int edgeCount;
	int vertexCount;
	int missCount;
	int vertTraversed;
	void addEdge(int src, int tar);
	void BFTraversal();

private:
	bool** adjMat;
	void BFS(int n, bool marked[]);
};

Graph::Graph(int n) {       // default parameter was moved for compilation
	vertexCount = n;
	edgeCount = 0;
	missCount = 0;
	vertTraversed = 0;
	
	if(n == 0)
		adjMat = 0;
	else {
		adjMat = new bool* [n];
		#pragma omp parallel for
		for(int i=0; i < n; i++)
			adjMat[i] = new bool [n];
		
		#pragma parallel for
		for(int i=0; i < n; i++)
			for(int j=0; j < n; j++)
				adjMat[i][j] = 0;
	}
}

Graph::~Graph() {
	for(int i=0; i < vertexCount; i++)
		delete [] adjMat[i];
		
	delete [] adjMat;
}

void Graph::addEdge(int src, int tar) {
	if(src < 0 || tar < 0 || src > vertexCount-1 || tar > vertexCount-1)
		missCount++;
	else if(src == tar)
		missCount++;
	else if (adjMat[src][tar] == 1)
		missCount++;
	else {
		adjMat[src][tar] = 1;
		edgeCount++;
	}
}

void Graph::BFTraversal() {
    
    // initialize vertex checker and set all to false
    bool marked[vertexCount];
    
    for (int i = 0; i < vertexCount; ++i)
        marked[i] = false;
    
    // the traversal will begin adding at 0
    #pragma omp parallel for
    for (int i = 0; i < vertexCount; ++i) {
        if (marked[i] == false)
            BFS(i, marked);
    }
}

void Graph::BFS(int n, bool marked[]) {
    
    
    int vert = n;
    queue <int> vertexQueue;
    
    // set the first vertex before testing for targets
    vertexQueue.push(vert);
    marked[vert] = true;
    
    while (!vertexQueue.empty()) {
        
        // add all target vetecies to the queue
        for (int i = 0; i < vertexCount; ++i) {
            if (adjMat[vert][i] && !marked[i]) {
                vertexQueue.push(i);
                marked[i] = true;
		vertTraversed++;
            }
        }
        // display and pop the source vertex
        vert = vertexQueue.front();
        vertexQueue.pop();
    }
}


// Take 3 commandline arguments from he usr when running the program
// Creates an undirected pgraph of n verticies and x endges to be
// parallelized on y number or processor threads.

int main(int argc, char **argv)
{
    

    // Create a graph given in the above diagram
    int vertices = atoi(argv[1]);
    int numEdges = atoi(argv[2]);
        omp_set_num_threads(atoi(argv[3]));

    Graph g(vertices);



    for (int i = 0; i < numEdges; i++) {
         int j = i % vertices;
        g.addEdge(j, rand()%vertices);
     }


 
    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 0) \n";
    g.BFTraversal();
    
    cout << "Verticies: " << g.vertexCount << endl
	 << "Edges: " << g.edgeCount << endl
	 << "Random Misses: " << g.missCount << endl;
	 
    cout << "Verticies traversed: " << g.vertTraversed << endl;
 
    return 0;
}

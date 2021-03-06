#include "my_tarjan.cpp"
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/transitive_closure.hpp>

using namespace boost;

int main(int argc, char*argv[])
{
    //An example of alternative way to design a graph "by hand" in order to use our algorithm
    /*typedef std::pair<int, int> Edge;
    int n=1000;
    Edge edge_array[n-1];

    int k=0;
    for(int i=0;i<n-1;i++){
        edge_array[i]=Edge(i,i+1);
    }
    
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    Graph g(&edge_array[0], &edge_array[n-1], n);*/

    //This function takes a graph formatted by graphml fashion from the stdin
    Graph g;
    dynamic_properties dp;
    read_graphml(std::cin, g, dp);

    std::cout << "A directed graph:" << std::endl;
    print_graph(g, get(vertex_index,g));
    std::cout << std::endl;

    //This is the TarjanClass object
    TarjanClass<typeInt, typeInt, typeInt, typeBool, typeBool, typeInt> tarjan(&g);
    std::vector<int>* comp_tarjan = tarjan.tarjan_scc();

    //Printing
    IndexMap index = get(vertex_index,g);

    for (int i = 0; i != comp_tarjan->size(); ++i){
        std::cout << index[i] << " -> " << (*comp_tarjan)[i] << std::endl;
    }
    return 0;
}

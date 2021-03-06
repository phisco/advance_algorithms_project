
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/config.hpp>
#include <boost/graph/graphml.hpp>
#include "include_and_types.cpp"
#include "my_pearce.cpp"
#include "my_nuutila.cpp"
#include "my_tarjan.cpp"
#include "my_pearce_not_recursive.cpp"
#include <boost/timer/timer.hpp>
#include <iostream>
#include <vector>
#include <stack>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>


#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace boost;

typedef boost::erdos_renyi_iterator<boost::minstd_rand, Graph> ERGen;

template <class Result1, class Result2>
bool compare_results(const Result1 r1, const Result2 r2){
    if (r1.size() != r2.size())
        return false;
    int maxVal=0;
    int minVal=0;
    for(int i=0; i!=r1.size(); i++){
        maxVal = std::max(maxVal, r1[i]);
        minVal = std::min(minVal, r2[i]);
    }
    minVal-=1;
    maxVal+=1;
    int comp[maxVal];
    for (int i=0; i != maxVal; i++)
        comp[i]=minVal;

    for (int i = 0; i != r1.size(); i++){
        if (comp[r1[i]]==minVal) {
            for (int j = 0; j < i; j++) {
                if (r1[i] != r1[j] && r2[i] == r2[j])
                    return false;
            }
            comp[r1[i]] = r2[i];
        }
        if (comp[r1[i]]!=r2[i]){
            return false;
        }
    }
    return true;
}

std::vector<int> correct_nuutila_root(std::vector<int> vec){
    bool ok = false;
    while(!ok){
        ok = true;
        for(int i=0; i<vec.size(); i++){
            if(vec[i] != i && vec[i]!= vec[vec[i]]){
                vec[i]=vec[vec[i]];
                ok=false;
            }
        }
    }
    return vec;
}
int main(int, char*[])
{
    /*
    typedef std::pair<int, int> Edge;
    const int num_nodes = 8;
    enum nodes { A, B, C, D, E, F, G, H, I, L, M, N};
    char name[] = "ABCDEFGHILMN";

    Edge edge_array[] = { Edge(A, B),
                          Edge(B, C), Edge(B, H),
                          Edge(C, D), Edge(C, G),
                          Edge(D, E),
                          Edge(E, F), Edge(E, C),
                          Edge(G, F), Edge(G, D),
                          Edge(H, A), Edge(H, G),
                          Edge(I, L),
                          Edge(M, N), Edge(N, M)
    };
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    Graph g(edge_array, edge_array + num_arcs, num_nodes);
    std::cout << "A directed graph:" << std::endl;
    print_graph(g,name);
    std::cout << std::endl;
    */

    dynamic_properties dp;
    minstd_rand gen;
    // Create graph with 100 nodes and edges with probability 0.05
    int nodes= 30;
    Graph g(ERGen(gen, nodes, 0.05), ERGen(), nodes);

    write_graphml(std::cout, g, dp, true);



    // Tarjan's algorithm over the given graph
    std::cout << "Tarjan\t\t" << std::flush;
    TarjanClass<typeInt, typeInt, typeInt, typeBool, typeBool, typeInt> tarjan(&g);
    std::vector<int>* component = tarjan.tarjan_scc();

    // Nuutila's algorithm over the given graph
    std::cout << "Nuutila\t\t" << std::flush;
    NuutilaClass<typeInt, typeInt, typeBool> nuutila(&g);
    std::vector<int>* root_nuutila = nuutila.nuutila_scc();

    // Pearce's recursive algorithm over the given graph
    std::cout << "Pearce\t\t" << std::flush;
    PearceClass<typeInt> pearce(&g);
    std::vector<int>* rindex = pearce.pearce_scc();

    // Pearce's non recursive algorithm over the given graph
    std::cout << "PearceNR\t\t" << std::flush;
    PearceNR <typeBool, typeInt> pearcenr(&g);
    std::vector<int>* rindexNR = pearcenr.pearce_not_recursive_scc();

    /*std::cout << "Pearce_NR\t" << std::flush;
    std::vector<int> rindex_not_recursive(num_vertices(g));
    int num_pearce_not_recursive = pearce_not_recursive_scc(g, make_iterator_property_map(rindex_not_recursive.begin(), get(vertex_index, g)));
    */
    std::vector<int> bgl_component(num_vertices(g)), discover_time(num_vertices(g));
    std::vector<default_color_type> bgl_color(num_vertices(g));
    std::vector<Vertex> bgl_root(num_vertices(g));
    int num_bgl = strong_components(g,
                                make_iterator_property_map(bgl_component.begin(), get(vertex_index, g)),
                                root_map(make_iterator_property_map(bgl_root.begin(), get(vertex_index, g))).
                                        color_map(make_iterator_property_map(bgl_color.begin(), get(vertex_index, g))).
                                        discover_time_map(make_iterator_property_map(discover_time.begin(), get(vertex_index, g))));

    std::cout << "Number of components: bgl:"<< num_bgl << std::endl;
    //std::cout << "Total number of components: " << num << std::endl;


    std::cout << "No" << "\t->\tBgl\tTa\tNu\tPe\tPeNr" << std::endl;
    for (int i = 0; i != (*component).size(); ++i){
        std::cout << i << "\t->\t"<< bgl_component[i] << "\t" << (*component)[i] << "\t" << (*root_nuutila)[i] << "\t" << (*rindex)[i] << "\t" << (*rindexNR)[i] << std::endl;
    }
    return 0;
}



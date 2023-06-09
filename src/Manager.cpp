//
// Created by Gonçalo Pinto on 29/05/2023.
//

#include "Manager.h"
#include "Read_files.h"
#include <iostream>
#include "vector"
#include "../data_structures/Graph.h"
#include "../data_structures/VertexEdge.h"

using namespace std;

Manager::Manager() {

}

Graph Manager::get_graph() {
    return this->graph;
}

void Manager::set_graph(Graph newGraph) {
    this->graph = newGraph;
}

void Manager::ex4_1() {

}


void Manager::tspBacktrackBruteforce(Vertex* current, double current_cost, int num_visited, double& min_cost, std::vector<Vertex *>& tsp_path) {
    bool vi=false;
    double costt=0;
    if (num_visited == graph.getNumVertex()) {
        // refactor later (use dijkstra to find minimum between current and start[findVertex(0)])
        // for real world graphs use latitude and logitude for distance instead of dijkstra
        for(Edge* e: current->getAdj()){
            Vertex* w = e->getDest();
            if(w==graph.findVertex(0)){
                costt = costt + current->getPath()->getWeight();
                costt  += current->getPath()->getWeight();
                vi=true;
            }

        }
        if(vi==false) return;

        double cost = current_cost + costt;
        if (cost < min_cost) {
            min_cost = cost;
            tsp_path.clear();
            tsp_path.push_back(current);
            //? id may not be needed
            for (Edge* e = current->getPath(); e->getSource()->getId() != graph.findVertex(0)->getId(); e = e->getSource()->getPath()) {
                tsp_path.push_back(e->getSource());
            }

            std::reverse(tsp_path.begin(), tsp_path.end()-1);


        }

        return;

    }
    for (Edge* e: current->getAdj()) { // may need refactor
        Vertex* w = e->getDest();
        if (!w->isVisited()) {
            w->setVisited(true);
            w->setPath(e);
            tspBacktrackBruteforce(w, current_cost + e->getWeight(), num_visited + 1, min_cost, tsp_path);
            w->setVisited(false);
            w->setPath(nullptr);
        }
    }

}


std::vector<Vertex *> Manager::tspBruteforce() {

    std::vector<Vertex *> tsp_path;
    for (auto& v: graph.getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }
    double min_cost = std::numeric_limits<double>::max();
    auto init = graph.findVertex(0);
    init->setVisited(true);
    tspBacktrackBruteforce(init, 0, 1, min_cost, tsp_path);
    return tsp_path;
}


void Manager::tspBF(Vertex* current, std::vector<Vertex *>& tsp_path, double current_cost, double& min_cost, std::vector<Vertex *>& best_tsp_path) {
    auto numVertices = graph.getNumVertex();

    if (tsp_path.size() == numVertices) {
        auto returnEdge = graph.findEdge(current->getId(), graph.findVertex(0)->getId());
        if (returnEdge != nullptr) {
            auto total_cost = current_cost + returnEdge->getWeight();
            if (total_cost < min_cost) {
                min_cost = total_cost;
                best_tsp_path = tsp_path;
            }
        }
        return;
    }
    for (auto e : current->getAdj()) {
        auto neighborVertex = e->getDest();
        if (!neighborVertex->isVisited()) {
            neighborVertex->setVisited(true);
            tsp_path.push_back(neighborVertex);
            auto edgeCost = e->getWeight();
            tspBF(neighborVertex, tsp_path, current_cost + edgeCost, min_cost, best_tsp_path);
            tsp_path.pop_back();
            neighborVertex->setVisited(false);
        }
    }
}

double Manager::tspBF_aux(std::vector<Vertex *>& best_tsp_path) {
    std::vector<Vertex *> tsp_path;

    for (auto& v: graph.getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    double min_cost = std::numeric_limits<double>::max();

    auto init = graph.findVertex(0);
    init->setVisited(true);
    tspBF(init, tsp_path, 0, min_cost, best_tsp_path);

    return min_cost;
}




/*
int Graph::getNumVertex() const {
    return this->vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return this->vertexSet;
}*/





#include "Graph.h"
#include <limits>
#include <algorithm>
#include <utility>

using namespace std;

Graph::Graph() {
    this->numVertex = 0;
}

Graph::Graph(int nrVertex) {
    this->numVertex = nrVertex;
}

bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    auto *v1 = new Vertex(id);
    vertexSet.insert(v1);
    return true;
}
bool Graph::addVertex(const int &id, double longitude, double latitude) {
    if (findVertex(id) != nullptr)
        return false;
    auto *v1 = new Vertex(id, longitude, latitude);
    vertexSet.insert(v1);
    return true;
}

bool Graph::addVertex(const int &id, string name) {
    if (findVertex(id) != nullptr)
        return false;
    auto *v1 = new Vertex(id, std::move(name));
    vertexSet.insert(v1);
    return true;
}

bool Graph::addEdge(const int &sourc, const int &dest, double w) const {
    Vertex *v1 = findVertex(sourc);
    Vertex *v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    //v1->getAdj().insert(v2->getAdj().end(), new Edge(v1, v2, w));
    v1->addAdj(v1,v2,w);
    return true;
}


bool Graph::removeVertex(const int &id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it) {
        if ((*it)->getId() == id) {
            vertexSet.erase(it);
            return true;
        }
    }
    return false;
}


int Graph::getNumVertex() const {
    return vertexSet.size();
}

set<Vertex *, CompararVertex> Graph::getVertexSet() const {
    return vertexSet;
}

Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

int Graph::findVertexIdx(const int &id) const {
    int i = 0;
    for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it, ++i)
        if ((*it)->getId() == id)
            return i;
    return -1;
}


void Graph::resetVisited() {
    for (auto v : vertexSet)
        v->setVisited(false);
}

void Graph::resetDist() {
    for (auto v : vertexSet)
        v->setDist(0);
}

void Graph::resetPath() {
    for (auto v : vertexSet)
        v->setPath(nullptr);
}

void Graph::dijkstra(Vertex* source) {
    auto cmp = [](Vertex* a, Vertex* b) {
        return a->getDist() < b->getDist();
    };
    std::priority_queue<Vertex *, std::vector<Vertex *>, decltype(cmp)> pq(cmp);

    for (Vertex* v: vertexSet) {
        v->setVisited(false);
        v->setDist(std::numeric_limits<double>::max());
    }

    source->setDist(0);
    pq.push(source);
    while (!pq.empty()) {
        Vertex* u = pq.top(); pq.pop();
        u->setVisited(true);

        for (Edge* e: u->getAdj()) {
            Vertex* v = e->getDest();
            double w = e->getWeight();
            if (!v->isVisited() && u->getDist() != std::numeric_limits<double>::max() && u->getDist() + w < v->getDist()) {
                v->setDist(u->getDist() + w);
                pq.push(v);
            }
        }
    }
}

Edge * Graph::findEdge(const Vertex &source, const Vertex &dest) {
    for (auto v : getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e->getSource()->getId() == source.getId() && e->getDest()->getId() == dest.getId()) {
                return e;
            }
        }
    }
    return nullptr;
}

void Graph::tspBacktrackBruteforce(Vertex* current, double current_cost, int num_visited, double& min_cost, std::vector<Vertex *> &tsp_path) {
    if (num_visited == getNumVertex()) {
        double cost = current_cost;
        bool hasEdge = false;
        for (Edge* e: current->getAdj()) {
            if (e->getDest()->getId() == 0) {
                hasEdge = true;
                cost += e->getWeight();
                break;
            }
        }

        if (!hasEdge) {
            dijkstra(current);
            cost += findVertex(0)->getDist(); // use dijkstra to get the distance between current and 0
            // for real world graph use latitude and longitude (maybe haversine algorithm)
        }

        if (cost < min_cost) {
            min_cost = cost;
            tsp_path.clear();
            tsp_path.push_back(current);
            //? id may not be needed
            for (Edge* e = current->getPath(); e->getSource()->getId() != findVertex(0)->getId(); e = e->getSource()->getPath()) {
                tsp_path.push_back(e->getSource());
            }

            std::reverse(tsp_path.begin(), tsp_path.end());
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

double Graph::tspBruteforce(std::vector<Vertex *> &tsp_path) {
    for (auto& v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    double min_cost = std::numeric_limits<double>::max();

    auto init = findVertex(0);
    init->setVisited(true);
    tspBacktrackBruteforce(init, 0, 1, min_cost, tsp_path);

    return min_cost;
}


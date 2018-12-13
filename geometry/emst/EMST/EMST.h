#pragma once

#include "Edge.h"
#include "Linalg.h"
#include "Quadedge.h"

#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <map>

typedef std::vector<Edge*> EdgeList;
typedef std::vector<Vert*> PointsList;
typedef std::vector<QuadEdge*> QuadList;
typedef std::tuple<EdgeList, EdgeList> EdgePartition;
typedef std::tuple<PointsList, PointsList> PointsPartition;

class Delaunay
{
private:
    PointsList vertices_;
    QuadList edges_;

    void GenerateRandomVerts(int n);

    PointsPartition SplitPoints(const PointsList& points);

    Edge* MakeEdgeBetween(int a, int b, const PointsList& points);
    Edge* Connect(Edge* a, Edge* b);
    void Kill(Edge* edge);

    EdgePartition LinePrimitive(const PointsList& points);
    EdgePartition TrianglePrimitive(const PointsList& points);

    Edge* LowestCommonTangent(Edge*& left_inner, Edge*& right_inner);
    Edge* LeftCandidate(Edge* base_edge);
    Edge* RightCandidate(Edge* base_edge);
    void MergeHulls(Edge*& base_edge);

    EdgePartition Triangulate(const PointsList& points);

public:
    Delaunay(int n);

    QuadList GetTriangulation();
    EdgeList GetMST();
};

Delaunay::Delaunay(int n)
{
    edges_ = QuadList();
    GenerateRandomVerts(n);
}

void Delaunay::GenerateRandomVerts(int n)
{
    std::vector<std::vector<int>> buffer;

    for (int i = 0; i < n; i++)
    {
        std::vector<int> xy = { rand() % 800, rand() % 600 };
        buffer.push_back(xy);
    }

    std::sort(buffer.begin(), buffer.end());
    buffer.erase(std::unique(buffer.begin(), buffer.end()), buffer.end());

    for (int i = 0; i < buffer.size(); i++)
    {
        vertices_.push_back(new Vert(buffer[i][0], buffer[i][1]));
    }
}

void Delaunay::Kill(Edge* edge)
{
    Splice(edge, edge->Oprev());
    Splice(edge->Sym(), edge->Sym()->Oprev());

    QuadEdge* raw = (QuadEdge*)(edge - (edge->index()));
    edges_.erase(std::remove(edges_.begin(), edges_.end(), raw));
    delete raw;
}

PointsPartition Delaunay::SplitPoints(const PointsList& points)
{
    int halfway = (points.size() / 2);

    PointsList left(points.begin(), points.begin() + halfway);
    PointsList right(points.begin() + halfway, points.end());

    return PointsPartition(left, right);
}

Edge* Delaunay::MakeEdgeBetween(int a, int b, const PointsList& points)
{
    Edge* e = Edge::Make(edges_);

    e->setOrigin(points[a]);
    e->setDestination(points[b]);

    return e;
}

Edge* Delaunay::Connect(Edge* a, Edge* b)
{
    Edge* e = Edge::Make(edges_);

    e->setOrigin(a->destination());

    e->setDestination(b->origin());

    Splice(e, a->Lnext());
    Splice(e->Sym(), b);

    return e;
}

EdgePartition Delaunay::LinePrimitive(const PointsList& points)
{
    Edge* e = MakeEdgeBetween(0, 1, points);
    Edge* e_sym = e->Sym();
    return EdgePartition({ e }, { e_sym });
}

EdgePartition Delaunay::TrianglePrimitive(const PointsList& points)
{
    Edge* a = MakeEdgeBetween(0, 1, points);
    Edge* b = MakeEdgeBetween(1, 2, points);

    Splice(a->Sym(), b);

    if (CCW(points[0], points[1], points[2]))
    {
        Edge* c = Connect(b, a);
        return EdgePartition({ a }, { b->Sym() });
    }
    else if (CCW(points[0], points[2], points[1]))
    {
        Edge* c = Connect(b, a);
        return EdgePartition({ c->Sym() }, { c });
    }
    else
    {
        // collinear
        return EdgePartition({ a }, { b->Sym() });
    }
}

Edge* Delaunay::LowestCommonTangent(Edge*& left_inner, Edge*& right_inner)
{
    while (true)
    {
        if (LeftOf(left_inner, right_inner->origin()))
        {
            left_inner = left_inner->Lnext();
        }
        else if (RightOf(right_inner, left_inner->origin()))
        {
            right_inner = right_inner->Rprev();
        }
        else
        {
            break;
        }
    }

    Edge* base_edge = Connect(right_inner->Sym(), left_inner);
    return base_edge;
}

Edge* Delaunay::LeftCandidate(Edge* base_edge)
{
    Edge* left_candidate = base_edge->Sym()->Onext();

    if (Valid(left_candidate, base_edge))
    {
        while (InCircle(base_edge->destination(), base_edge->origin(),
            left_candidate->destination(), left_candidate->Onext()->destination()))
        {
            Edge* t = left_candidate->Onext();
            Kill(left_candidate);
            left_candidate = t;
        }
    }

    return left_candidate;
}

Edge* Delaunay::RightCandidate(Edge* base_edge)
{
    Edge* right_candidate = base_edge->Oprev();

    if (Valid(right_candidate, base_edge))
    {
        while (InCircle(base_edge->destination(), base_edge->origin(),
            right_candidate->destination(), right_candidate->Oprev()->destination()))
        {
            Edge* t = right_candidate->Oprev();
            Kill(right_candidate);
            right_candidate = t;
        }
    }

    return right_candidate;
}

void Delaunay::MergeHulls(Edge*& base_edge)
{
    while (true)
    {
        Edge* left_candidate = LeftCandidate(base_edge);
        Edge* right_candidate = RightCandidate(base_edge);

        if (!Valid(left_candidate, base_edge) && !Valid(right_candidate, base_edge))
        {
            break;
        }
        else if (!Valid(left_candidate, base_edge)
            || InCircle(left_candidate->destination(), left_candidate->origin(),
                right_candidate->origin(), right_candidate->destination()))
        {
            base_edge = Connect(right_candidate, base_edge->Sym());
        }
        else
        {
            base_edge = Connect(base_edge->Sym(), left_candidate->Sym());
        }
    }
}

EdgePartition Delaunay::Triangulate(const PointsList& points)
{
    if (points.size() == 2)
    {
        return LinePrimitive(points);
    }
    if (points.size() == 3)
    {
        return TrianglePrimitive(points);
    }

    PointsPartition partition = SplitPoints(points);

    EdgePartition left = Triangulate(std::get<0>(partition));
    EdgePartition right = Triangulate(std::get<1>(partition));

    Edge* right_inner = std::get<0>(right)[0];
    Edge* left_inner = std::get<1>(left)[0];

    Edge* left_outer = std::get<0>(left)[0];
    Edge* right_outer = std::get<1>(right)[0];

    Edge* base_edge = LowestCommonTangent(left_inner, right_inner);

    if (left_inner->origin() == left_outer->origin())
    {
        left_outer = base_edge->Sym();
    }
    if (right_inner->origin() == right_outer->origin())
    {
        right_outer = base_edge;
    }

    MergeHulls(base_edge);

    return EdgePartition({ left_outer }, { right_outer });
}

QuadList Delaunay::GetTriangulation()
{
    EdgePartition tuple = Triangulate(vertices_);
    return edges_;
}

EdgeList Delaunay::GetMST()
{
    EdgeList mst;
    PointsList queue;
    std::map<Vert*, int> distance;

    for (auto i = vertices_.begin(); i != vertices_.end(); i++)
    {
        distance[*i] = -1;
    }

    Vert* root = vertices_[0];
    queue.push_back(root);
    distance[root] = 0;

    while (queue.size() > 0)
    {
        Vert* current = queue.back();
        queue.pop_back();

        for (Edge* e = current->edge(); e != current->edge()->Oprev(); e = e->Onext())
        {
            Vert* dest = e->destination();
            if (distance[dest] == -1)
            {
                distance[dest] = distance[current] + 1;
                queue.push_back(dest);
                mst.push_back(e);
            }
        }
    }

    return mst;
}
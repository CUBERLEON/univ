#pragma once

#include <memory>
#include <vector>

class Edge;
class QuadEdge;

class Vert
{
private:
    Edge* edge_;
    float x_;
    float y_;

public:
    Vert(float x, float y);

    Edge* edge() { return edge_; };
    void AddEdge(Edge* edge) { edge_ = edge; };

    int x() { return x_; };
    int y() { return y_; };
    int lengthsquared() { return x_ * x_ + y_ * y_; };
};

Vert::Vert(float x, float y)
    : x_(x) , y_(y)
{}

class Edge
{
private:
    Vert* origin_;
    int index_;
    Edge* next_;

    friend QuadEdge;

public:
    Edge(Vert* _origin);
    Edge();
    ~Edge();

    bool draw;

    int index() { return index_; };

    Edge* Rot();
    Edge* InvRot();
    Edge* Sym();
    Edge* Onext() { return next_; };

    Edge* Oprev() { return Rot()->Onext()->Rot(); };
    Edge* Dnext() { return Sym()->Onext()->Sym(); };
    Edge* Dprev() { return InvRot()->Onext()->InvRot(); };
    Edge* Lnext() { return InvRot()->Onext()->Rot(); };
    Edge* Lprev() { return Onext()->Sym(); };
    Edge* Rnext() { return Rot()->Onext()->InvRot(); };
    Edge* Rprev() { return Sym()->Onext(); };

    Vert* origin() { return origin_; };
    Vert* destination() { return Sym()->origin(); };

    void setNext(Edge* next) { next_ = next; };
    void setIndex(int index) { index_ = index; };
    void setOrigin(Vert* org);
    void setDestination(Vert* dest);

    static Edge* Make(std::vector<QuadEdge*>& list);
};

Edge::Edge()
{}

Edge::~Edge()
{}

Edge::Edge(Vert* _origin)
    : origin_(_origin)
{}

Edge* Edge::Rot() { return (index_ < 3) ? (this + 1) : (this - 3); };

Edge* Edge::InvRot() { return (index_ > 0) ? (this - 1) : (this + 3); }

Edge* Edge::Sym() { return (index_ < 2) ? (this + 2) : (this - 2); }

void Edge::setOrigin(Vert* origin)
{
    origin->AddEdge(this);
    origin_ = origin;
    draw = true;
}

void Edge::setDestination(Vert* dest)
{
    Edge* sym = Sym();
    dest->AddEdge(sym);
    sym->setOrigin(dest);
}

void Splice(Edge* a, Edge* b)
{
    Edge* alpha = a->Onext()->Rot();
    Edge* beta = b->Onext()->Rot();

    Edge* t1 = b->Onext();
    Edge* t2 = a->Onext();
    Edge* t3 = beta->Onext();
    Edge* t4 = alpha->Onext();

    a->setNext(t1);
    b->setNext(t2);
    alpha->setNext(t3);
    beta->setNext(t4);
}
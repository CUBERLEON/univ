#pragma once

#include <SFML\Graphics.hpp>

void DrawEdge(Edge* e, sf::RenderWindow& window, const sf::Color& color)
{
    Edge* sym = e->Sym();
    Vert* destination = e->destination();
    Vert org = *(e->origin());
    Vert dest = *(e->destination());

    sf::VertexArray v(sf::Lines, 2);

    v[0].position = sf::Vector2f(org.x(), org.y());
    v[0].color = color;
    v[1].position = sf::Vector2f(dest.x(), dest.y());
    v[1].color = color;

    window.draw(v);
}

void Render(QuadList& quads, EdgeList& mst)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Euclidean minimal spanning tree");

    std::vector<std::tuple<sf::Vector2f, sf::Vector2f>> quads_debug;
    for (int i = 0; i < quads.size(); i++)
    {
        sf::Vector2f a(quads[i]->edges[0].origin()->x(), quads[i]->edges[0].origin()->y());
        sf::Vector2f b(quads[i]->edges[2].origin()->x(), quads[i]->edges[2].origin()->y());
        quads_debug.push_back(std::make_tuple(a, b));
    }

    while (window.isOpen())
    {
        window.clear(sf::Color::White);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (auto i = quads.begin(); i != quads.end(); ++i)
        {
            if ((*i)->edges[0].draw)
            {
                DrawEdge((*i)->edges, window, sf::Color::Black);
            }
        }

        for (auto i = mst.begin(); i != mst.end(); ++i)
        {
            if ((*i)->draw)
            {
                DrawEdge((*i), window, sf::Color::Green);
            }
        }

        window.display();
    }
}
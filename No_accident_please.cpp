#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

struct Flight {
    vector<Vector2f> points;
    Color color;
};

bool intersect(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4) {
    float det = (p2.x - p1.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p2.y - p1.y);
    if (det == 0) return false;
    float t = ((p3.x - p1.x) * (p4.y - p3.y) - (p3.y - p1.y) * (p4.x - p3.x)) / det;
    float u = -((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)) / det;
    return (t > 0 && t < 1 && u > 0 && u < 1);
}

void adjustPath(Flight& flight, const vector<Flight>& otherFlights) {
    for (size_t i = 0; i < flight.points.size() - 1; ++i) {
        for (const auto& other : otherFlights) {
            for (size_t j = 0; j < other.points.size() - 1; ++j) {
                if (intersect(flight.points[i], flight.points[i+1], other.points[j], other.points[j+1])) {
                    // If intersection found, adjust the middle point
                    if (i == 0) {
                        flight.points[1].x += 0.5f;
                        flight.points[1].y += 0.5f;
                    }
                }
            }
        }
    }
}

void drawFlightPath(RenderWindow& window, const Flight& flight) {
    for (size_t i = 0; i < flight.points.size() - 1; ++i) {
        Vertex line[] = {
            Vertex(flight.points[i], flight.color),
            Vertex(flight.points[i + 1], flight.color)
        };
        window.draw(line, 2, Lines);
    }
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Flight Paths");
    
    vector<Flight> flights = {
        {{{100, 100}, {200, 200}, {300, 300}}, Color::Red},
        {{{100, 100}, {200, 400}, {300, 200}}, Color::Green},
        {{{100, 100}, {400, 200}, {300, 400}}, Color::Blue}
    };

    // Adjust paths to avoid intersections
    for (size_t i = 1; i < flights.size(); ++i) {
        vector<Flight> previousFlights(flights.begin(), flights.begin() + i);
        adjustPath(flights[i], previousFlights);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::White);

        for (const auto& flight : flights) {
            drawFlightPath(window, flight);
        }

        window.display();
    }

    return 0;
}
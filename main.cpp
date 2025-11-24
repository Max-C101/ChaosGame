#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace sf;

class ChaosGame {
private:
    RenderWindow window;
    Font font;
    Text instructions;

    //data structures
    vector<Vector2f> vertices;
    vector<RectangleShape> points;
    Vector2f currentPoint;

  
    bool gameStarted;
    int clickCount;
    int targetVertices; // 3 for Triangle, 4 for Square, 5 for Pentagon
    int previousVertexIndex; // Required for the Square restriction

    void loadResources() {
        if (!font.loadFromFile("arial.ttf")) {
            exit(-1);
        }
        instructions.setFont(font);
        instructions.setCharacterSize(24);
        instructions.setFillColor(Color::White);
        instructions.setPosition(10, 10);
        updateInstructions();
    }

    void updateInstructions() {
        if (!gameStarted) {
            string shape = (targetVertices == 3) ? "Triangle" : (targetVertices == 4) ? "Square" : "Pentagon";
            instructions.setString("Mode: " + shape + "\nClick " + to_string(targetVertices) + " times to set vertices.\n(Press 3, 4, or 5 to change mode)");
        }
        else {
            instructions.setString("Generating Fractal... (Press 3, 4, 5 to reset)");
        }
    }

    //   (finding point between A and B)
    Vector2f lerp(Vector2f start, Vector2f end, float percent) {
        return Vector2f(
            start.x + percent * (end.x - start.x),
            start.y + percent * (end.y - start.y)
        );
    }

public:
    ChaosGame()
        : window(VideoMode(1000, 800), "Chaos Game - Extra Credit"),
        gameStarted(false),
        clickCount(0),
        targetVertices(3), // Default to Triangle
        previousVertexIndex(-1)
    {
        window.setFramerateLimit(60);
        loadResources();
        srand(static_cast<unsigned>(time(0)));
    }

    void reset(int newTarget) {
        targetVertices = newTarget;
        vertices.clear();
        points.clear();
        clickCount = 0;
        gameStarted = false;
        previousVertexIndex = -1;
        updateInstructions();
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    void processEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            //Keyboard input for Extra Credit modes
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Num3) reset(3);
                if (event.key.code == Keyboard::Num4) reset(4);
                if (event.key.code == Keyboard::Num5) reset(5);
            }

            // Mouse handling
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (!gameStarted) {
                    Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    if (clickCount < targetVertices) {
                        vertices.push_back(mousePos);

                        // Draw the vertex as a big red dot
                        RectangleShape r(Vector2f(8, 8));
                        r.setPosition(mousePos);
                        r.setFillColor(Color::Red);
                        points.push_back(r);

                        clickCount++;
                    }
                    else {
                        //The final click starts the game
                        currentPoint = mousePos;
                        gameStarted = true;
                        updateInstructions();
                    }
                }
            }
        }
    }

    void update() {
        if (gameStarted) {
            // Speed up generation (100 points per frame)
            for (int i = 0; i < 100; i++) {
                int index = rand() % vertices.size();

                // EXTRA CREDIT LOGIC:
                // For Square (4 vertices), you cannot pick the same vertex twice in a row.
                if (targetVertices == 4) {
                    while (index == previousVertexIndex) {
                        index = rand() % vertices.size();
                    }
                    previousVertexIndex = index;
                }

                Vector2f target = vertices[index];

                //MATH LOGIC:
                float ratio = 0.5f; // Default for Triangle and Square
                if (targetVertices == 5) {
                    // Pentagon ratio is 1/phi (approx 0.618)
                    ratio = 1.0f / 1.618f;
                }

                // Calculate new position
                currentPoint = lerp(currentPoint, target, ratio);

                // Add pixel
                RectangleShape pixel(Vector2f(1, 1));
                pixel.setPosition(currentPoint);
                // Change color based on mode
                pixel.setFillColor(targetVertices == 3 ? Color::Cyan : (targetVertices == 4 ? Color::Green : Color::Magenta));
                points.push_back(pixel);
            }
        }
    }

    void render() {
        window.clear();
        window.draw(instructions);
        for (const auto& point : points) {
            window.draw(point);
        }
        window.display();
    }
};

int main() {
    ChaosGame game;
    game.run();
    return 0;
}
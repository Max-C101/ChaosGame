#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace sf;

class ChaosGame {
private:
    // Window & Graphics
    RenderWindow window;
    Font font;
    Text instructions;

    // Game Data
    vector<Vector2f> vertices;         // Stores the 3 triangle corners
    vector<RectangleShape> points;     // Stores the generated fractal dots
    Vector2f currentPoint;             // The "jumper" point

  
    bool gameStarted;
    int clickCount;

   
    void loadResources() {
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Error: Could not load arial.ttf" << endl;
            exit(0);
        }
        instructions.setFont(font);
        instructions.setCharacterSize(24);
        instructions.setFillColor(Color::White);
        instructions.setPosition(10, 10);
        instructions.setString("Click 3 times to set vertices.");
    }

    
    void addPoint(Vector2f pos, Color color, float size) {
        RectangleShape rect(Vector2f(size, size));
        rect.setPosition(pos);
        rect.setFillColor(color);
        points.push_back(rect);
    }

public:
    // Initialize window and variables
    ChaosGame(int width, int height)
        : window(VideoMode(width, height), "Chaos Game (CISP 400)"),
        gameStarted(false),
        clickCount(0)
    {
        window.setFramerateLimit(60);
        loadResources();
        srand(time(0));
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

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }

    void handleMouseClick(int x, int y) {
        Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));

        if (!gameStarted) {
            if (clickCount < 3) {
                vertices.push_back(mousePos);
                addPoint(mousePos, Color::Red, 5.0f); // Draw vertex
                clickCount++;

                if (clickCount == 3) {
                    instructions.setString("Click once more to start chaos!");
                }
            }
            else {
                //4th Click: Start
                currentPoint = mousePos;
                gameStarted = true;
                instructions.setString("Generating Fractal...");
            }
        }
    }

    //2. Update Game
    void update() {
        if (gameStarted) {
            // Generate multiple points per frame for speed
            for (int i = 0; i < 50; i++) {
                int randomVertexIndex = rand() % 3;
                Vector2f target = vertices[randomVertexIndex];

                // Midpoint
                float newX = (currentPoint.x + target.x) / 2.0f;
                float newY = (currentPoint.y + target.y) / 2.0f;

                currentPoint = Vector2f(newX, newY);
                addPoint(currentPoint, Color::Cyan, 1.0f);
            }
        }
    }

    // send to console
    void render() {
        window.clear();

       
        window.draw(instructions);

        // Draw points
        for (const auto& point : points) {
            window.draw(point);
        }

        window.display();
    }
};

int main() {
    ChaosGame game(1000, 1000);
    game.run();
    return 0;
}
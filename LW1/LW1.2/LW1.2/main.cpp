#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>


const std::string ERROR_OPENING_FILES{ "Error opening file!" };

struct PairOfNum {
    int value;
    int outputSignal;
};

// Функция для рисования вершины
void drawNode(sf::RenderWindow& window, sf::Vector2f position, const std::string& label, sf::Font& font) {
    sf::CircleShape circle(20.f);
    circle.setPosition(position - sf::Vector2f(20.f, 20.f)); // Позиционирование круга по центру
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2.f);
    window.draw(circle);

    sf::Text text(label, font, 16);
    text.setPosition(position - sf::Vector2f(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f));
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}

// Функция для рисования рёбер
void drawEdge(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, const std::string& label, sf::Font& font) {
    sf::Vertex line[] = {
        sf::Vertex(start),
        sf::Vertex(end)
    };
    line[0].color = sf::Color::Black; // Изменение цвета начальной точки на чёрный
    line[1].color = sf::Color::Black; // Изменение цвета конечной точки на чёрный
    window.draw(line, 2, sf::Lines);

    sf::Vector2f midpoint = (start + end) / 2.f;
    sf::Text text(label, font, 12);
    text.setPosition(midpoint);
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}

int main() {
    std::ifstream fileInput("input.txt");
    if (!fileInput.is_open()) {
        std::cout << ERROR_OPENING_FILES << std::endl;
        return 1;
    }

    int n, m;
    fileInput >> n >> m;
    std::vector<std::vector<PairOfNum>> table(m, std::vector<PairOfNum>(n));
    std::vector<PairOfNum> vecOfTops(n + 1);
    std::string strToRead;
    int intToRead;
    char chToRead;

    fileInput >> strToRead;

    for (int i = 1; i < n + 1; i++) {
        fileInput >> chToRead >> intToRead >> chToRead;
        vecOfTops[i].value = i;
        fileInput >> chToRead >> intToRead;
        vecOfTops[i].outputSignal = intToRead;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fileInput >> intToRead;
            if (intToRead >= 0) {
                table[i][j].value = intToRead;
                table[i][j].outputSignal = vecOfTops[intToRead].outputSignal;
            }
            else {
                table[i][j].value = 0;
                table[i][j].outputSignal = 0;
            }
        }
    }

    // Создание окна
    sf::RenderWindow window(sf::VideoMode(800, 600), "Automaton Visualization");

    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return 1;
    }

    // Рассчитываем позиции вершин
    std::vector<sf::Vector2f> nodePositions(n + 1);
    float radius = 150.f;
    sf::Vector2f center(window.getSize().x / 2.f, window.getSize().y / 2.f);
    for (int i = 1; i <= n; ++i) {
        float angle = (i - 1) * 2 * M_PI / n - M_PI / 2; // Угол поворота вершины
        nodePositions[i] = center + sf::Vector2f(std::cos(angle), std::sin(angle)) * radius;
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

        }

        window.clear(sf::Color::White);

        // Рисуем вершины
        for (int i = 1; i <= n; ++i) {
            std::ostringstream labelStream;
            labelStream << "s" << i << "/y" << vecOfTops[i].outputSignal;
            drawNode(window, nodePositions[i], labelStream.str(), font);
        }

        // Рисуем рёбра
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (table[i][j].value != 0) {
                    std::ostringstream labelStream;
                    labelStream << "y" << table[i][j].outputSignal;
                    drawEdge(window, nodePositions[j + 1], nodePositions[table[i][j].value], labelStream.str(), font);

                }
            }
        }

        window.display();
    }
       
    return 0;
}

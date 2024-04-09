#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

/*
6 2 mur
s0/y1 s0/y2 s1/y1 s1/y2 s2/y2 s3/y1
1 1 2 2 3 1
5 5 5 5 6 4

6 2 mur
s0/y1 s0/y2 s1/y1 s1/y2 s2/y2 s3/y1
- 1 2 2 3 1
5 5 5 5 6 4
*/
/*
4 2 mili
S0/y1 S0/y2 S1/y1 S0/y1
S2/y2 S2/y2 S3/y1 S1/y2

4 2 mili
-/- S0/y2 S1/y1 S0/y1
S2/y2 S2/y2 S3/y1 S1/y2
*/

const std::string ERROR_OPENING_FILES{ "Error opening file!" };

const int RadiusOfTop = 20;

struct PairOfNum {
    int value;
    int outputSignal;
};

void drawNode(sf::RenderWindow& window, sf::Vector2f position, const std::string& label, sf::Font& font) {
    sf::CircleShape circle(20.f);
    circle.setPosition(position - sf::Vector2f(20.f, 20.f));
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2.f);
    window.draw(circle);

    sf::Text text(label, font, 16);
    text.setPosition(position - sf::Vector2f(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f));
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}

void drawEdge(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, const std::string& label, sf::Font& font) {
    // Вычисляем направление линии
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length; // Нормализуем вектор направления

    // Смещаем конечную точку линии на расстояние, равное радиусу вершины
    float nodeRadius = 20.f; // Радиус вершины
    sf::Vector2f offset = direction * nodeRadius;
    end -= offset;

    // Рисуем линию
    sf::Vertex line[] = {
        sf::Vertex(start + offset, sf::Color::Black),
        sf::Vertex(end, sf::Color::Black)
    };
    window.draw(line, 2, sf::Lines);

    // Вычисляем еще две точки для треугольника
    sf::Vector2f perpendicular(-direction.y, direction.x); // Вектор, перпендикулярный направлению линии
    sf::Vector2f trianglePoint1 = end - offset + perpendicular * 10.f;
    sf::Vector2f trianglePoint2 = end - offset - perpendicular * 10.f;

    // Рисуем треугольник
    sf::Vertex triangle[] = {
        sf::Vertex(end, sf::Color::Red),
        sf::Vertex(trianglePoint1, sf::Color::Red),
        sf::Vertex(trianglePoint2, sf::Color::Red)
    };
    window.draw(triangle, 3, sf::Triangles);

    // Рисуем текст
    sf::Vector2f midpoint = (start + end) / 2.f;
    sf::Text text(label, font, 12);
    text.setPosition(midpoint);
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}

bool findNameAvt(const std::string& substring) {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    if (std::getline(file, line)) {
        if (line.find(substring) != std::string::npos) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void drawVertexMili(sf::RenderWindow& window, const sf::Vector2f& position, const std::string& label) {
    sf::CircleShape circle(RadiusOfTop);
    circle.setPosition(position);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setFillColor(sf::Color::Transparent);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    if (!font.loadFromFile("arial.ttf")) {
        std::ofstream logFile("log.txt");
        logFile << "error text";
        logFile.close();
        return;
    }
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Blue);
    text.setPosition(position.x - text.getLocalBounds().width / 2,
        position.y - text.getLocalBounds().height / 2);

    window.draw(circle);
    window.draw(text);
}

void drawEdgeMili(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, int signal, float radius) {
    if (start == end) {
        float offset = radius * 1.0f;

        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, start + sf::Vector2f(offset, offset));
        triangle.setPoint(1, start + sf::Vector2f(offset + radius * 2.0f, offset));
        triangle.setPoint(2, start + sf::Vector2f(offset + radius, offset + radius * 2.0f));
        triangle.setFillColor(sf::Color::Transparent);
        triangle.setOutlineThickness(2.0f);
        triangle.setOutlineColor(sf::Color::Black);
        window.draw(triangle);


        sf::Text signalText;
        sf::Font font;
        font.loadFromFile("arial.ttf");
        if (!font.loadFromFile("arial.ttf")) {
            std::ofstream logFile("log.txt");
            logFile << "error text";
            logFile.close();
            return;
        }
        signalText.setFont(font);
        signalText.setString("Y<" + std::to_string(signal) + ">");
        signalText.setCharacterSize(20);
        signalText.setFillColor(sf::Color::Blue);

        sf::Vector2f signalPosition = start + sf::Vector2f(offset + radius / 2.0f, offset);
        signalText.setPosition(signalPosition.x - signalText.getLocalBounds().width / 2,
            signalPosition.y - signalText.getLocalBounds().height / 2);

        window.draw(signalText);
    }
    else {

        sf::Vector2f direction = end - start;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;

        float nodeRadius = 20.f;
        sf::Vector2f offset = direction * nodeRadius;
        sf::Vector2f endTemp = end - offset;

        sf::Vertex line[] = {
            sf::Vertex(start + offset, sf::Color::Black),
            sf::Vertex(endTemp, sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);

        sf::Vector2f perpendicular(-direction.y, direction.x);
        sf::Vector2f trianglePoint1 = endTemp - offset + perpendicular * 10.f;
        sf::Vector2f trianglePoint2 = endTemp - offset - perpendicular * 10.f;

        sf::Vertex triangle[] = {
            sf::Vertex(endTemp, sf::Color::Red),
            sf::Vertex(trianglePoint1, sf::Color::Red),
            sf::Vertex(trianglePoint2, sf::Color::Red)
        };
        window.draw(triangle, 3, sf::Triangles);

        sf::Text signalText;
        sf::Font font;
        font.loadFromFile("arial.ttf");
        if (!font.loadFromFile("arial.ttf")) {
            std::ofstream logFile("log.txt");
            logFile << "error text";
            logFile.close();
            return;
        }
        signalText.setFont(font);
        signalText.setString("Y<" + std::to_string(signal) + ">");
        signalText.setCharacterSize(15);
        signalText.setFillColor(sf::Color::Blue);

        sf::Vector2f edgeCenter = (start + offset + endTemp) / 2.0f;

        sf::Vector2f signalPosition = edgeCenter + direction * radius;

        signalText.setPosition(signalPosition.x - signalText.getLocalBounds().width / 2,
            signalPosition.y - signalText.getLocalBounds().height / 2);

        window.draw(signalText);
    }
}

void drawGraphMili(sf::RenderWindow& window, const std::vector<std::tuple<int, std::string>>& vecTopsForDraw, const std::vector<std::tuple<int, int, int>>& vecEdgesForDraw) {
    int numVertices = vecTopsForDraw.size() - 2;
    sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    float radius = 200.0f;


    for (size_t i = 1; i < vecTopsForDraw.size() - 1; ++i) {
        float angle = (i - 1) * 2 * M_PI / numVertices;
        sf::Vector2f position = center + sf::Vector2f(std::cos(angle), std::sin(angle)) * radius;
        drawVertexMili(window, position, std::get<1>(vecTopsForDraw[i]));
    }


    for (const auto& edge : vecEdgesForDraw) {
        int startIndex = std::get<0>(edge) - 1;
        int endIndex = std::get<1>(edge) - 1;
        int signal = std::get<2>(edge);

        float startAngle = startIndex * 2 * M_PI / numVertices;
        float endAngle = endIndex * 2 * M_PI / numVertices;

        sf::Vector2f startPos = center + sf::Vector2f(std::cos(startAngle), std::sin(startAngle)) * radius;
        sf::Vector2f endPos = center + sf::Vector2f(std::cos(endAngle), std::sin(endAngle)) * radius;

        drawEdgeMili(window, startPos, endPos, signal, RadiusOfTop);
    }
}

int main()
{
    std::ifstream fileInput("input.txt");
    if (!fileInput.is_open()) 
    {
         std::ofstream logFile("log.txt");
         logFile << ERROR_OPENING_FILES;
         logFile.close();
         return 1;
    }

    bool mili = findNameAvt("mili");
    bool mur = findNameAvt("mur");

    if (mur)
    {
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
                fileInput >> chToRead;
                if (chToRead == '-') {
                    table[i][j].value = 0;
                    table[i][j].outputSignal = 0;
                }
                else {
                    fileInput.putback(chToRead);
                    fileInput >> intToRead;
                    table[i][j].value = intToRead;
                    table[i][j].outputSignal = vecOfTops[intToRead].outputSignal;
                }
            }
        }

        // Открываем файл для записи
        std::ofstream fileOutput("output.txt");
        if (!fileOutput.is_open()) {
            std::cout << ERROR_OPENING_FILES << std::endl;
            return 1;
        }

        // Записываем вершины в файл
        for (int i = 1; i <= n; ++i) {
            fileOutput << i << " ";
        }
        fileOutput << std::endl;

        // Записываем ребра в файл
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (table[i][j].value != 0) {
                    fileOutput << table[i][j].value << "/y" << table[i][j].outputSignal << " ";
                }
                else {
                    fileOutput << "- ";
                }
            }
            fileOutput << std::endl;
        }


        sf::RenderWindow window(sf::VideoMode(800, 600), "Automaton Visualization");


        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return 1;
        }


        std::vector<sf::Vector2f> nodePositions(n + 1);
        float radius = 150.f;
        sf::Vector2f center(window.getSize().x / 2.f, window.getSize().y / 2.f);
        for (int i = 1; i <= n; ++i) {
            float angle = (i - 1) * 2 * M_PI / n - M_PI / 2;
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


            for (int i = 1; i <= n; ++i) {
                std::ostringstream labelStream;
                labelStream << "s" << i;
                drawNode(window, nodePositions[i], labelStream.str(), font);
            }

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
    }

    if (mili)
    {
        std::ifstream inputFile("input.txt");
        if (!inputFile.is_open())
        {
            std::cout << "err";
            return 1;
        }


        int numOfColumns;
        int numOfStr;
        std::string typeOfAut;

        inputFile >> numOfColumns >> numOfStr >> typeOfAut;

        std::vector<std::tuple<int, int>> vecOfTops;

        std::vector<std::vector< std::tuple<int, int, int>>> vecOfRules(numOfStr);
        for (auto& innerVec : vecOfRules) {
            innerVec.resize(numOfColumns);
        }

        char tempCh;
        int tempStage;
        int tempSygnal;


        for (int i = 0; i < numOfStr; i++)
        {
            for (int j = 0; j < numOfColumns; j++)
            {
                inputFile >> tempCh;
                if (tempCh != '-')
                {
                    inputFile >> tempStage >> tempCh >> tempCh >> tempSygnal;
                    vecOfRules[i][j] = std::make_tuple(tempStage, tempSygnal, j);

                    std::tuple<int, int> newTuple = std::make_tuple(tempStage, tempSygnal);
                    auto it = std::find_if(vecOfTops.begin(), vecOfTops.end(), [&newTuple](const std::tuple<int, int>& tuple) {
                        return tuple == newTuple;
                        });
                    if (it == vecOfTops.end()) {
                        vecOfTops.push_back(newTuple);
                    }
                }
                else
                {
                    inputFile >> tempCh >> tempCh;
                    vecOfRules[i][j] = std::make_tuple(-1, -1, -1);
                }
            }
            std::cout << std::endl;
        }

        std::ofstream outputFile("res.txt");

        if (!outputFile.is_open())
        {
            std::cout << "err";
            return 1;
        }

        for (const auto& tuple : vecOfTops)
        {
            outputFile << std::get<0>(tuple) << "/" << std::get<1>(tuple) << " ";
        }
        outputFile << std::endl;

        for (int i = 0; i < numOfStr; i++)
        {
            for (const auto& top : vecOfTops)
            {
                bool founded = false;
                for (int j = 0; j < vecOfRules[i].size(); j++)
                {
                    if (std::get<2>(vecOfRules[i][j]) == std::get<0>(top))
                    {
                        founded = true;

                        for (int e = 0; e < vecOfTops.size(); e++)
                        {
                            if ((std::get<0>(vecOfTops[e]) == std::get<0>(vecOfRules[i][j])) && (std::get<1>(vecOfTops[e]) == std::get<1>(vecOfRules[i][j])))
                            {
                                int msg = e + 1;
                                outputFile << msg << " ";
                            }
                        }

                        break;
                    }
                }
                if (!founded)
                {
                    outputFile << "- ";
                }
            }


            outputFile << std::endl;
        }
        outputFile.close();

        std::vector<std::tuple<int, std::string>> vecTopsForDraw(vecOfTops.size() + 2, std::make_tuple(0, ""));

        std::vector<std::tuple<int, int, int>> vecEdgesForDraw;
        std::ifstream fileRes("res.txt");
        std::ofstream fileLog("log.txt");

        std::string strToRead;
        for (int i = 1; i < vecOfTops.size() + 1; i++)
        {
            fileRes >> strToRead;
            std::get<0>(vecTopsForDraw[i]) = i;
            std::get<1>(vecTopsForDraw[i]) = strToRead;
        }

        int tempNum;
        char tempChar;
        for (int i = 1; i <= numOfStr; i++)
        {
            for (int j = 1; j <= vecOfTops.size(); j++)
            {
                fileRes >> tempChar;
                if (tempChar != '-')
                {
                    tempNum = tempChar - '0';
                    vecEdgesForDraw.push_back(std::make_tuple(j, tempNum, i));
                }
            }
        }

        sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Drawing");

        window.clear(sf::Color::White);

        drawGraphMili(window, vecTopsForDraw, vecEdgesForDraw);

        window.display();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }

    return 0;
}
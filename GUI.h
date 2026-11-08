#pragma once
#include <SFML/Graphics.hpp>
#include "trie.h"
#include "TernarySearchTree.h"
#include <Vector>
#include <string>

class GUI{
    private:
        sf::RenderWindow window;
        sf::Font font;
        std::string inputText;
        bool inputSelected = false;
        bool trieSelected = true;
        void drawRectangle(float x, float y, float width, float hight, sf::Color fill, sf::Color outline, float thickness = 0.f);
        void drawText(const std::string& text, unsigned int size, float x, float y, sf::Color color);

        void drawInterface();

        trie trieTree;
        TernarySearchTree tstTree;

        std::vector<std::string> suggestions;
        void updateSuggestions();

        double trieTime = 0.0;
        double tstTime = 0.0;
        double executionTime = 0.0;
        bool comparisonReady = false;
        void comparePerformance();
    public:
        GUI();
        void run();
};
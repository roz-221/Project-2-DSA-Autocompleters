#include "GUI.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "dictionary.h"

GUI::GUI() : window(sf::VideoMode({1000, 700}), "Autocomplete for English Words"){
    window.setFramerateLimit(60);
    if(!font.openFromFile("resources/fonts/PressStart2P-Regular.ttf")){
        throw std::runtime_error("Failed to load Font");
    }

    std::vector<std::string> words = dict::loadFrequencyCsv("data/valid_words_sorted_by_frequency.csv");
    if(words.empty()){
        throw std::runtime_error("Fail to load dictionary");
    }
    trieTree.build(words);
    tstTree.build(words);
    updateSuggestions();
}

void GUI::drawRectangle(float x, float y, float width, float height, sf::Color fill, sf::Color outline, float thickness){
    sf::RectangleShape rectangle({width,height});
    rectangle.setPosition({x,y});
    rectangle.setFillColor(fill);
    rectangle.setOutlineColor(outline);
    rectangle.setOutlineThickness(thickness);
    window.draw(rectangle);
}



void GUI::drawText(const std::string& text, unsigned int size,float x, float y, sf::Color color){
    sf::Text label(font, text,size);
    label.setPosition({x,y});
    label.setFillColor(color);
    window.draw(label);
}

void GUI::updateSuggestions(){
    suggestions.clear();
    std::string prefix = inputText;
    for(char& c: prefix){
        if(c >= 'A' && c <= 'Z'){
            c = c- 'A' + 'a';
        }
    }
    //start time for execution time
    auto start = std::chrono::steady_clock::now();
    if(trieSelected){
        suggestions = trieTree.suggest(prefix);
    } else{
        suggestions = tstTree.suggest(prefix);
    }
    //end time for execution time
    auto end = std::chrono::steady_clock::now();

    //convert to microseconds
    executionTime = std::chrono::duration<double,std::micro>( end-start ).count();
}

void GUI::comparePerformance(){
    using Clock = std::chrono::steady_clock;
    std::string prefix = inputText;

    for(char& c : prefix){
        if( c >='A' && c <= 'Z'){
            c = c- 'A' + 'a';
        }
    }

    //measure trie time
    auto start = Clock::now();
    auto trieResult = trieTree.suggest(prefix);
    auto end= Clock::now();
    trieTime = std::chrono::duration<double, std::micro>(end-start).count();

    //measure tst time
    start = Clock::now();
    auto tstResults = tstTree.suggest(prefix);
    end = Clock::now();
    tstTime = std::chrono::duration<double, std::micro>(end-start).count();

    comparisonReady = true;
}

void GUI::drawInterface(){
    const sf::Color blue(70, 95, 245);
    const sf::Color purple(90, 35,210);
    const sf::Color red(240, 35, 45);
    const sf::Color black(10, 10, 35);
    window.clear(blue);

    //draw title
    drawText("AUTOCOMPLETE FOR ENGLISH WORDS", 20, 85, 30,black);

    //title for total words loaded
    drawText("TOTAL WORDS LOADED: " + std::to_string(trieTree.wordCount()), 10, 330, 95, black);

    //user input box
    drawText("Word:", 12,50,135, black);
    //when clicking on box change its color to show it is selected
    sf::Color inputBorder = inputSelected ? sf::Color::White : purple;
    drawRectangle(50, 165, 260, 45, blue, inputBorder,3);
    //display typed text
    drawText(inputText, 12, 60, 180, black);
    //show cursor
    if(inputSelected){
        drawText("_",12, 60 + inputText.length() * 12, 180, black);
    }

    //create boxes for trie and ternary search
    sf::Color trieColor = trieSelected ? sf::Color(40, 200, 100): red;
    sf::Color tstColor = !trieSelected ? sf::Color(10, 200, 100): red;

    drawRectangle(50, 235, 260, 48, trieColor, black);
    drawText("TRIE",14, 145, 251, black);
    drawRectangle(50, 300, 260, 48, tstColor ,black);
    drawText("TERNARY SEARCH", 10, 65, 318, black);

    //performance comparison button
    drawRectangle(50,365, 260, 48, red ,black);
    drawText("COMPARE PERFORMANCE", 9,63,384, black);

    //autocomplete output box
    drawText("TOP 10 AUTOCOMPLETE WORDS", 12, 610, 125,black);
    for(int i=0; i < 10; i++){
        float y = 160 + i*38;
        drawRectangle(610,160 +i * 38, 340, 32, blue,purple, 3);
        if(i< static_cast<int>(suggestions.size())){
            drawText(suggestions[i], 9, 620, y+9, black);
        }
    }

    //Peformance exectution time table
    drawText("PERFORMANCE COMPARISON", 11, 75, 460, black);
    const float table_X = 50.f;
    const float table_Y = 495.f;
    const float rowHeight = 42.f;
    const float colWidth[] = {260.f, 140.f, 140.f};
    
    for(int row =0; row < 4; row++){
        float x = table_X;
        for(float width : colWidth){
            drawRectangle(x, table_Y + row * rowHeight, width, rowHeight, blue, purple, 2);
            x+=width;
        }
    }

    drawText("METRIC", 9, 65, 510,black);
    drawText("TRIE", 9, 335, 510, black);
    drawText("TST", 9, 475, 510, black);
    drawText("Autocomplete Time (us)", 8, 60, 552, black);
    drawText("Node Count", 9, 60,594, black);
    drawText("Estimated Space (KiB)", 8, 60, 636, black);

    //display time comparison results
    if(comparisonReady){
        std::ostringstream trieTimeText;
        std::ostringstream tstTimeText;
        trieTimeText << std::fixed << std::setprecision(2)<< trieTime;
        tstTimeText << std::fixed <<std::setprecision(2) << tstTime;

        //row 1 - execution time
        drawText(trieTimeText.str(),8 ,335, 552,black);
        drawText(tstTimeText.str(),8, 475, 552, black);
        //row 2 - node count
        drawText(std::to_string(trieTree.nodeCount()), 8, 335,594, black);
        drawText(std::to_string(tstTree.nodeCount()), 8, 475,594,black);
        //row 3- memory usage
        drawText(std::to_string(trieTree.memoryCost() / 1024), 8, 335, 636,black);
        drawText(std::to_string(tstTree.memoryCost() / 1024), 8,475,636, black);
    }

    std::ostringstream timeText;
    timeText <<std::fixed<< std::setprecision(2) <<executionTime;
    //lable for execution time for current run
    drawText("Execution Time: " +timeText.str()+ " us", 9,660,645,black);
}

void GUI::run(){
    while(window.isOpen()){
        while(const auto event = window.pollEvent()){
            if(event -> is<sf::Event::Closed>()){
                window.close();
            }
            //detect a mouse Click
            if(const auto* mouse = event -> getIf<sf::Event::MouseButtonPressed>()){
                //check left click
                if (mouse -> button == sf::Mouse::Button::Left){
                    //get position of mouse
                    float mouse_X = static_cast<float>(mouse -> position.x);
                    float mouse_Y = static_cast<float>(mouse -> position.y);
                    //check if position of click is within the box
                    inputSelected = mouse_X >= 50 && mouse_X <= 310 && mouse_Y >= 165 && mouse_Y <= 210;
                    //check if trie buton was clicked
                    if(mouse_X >= 50 && mouse_X <= 310 && mouse_Y >= 235 && mouse_Y <= 283){
                        trieSelected = true;
                        updateSuggestions();
                        std::cout << "Trie selected\n";
                    }
                    //check if TST button was clicked
                    if (mouse_X >= 50 && mouse_X <= 310 && mouse_Y >= 300 && mouse_Y <= 348){
                        trieSelected = false;
                        updateSuggestions();
                        std::cout << "TST selected\n";
                    }
                    //check if compare button was clicked
                    if(mouse_X >= 50&& mouse_X <= 310 && mouse_Y >= 365 && mouse_Y <= 413){
                        comparePerformance();
                        std::cout<< "Performances Compared\n";
                    }
                }
            }
            //check keyboard input
            if(const auto* text = event ->getIf<sf::Event::TextEntered>()){
                if(inputSelected){
                    char32_t character = text -> unicode;
                    if(character == 8) {
                        if(!inputText.empty()){
                            inputText.pop_back();
                            updateSuggestions();
                        }
                    //make it so that it only accepts english letters
                    } else if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z' )){
                        //place a limit on input
                        if(inputText.length() < 15) {
                            inputText += static_cast<char>(character);
                            updateSuggestions();
                        }
                    }

                }
            }
        }
    
        drawInterface();
        window.display();
    }
}
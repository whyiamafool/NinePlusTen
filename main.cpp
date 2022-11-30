#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"

/*class Deck {
    public:
        char** DrawRandom();
    private:
        char deck[52][3] = {"1S", "2S"};;
};

Deck::Deck() {
    //
}

char** Deck::DrawRandom() {
    char *test[] = {"1S", "2S"};

    return test;
}*/

class Deck {
    public:
        Deck();
        char* DrawRandom();
    private:
        char deck[52][4] = {
            "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
            "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
            "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
            "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD",
        };
};

Deck::Deck() {
    //nothing here whoo
}

char* Deck::DrawRandom() {
    int randIndex = rand() % 51;

    return deck[randIndex];
}

void DrawMenu(FEHIcon::Icon *top, FEHIcon::Icon *bottom) {
    FEHImage titlescreen;

    titlescreen.Open("Title ScreenFEH.pic");
    titlescreen.Draw(34, 65);
    titlescreen.Close();

    char top_menu[2][20] = {"SINGLE", "MULTI"};
    char bottom_menu[4][20] = {"STATS", "RULES", "THEME", "CREDS"};
    FEHIcon::DrawIconArray(top, 1, 2, 130, 55, 50, 50, top_menu, RED, RED);
    FEHIcon::DrawIconArray(bottom, 1, 4, 200, 10, 10, 10, bottom_menu, RED, RED);
    
    LCD.Update();
}

void DrawBack() {
    FEHImage backimg;
    
    backimg.Open("BackFEH.pic");
    backimg.Draw(7, 7);
    backimg.Close();

    LCD.Update();
}

void DrawRules() {
    FEHImage rulesimg;

    rulesimg.Open("InstructionsFEH.pic");
    rulesimg.Draw(0, 30);
    rulesimg.Close();

    LCD.Update();
}

void DrawBoard() {
    LCD.SetFontColor(DARKGREEN);
    LCD.FillRectangle(0, 0, 320, 240);
    LCD.SetFontColor(FORESTGREEN);
    LCD.FillRectangle(5, 5, 310, 230);
}

/* Entry point to the application */
int main() {

    float x, y, xtrash, ytrash;
    int menuState = 0;
    FEHIcon::Icon top[2], bottom[4], back[1];
    DrawMenu(top, bottom);

    srand(time(NULL));
    Deck deck;

    while (1) {
        LCD.ClearBuffer();

        while (!LCD.Touch(&xtrash, &ytrash)) { }
        while (LCD.Touch(&x, &y)) { }

        if (menuState == 0) { // 0 represents the main menu
            if (top[0].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBoard();
                menuState = 2; // 2 represents game is in progress
            
                // single player game goes here bruh
                LCD.WriteAt("game", 87, 111);
            } else if (top[1].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBoard();
                menuState = 2;
                
                //LCD.WriteAt("game", 87, 111);
            } else if (bottom[0].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack();
                menuState = 1;

                LCD.DrawRectangle(75, 5, 240, 35);
                LCD.WriteRC("STATISTICS", 1, 11);
                LCD.WriteRC("Wins: 5", 6, 9);
                LCD.WriteRC("Losses: 12", 7, 8);
            } else if (bottom[1].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack();
                menuState = 1;

                // rules page
                DrawRules();
            } else if (bottom[2].Pressed(x, y, 1 )){
                LCD.Clear();
                DrawBack();
                menuState = 1;
                // theme select
                LCD.WriteRC("Restricted Area Please Leave", 6, 9);

                LCD.WriteRC(deck.DrawRandom(), 4, 5);
            } else if (bottom[3].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack();
                menuState = 1;

                LCD.WriteRC("Written By:", 5, 8);
                LCD.WriteRC("Aditya Chittari", 7, 6);
                LCD.WriteRC("Aaryan Makavana", 8, 6);
            }
        } else if (menuState == 1) { // any of the sub-menus / games
            if (7 < x && x < (7 + 42) && 7 < y && y < (7 + 14)) {
                LCD.Clear();
                DrawMenu(top, bottom);
                menuState = 0;
            }
        }
        
        LCD.Update();

        Sleep(0.125);
    }

    return 0;
}
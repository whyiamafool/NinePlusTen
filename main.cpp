#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "FEHRandom.h"
#include <iostream>

#define CARDWIDTH 40
#define CARDHEIGHT 50

#define OCEANBORDER 0x076F94
#define SAND 0xF6BB99
#define CORAL 0xF55F51

#define BUCKEYERED 0xBB0000
#define BUCKEYEGRAY 0x666666

class Deck {
    public:
        Deck();
        char* DrawRandomCard();
        void printDeck();
    private:
        char deck[52][5] = {
            "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
            "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
            "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
            "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD"
        };
        char dealtcards[52][5];
        int dealtcardindex;
};

/*void Deck::printDeck() {
    for (int i = 0; i < 52; i++) {
        std::cout << i << ": " << deck[i] << std::endl;
    }
}*/

Deck::Deck() {
    dealtcardindex = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(dealtcards[i], "");
    }
}

char* Deck::DrawRandomCard() { // issues a unique random card. crashes when all 52 cards have been dealt. tech no need to fix
    int randIndex = Random.RandInt() % 51;

    std::cout << "rI(og): " << randIndex << ", DRCoutput: " << deck[randIndex] <<"\n";

    for (int i = 0; i < dealtcardindex; i++) {
		while (strcmp(dealtcards[i], deck[randIndex]) == 0) {
			randIndex = Random.RandInt() % 51;
			i = 0;
		}
   	}

	strcpy(dealtcards[dealtcardindex], deck[randIndex]);
	dealtcardindex++;

    std::cout << "rI(ed): " << randIndex << ", DRCoutput: " << deck[randIndex] <<"\n";

    return deck[randIndex];
}

class Hand {
    public: 
        Hand(int player);
        void Hit(Deck *deckptr);
        void DrawHand(int theme);
    private:
        int playerNo;
        FEHIcon::Icon handIconArray[11];
        char cardsInHand[11][20];
        int noOfCards;
};

Hand::Hand(int player) {
    playerNo = player;
    noOfCards = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(cardsInHand[i], "");
    }
}

void Hand::Hit(Deck *deckptr) {
    strcpy(cardsInHand[noOfCards], deckptr -> DrawRandomCard());
    for (int i = 0; i <= noOfCards; i++) {
        //std::cout << "HitR: " << cardsInHand[i]<<"\n"<<"***"<<"\n";
    }
    noOfCards++;
}

void Hand::DrawHand(int theme) {
    if (playerNo == 1) {
        int widthmargin = (320 - (noOfCards * CARDWIDTH))/2;
        int heightmargin = (240 - 5) - CARDHEIGHT;

        if (theme == 0) {
            LCD.SetFontColor(LIGHTGRAY); //0x076F94
            LCD.FillRectangle(widthmargin, heightmargin, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, heightmargin, 5, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        } else if (theme == 1) {
            LCD.SetFontColor(SAND); //0x076F94
            LCD.FillRectangle(widthmargin, heightmargin, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, heightmargin, 5, widthmargin, widthmargin, cardsInHand, CORAL, CORAL);
        } else if (theme == 2) {
            LCD.SetFontColor(SNOW); //0x076F94
            LCD.FillRectangle(widthmargin, heightmargin, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, heightmargin, 5, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        }

        /*LCD.SetFontColor(SAND); //0x076F94
        LCD.FillRectangle(widthmargin, heightmargin, noOfCards * CARDWIDTH, CARDHEIGHT);
        FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, heightmargin, 5, widthmargin, widthmargin, cardsInHand, CORAL, CORAL);*/
        
    } else if (playerNo == 2) {
        int widthmargin = (320 - (noOfCards * CARDWIDTH))/2;
        int heightmargin = (240 - 5) - CARDHEIGHT;

        if (theme == 0) {
            LCD.SetFontColor(LIGHTGRAY); //0x076F94
            LCD.FillRectangle(widthmargin, 5, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, 5, heightmargin, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        } else if (theme == 1) {
            LCD.SetFontColor(SAND); //0x076F94
            LCD.FillRectangle(widthmargin, 5, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, 5, heightmargin, widthmargin, widthmargin, cardsInHand, CORAL, CORAL);
        } else if (theme == 2) {
            LCD.SetFontColor(SNOW); //0x076F94
            LCD.FillRectangle(widthmargin, 5, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, 5, heightmargin, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        }

        /*LCD.SetFontColor(SAND); //0x076F94
        LCD.FillRectangle(widthmargin, 5, noOfCards * CARDWIDTH, CARDHEIGHT);
        FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, 5, heightmargin, widthmargin, widthmargin, cardsInHand, CORAL, CORAL);*/
    }
}

// TODO: function prototypes

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

void DrawThemeSelect(int theme) {
    FEHImage themeimg;

    themeimg.Open("ThemeSelectFEH.pic");
    themeimg.Draw(0, 40);
    themeimg.Close();

    LCD.SetFontColor(RED);
    LCD.WriteAt("Current Theme:", 77, 30);

    if (theme == 0) {
        LCD.SetFontColor(FORESTGREEN);
        LCD.WriteAt("Casino", 125, 50);
    } else if (theme == 1) {
        LCD.SetFontColor(DARKCYAN);
        LCD.WriteAt("Ocean", 130, 50);
    } else if (theme == 2) {
        LCD.SetFontColor(BUCKEYEGRAY);
        LCD.WriteAt("Buckeyes", 113, 50);
    }

    LCD.Update();
}

void DrawBoard(int theme) {
    /*FEHImage casinoboard;

    casinoboard.Open("CasinoBoardFEH.pic");
    casinoboard.Draw(0, 0);
    casinoboard.Close();

    LCD.Update();*/

    if (theme == 0) {
        LCD.SetFontColor(DARKGREEN);
        LCD.FillRectangle(0, 0, 320, 240);
        LCD.SetFontColor(FORESTGREEN);
        LCD.FillRectangle(5, 5, 310, 230);
    } else if (theme == 1) {
        LCD.SetFontColor(OCEANBORDER);
        LCD.FillRectangle(0, 0, 320, 240);
        LCD.SetFontColor(DARKCYAN);
        LCD.FillRectangle(5, 5, 310, 230);
    } else if (theme == 2) {
        LCD.SetFontColor(BUCKEYERED);
        LCD.FillRectangle(0, 0, 320, 240);
        LCD.SetFontColor(BUCKEYEGRAY);
        LCD.FillRectangle(5, 5, 310, 230);}

}

void DrawHitStand(FEHIcon::Icon *hit, FEHIcon::Icon *stand, int theme) {

    char hitIcon[1][20] = {"H"};
    char standIcon[1][20] = {"S"};

    if (theme == 0) { //casino theme
        LCD.SetFontColor(LIGHTGRAY);
        LCD.FillRectangle(20, 100, 40, 40);
        LCD.FillRectangle(260, 100, 40, 40);
        FEHIcon::DrawIconArray(hit, 1, 1, 100, 100, 20, 260, hitIcon, BLACK, BLACK);
        FEHIcon::DrawIconArray(stand, 1, 1, 100, 100, 260, 20, standIcon, BLACK, BLACK);
    } else if (theme == 1) { //ocean theme
        LCD.SetFontColor(SAND);
        LCD.FillRectangle(20, 100, 40, 40);
        LCD.FillRectangle(260, 100, 40, 40);
        FEHIcon::DrawIconArray(hit, 1, 1, 100, 100, 20, 260, hitIcon, CORAL, CORAL);
        FEHIcon::DrawIconArray(stand, 1, 1, 100, 100, 260, 20, standIcon, CORAL, CORAL);
    } else if (theme == 2) { //buckeye theme
        LCD.SetFontColor(SNOW);
        LCD.FillRectangle(20, 100, 40, 40);
        LCD.FillRectangle(260, 100, 40, 40);
        FEHIcon::DrawIconArray(hit, 1, 1, 100, 100, 20, 260, hitIcon, BLACK, BLACK);
        FEHIcon::DrawIconArray(stand, 1, 1, 100, 100, 260, 20, standIcon, BLACK, BLACK);
    }
}

/* Entry point to the application */
int main() {

    float x, y, xtrash, ytrash;
    int menuState = 0;
    int turn = 0;
    int theme = 0;
    FEHIcon::Icon top[2], bottom[4], back[1];
    FEHIcon::Icon hit[1], stand[1];

    DrawMenu(top, bottom);

    Deck deck;
    //Deck *deckptr;
    //deckptr = &deck;

    Hand player1(1), player2(2);

    while (1) {
        LCD.ClearBuffer();

        while (!LCD.Touch(&xtrash, &ytrash)) { }
        while (LCD.Touch(&x, &y)) { }

        //LCD.Clear(BLACK);

        if (menuState == 0) { // 0 represents the main menu
            if (top[0].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBoard(theme);
                menuState = 3; // 3 represents game is in progress
                DrawHitStand(hit, stand, theme);

                // GAME CODE
                player1.Hit(&deck);
                player1.Hit(&deck);
                player1.Hit(&deck);
                player1.Hit(&deck);
                player1.DrawHand(theme);

                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.DrawHand(theme);

            } else if (top[1].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBoard(1);
                menuState = 3;
                DrawHitStand(hit, stand, 1);

                // GAME CODE
                while (turn == 0){
                    while (!LCD.Touch(&xtrash, &ytrash)) { }
                    while (LCD.Touch(&x, &y)) { }
                    LCD.WriteRC("PLAYER 1", 6, 9);
                    if (hit[0].Pressed(x, y, 1)) {
                        player1.Hit(&deck);
                        player1.DrawHand(theme);
                        turn++;
                    } else if (stand[0].Pressed(x, y, 1)){
                        turn++;
                    }
                    LCD.Update();
                }
                while (turn == 1){
                    LCD.WriteRC("PLAYER 2", 6, 9);
                    if (hit[0].Pressed(x, y, 1)) {
                    player2.Hit(&deck);
                    player2.DrawHand(theme);
                    turn--;
                    } else if (stand[0].Pressed(x, y, 1)){
                    turn--;
                    }
                }

                
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
            } else if (bottom[2].Pressed(x, y, 1 )){ // theme select
                LCD.Clear();
                DrawBack();
                menuState = 2; // 2 represents user is selecting a theme

                DrawThemeSelect(theme);
            } else if (bottom[3].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack();
                menuState = 1;

                LCD.WriteRC("Written By:", 5, 8);
                LCD.WriteRC("Aditya Chittari", 7, 6);
                LCD.WriteRC("Aaryan Makavana", 8, 6);
            }
        } else if (menuState == 1 || menuState == 2) { // any of the sub-menus / games
            if (7 <= x && x < (7 + 42) && 7 <= y && y < (7 + 14)) {
                LCD.Clear();
                DrawMenu(top, bottom);
                menuState = 0;
            }

            if (menuState == 2) {

                if (52 <= x && x < 270 && 78 <= y && y < 123) {
                    theme = 0;
                    DrawThemeSelect(theme);
                } else if (52 <= x && x < 270 && 127 <= y && y < 172) {
                    theme = 1;
                    DrawThemeSelect(theme);
                } else if (52 <= x && x < 270 && 176 <= y && y < 221) {
                    theme = 2;
                    DrawThemeSelect(theme);
                }
            }
        }

        //LCD.WriteAt(x, 100, 100);
        //LCD.WriteAt(y, 100, 120);
        
        LCD.Update();

        Sleep(0.125);
    }

    return 0;
}
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "FEHRandom.h"
#include <iostream>
#include <string>

#define CARDWIDTH 42
#define CARDHEIGHT 50

#define OCEANBORDER 0x076F94
#define SAND 0xF6BB99
#define CORAL 0xF55F51

#define BUCKEYERED 0xBB0000
#define BUCKEYEGRAY 0x666666
#define BUCKEYECARDGRAY 0xCBCBD1

#define CPUTime 0.5

class Deck {
    public:
        Deck();
        char* DrawRandomCard();
        void resetDeck();
        char deck[52][20] = {
            "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
            "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
            "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
            "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD"
        };
    private:
        char dealtcards[52][20];
        int dealtcardindex;
};

Deck::Deck() {
    dealtcardindex = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(dealtcards[i], "");
    }
}

char* Deck::DrawRandomCard() { // issues a unique random card. crashes when all 52 cards have been dealt. tech no need to fix
    int randIndex = Random.RandInt() % 51;

    for (int i = 0; i < dealtcardindex; i++) {
		while (strcmp(dealtcards[i], deck[randIndex]) == 0) {
			randIndex = Random.RandInt() % 51;
			i = 0;
		}
   	}

	strcpy(dealtcards[dealtcardindex], deck[randIndex]);
	dealtcardindex++;

    return deck[randIndex];
}

void Deck::resetDeck() {
    dealtcardindex = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(dealtcards[i], "");
    }
}


class Hand {
    public: 
        Hand(int player);
        void Hit(Deck *deckptr);
        void DrawHand(int theme);
        int getNoOfCards();
        int getHandValue();
        int getAces(); //debugger
        void resetHand(int player);
        int CPUAIDecision(int theme, Hand p1);
    private:
        int playerNo;
        FEHIcon::Icon handIconArray[11];
        char cardsInHand[11][20];
        int noOfCards;
        int handValue;
        int noOfAces;
        int aceDifferential;
        float yoloCoefficient;
};

Hand::Hand(int player) {
    playerNo = player;
    noOfCards = 0;
    handValue = 0;
    noOfAces = 0;
    aceDifferential = 0;
    yoloCoefficient = (Random.RandInt() % 100) / 100.0;
    for (int i = 0; i < 52; i++) {
        strcpy(cardsInHand[i], "");
    }
}

void Hand::Hit(Deck *deckptr) {
    strcpy(cardsInHand[noOfCards], deckptr -> DrawRandomCard());
    
    if (cardsInHand[noOfCards][0] == 'A') {
        noOfAces++;
    }

    noOfCards++;
}

int Hand::getNoOfCards() {
    return noOfCards;
}

int Hand::getHandValue() {
    handValue = 0;
    aceDifferential = 0;

    for (int i = 0; i < noOfCards; i++) {
        if (cardsInHand[i][0] == 'K' || cardsInHand[i][0] == 'Q' || cardsInHand[i][0] == 'J' || cardsInHand[i][0] == '1') {
            handValue += 10;
        } else if (cardsInHand[i][0] == 'A') {
            handValue += 11;
        } else {
            handValue += (cardsInHand[i][0] - '0');
        }
    }

    while (handValue > 21 && noOfAces > aceDifferential) {
        handValue -= 10;
        aceDifferential++;
    }

    return handValue;
}

int Hand::getAces() {
    return noOfAces;
}

void Hand::resetHand(int player) {
    playerNo = player;
    noOfCards = 0;
    handValue = 0;
    noOfAces = 0;
    aceDifferential = 0;
    yoloCoefficient = (Random.RandInt() % 100) / 100.0;
    for (int i = 0; i < 52; i++) {
        strcpy(cardsInHand[i], "");
    }
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
            LCD.SetFontColor(BUCKEYECARDGRAY); //0x076F94
            LCD.FillRectangle(widthmargin, heightmargin, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, heightmargin, 5, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        }
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
            LCD.SetFontColor(BUCKEYECARDGRAY); //0x076F94
            LCD.FillRectangle(widthmargin, 5, noOfCards * CARDWIDTH, CARDHEIGHT);
            FEHIcon::DrawIconArray(handIconArray, 1, noOfCards, 5, heightmargin, widthmargin, widthmargin, cardsInHand, BLACK, BLACK);
        }
    }
}

int Hand::CPUAIDecision(int theme, Hand p1) { // CPU = player 2
    if (theme == 0) {
        LCD.SetFontColor(theme);
        LCD.FillCircle(140, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(160, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(180, 85, 5);
        Sleep(CPUTime);
        LCD.SetFontColor(FORESTGREEN);
        LCD.FillCircle(140, 85, 5);
        LCD.FillCircle(160, 85, 5);
        LCD.FillCircle(180, 85, 5);
    } else if (theme == 1) {
        LCD.SetFontColor(CORAL);
        LCD.FillCircle(140, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(160, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(180, 85, 5);
        Sleep(CPUTime);
        LCD.SetFontColor(DARKCYAN);
        LCD.FillCircle(140, 85, 5);
        LCD.FillCircle(160, 85, 5);
        LCD.FillCircle(180, 85, 5);
    } else if (theme == 2) {
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(140, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(160, 85, 5);
        Sleep(CPUTime);
        LCD.FillCircle(180, 85, 5);
        Sleep(CPUTime);
        LCD.SetFontColor(BUCKEYEGRAY);
        LCD.FillCircle(140, 85, 5);
        LCD.FillCircle(160, 85, 5);
        LCD.FillCircle(180, 85, 5);
    }

    int hitOrNotToHit = 0;

    if (p1.getHandValue() > 21) {
        //stand.
    } else if (handValue <= 11) {
        hitOrNotToHit = 1;
    } else if (handValue > p1.getHandValue()) { // risk starts here
        //stand.
    } else if (handValue < p1.getHandValue()) {
        hitOrNotToHit = 1;
    } else if (handValue = p1.getHandValue()) {
        if (noOfCards > p1.getNoOfCards()) {
            hitOrNotToHit = 1;
        } else if (noOfCards == p1.getNoOfCards()) {
            float probMultiplier = (21 - handValue) / 13.0;
            if (probMultiplier > yoloCoefficient) {
                hitOrNotToHit = 1;
            }

            //debugging lines
            //std::cout << "pM: " << probMultiplier << std::endl;
        }
    } else {
        float probMultiplier = (21 - handValue) / 13.0;
        if (probMultiplier > yoloCoefficient) {
            hitOrNotToHit = 1;
        }

        //std::cout << "pM: " << probMultiplier << std::endl;
    }
        
    //std::cout << "yC: " << yoloCoefficient << std::endl;

    return hitOrNotToHit;
}

void DrawMenu(FEHIcon::Icon *top, FEHIcon::Icon *bottom) {
    FEHImage titlescreen;
    FEHImage exitimg;

    titlescreen.Open("Title ScreenFEH.pic");
    titlescreen.Draw(34, 65);
    titlescreen.Close();

    exitimg.Open("ExitFEH.pic");
    exitimg.Draw(6, 6);
    exitimg.Close();

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

void DrawStatistics() {
    FEHImage statsimg;

    statsimg.Open("StatisticsFEH.pic");
    statsimg.Draw(79, 25);
    statsimg.Close();

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

void DrawCredits() {
    FEHImage creditsimg;

    creditsimg.Open("CreditsFEH.pic");
    creditsimg.Draw(0, 30);
    creditsimg.Close();

    LCD.Update();
}

void DrawBoard(int theme) {

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
        LCD.SetFontColor(BUCKEYECARDGRAY);
        LCD.FillRectangle(20, 100, 40, 40);
        LCD.FillRectangle(260, 100, 40, 40);
        FEHIcon::DrawIconArray(hit, 1, 1, 100, 100, 20, 260, hitIcon, BLACK, BLACK);
        FEHIcon::DrawIconArray(stand, 1, 1, 100, 100, 260, 20, standIcon, BLACK, BLACK);
    }
}

void DrawArrow(int player, int theme) {
    FEHImage downarrowimg;
    FEHImage uparrowimg;

    if (player == 1) {
        if (theme == 0 || theme == 2) {
            downarrowimg.Open("Down Arrow (Black)FEH.pic");
            downarrowimg.Draw(152, 138);
            downarrowimg.Close();
        } else {
            downarrowimg.Open("Down Arrow (Coral)FEH.pic");
            downarrowimg.Draw(152, 138);
            downarrowimg.Close();
        }
    } else if (player == 2) {
        if (theme == 0 || theme == 2) {
            uparrowimg.Open("Up Arrow (Black)FEH.pic");
            uparrowimg.Draw(152, 92);
            uparrowimg.Close();
        } else {
            uparrowimg.Open("Up Arrow (Coral)FEH.pic");
            uparrowimg.Draw(152, 92);
            uparrowimg.Close();
        }
    }
}

void DrawEE() {
    FEHImage EEimg;

    EEimg.Open("EEV2FEH.pic");
    EEimg.Draw(286, 197);
    EEimg.Close();
}

/* Entry point to the application */
int main() {

    int repeat = 1;
    int EE = 0;

    float x, y, xtrash, ytrash;
    int menuState = 0;
    int turn = 1;
    int CPUDecision = 0;
    int passCount = 0;
    int gameOver = 0;

    int p1W = 0, p1L = 0, p1T = 0, p2W = 0, p2L = 0, p2T = 0;

    int theme = 0;
    FEHIcon::Icon top[2], bottom[4], back[1];
    FEHIcon::Icon hit[1], stand[1];

    DrawMenu(top, bottom);

    Deck deck;

    Hand player1(1), player2(2);

    /* debugging lines
    for (int smth = 0; smth < 52; smth++) {
        std::cout << deck.deck[smth] << std::endl;
    }*/

    while (repeat) {
        LCD.ClearBuffer();

        while (!LCD.Touch(&xtrash, &ytrash)) { }
        while (LCD.Touch(&x, &y)) { }

        if (menuState == 0) { // 0 represents the main menu
            if (top[0].Pressed(x, y, 1)) {
                turn = 1;
                passCount = 0;
                gameOver = 0;
                deck.resetDeck();
                player1.resetHand(1);
                player2.resetHand(2);

                LCD.Clear();
                DrawBoard(theme);
                menuState = 3;
                DrawHitStand(hit, stand, theme);

                // GAME CODE
                player1.Hit(&deck);
                player1.Hit(&deck);
                player1.DrawHand(theme);
                LCD.WriteAt("Count:", 110, 165);
                LCD.WriteAt(player1.getHandValue(), 185, 165);

                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.DrawHand(theme);
                LCD.WriteAt("Count:", 110, 60);
                LCD.WriteAt(player2.getHandValue(), 185, 60);

                LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                DrawArrow(1, theme);

            } else if (top[1].Pressed(x, y, 1)) {
                turn = 1;
                passCount = 0;
                gameOver = 0;
                deck.resetDeck();
                player1.resetHand(1);
                player2.resetHand(2);

                LCD.Clear();
                DrawBoard(theme);
                menuState = 4;
                DrawHitStand(hit, stand, theme);

                // GAME CODE
                player1.Hit(&deck);
                player1.Hit(&deck);
                player1.DrawHand(theme);
                LCD.WriteAt("Count:", 110, 165);
                LCD.WriteAt(player1.getHandValue(), 185, 165);

                player2.Hit(&deck);
                player2.Hit(&deck);
                player2.DrawHand(theme);
                LCD.WriteAt("Count:", 110, 60);
                LCD.WriteAt(player2.getHandValue(), 185, 60);

                LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                DrawArrow(1, theme);
                
            } else if (bottom[0].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack();
                menuState = 1;

                DrawStatistics();
                LCD.SetFontColor(RED);
                LCD.WriteAt("Wins: ", 118, 65);
                LCD.WriteAt(p1W, 188, 65);
                LCD.WriteAt("Losses: ", 108, 85);
                LCD.WriteAt(p1L, 198, 85);
                LCD.WriteAt("Ties: ", 118, 105);
                LCD.WriteAt(p1T, 188, 105);

                LCD.WriteAt("Wins: ", 118, 150);
                LCD.WriteAt(p2W, 188, 150);
                LCD.WriteAt("Losses: ", 108, 170);
                LCD.WriteAt(p2L, 198, 170);
                LCD.WriteAt("Ties: ", 118, 190);
                LCD.WriteAt(p2T, 188, 190);
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
                menuState = 10;

                DrawCredits();
            } else if (6 <= x && x < (6 + 12) && 6 <= y && y < (6 + 9)) {
                repeat = 0;
            }

        } else if (menuState == 1 || menuState == 2 || menuState == 10) { // any of the sub-menus / games
            if (7 <= x && x < (7 + 42) && 7 <= y && y < (7 + 14)) {
                LCD.Clear();
                DrawMenu(top, bottom);
                menuState = 0;
                EE = 0;
            }

            if (menuState == 2) { // theme select

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

            if (menuState == 10) {

                if (86 <= x && x < (86+148) && 133 <= y && y < (133+11)) {
                    EE++;
                }
                
                if (EE >= 7) {
                    DrawEE();
                }
                
            }

        } else if (menuState == 3) { // actual game (1-player)
            if (turn == 1) {

                if (hit[0].Pressed(x, y, 1)) {
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player1.Hit(&deck);
                    player1.DrawHand(theme);
                    player2.DrawHand(theme);

                    LCD.WriteAt("Count:", 110, 165);
                    LCD.WriteAt(player1.getHandValue(), 185, 165);
                    LCD.WriteAt("Count:", 110, 60);
                    LCD.WriteAt(player2.getHandValue(), 185, 60);

                    LCD.WriteAt("CPU'S TURN", 100, 112);
                    passCount = 0;
                    
                    CPUDecision = player2.CPUAIDecision(theme, player1);
                    if (CPUDecision == 0) {
                        passCount++;
                    } else if (CPUDecision == 1) {
                        player2.Hit(&deck);
                        passCount = 0;
                    }

                    if (theme == 0) {
                        LCD.SetFontColor(FORESTGREEN);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(BLACK);
                    } else if (theme == 1) {
                        LCD.SetFontColor(DARKCYAN);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(CORAL);
                    } else if (theme == 2) {
                        LCD.SetFontColor(BUCKEYEGRAY);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(BLACK);
                    }
                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);

                } else if (stand[0].Pressed(x, y, 1)) {
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player1.DrawHand(theme);
                    player2.DrawHand(theme);

                    LCD.WriteAt("Count:", 110, 165);
                    LCD.WriteAt(player1.getHandValue(), 185, 165);
                    LCD.WriteAt("Count:", 110, 60);
                    LCD.WriteAt(player2.getHandValue(), 185, 60);

                    LCD.WriteAt("CPU'S TURN", 100, 112);
                    passCount++;

                    CPUDecision = player2.CPUAIDecision(theme, player1);
                    if (CPUDecision == 0) {
                        passCount++;
                    } else if (CPUDecision == 1) {
                        player2.Hit(&deck);
                        passCount = 0;
                    }
                    
                    if (theme == 0) {
                        LCD.SetFontColor(FORESTGREEN);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(BLACK);
                    } else if (theme == 1) {
                        LCD.SetFontColor(DARKCYAN);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(CORAL);
                    } else if (theme == 2) {
                        LCD.SetFontColor(BUCKEYEGRAY);
                        LCD.FillRectangle(95, 110, 150, 20);
                        LCD.FillRectangle(110, 60, 100, 30);
                        LCD.SetFontColor(BLACK);
                    }
                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);
                }

            }

            if (player1.getHandValue() > 21) { //bust conditions
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p1L++;
                p2W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 1 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 2 WINS", 7, 6);
            } else if (player2.getHandValue() > 21) {
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p2L++;
                p1W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 2 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 1 WINS", 7, 6);
            } else if (passCount >= 2) { // both stand condition
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                gameOver = 1;
                if (player1.getHandValue() > player2.getHandValue()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                } else if (player2.getHandValue() > player1.getHandValue()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                } else if (player1.getNoOfCards() < player2.getNoOfCards()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                } else if (player2.getNoOfCards() < player1.getNoOfCards()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                } else {
                    p1T++;
                    p2T++;
                    LCD.WriteRC("TIE", 6, 12);
                }
            }
            
            player1.DrawHand(theme);
            player2.DrawHand(theme);

            LCD.WriteAt("Count:", 110, 165);
            LCD.WriteAt(player1.getHandValue(), 185, 165);
            LCD.WriteAt("Count:", 110, 60);
            LCD.WriteAt(player2.getHandValue(), 185, 60);

        } else if (menuState == 4) { // actual game (2-player)
            if (turn == 1) {

                if (hit[0].Pressed(x, y, 1)) {
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player1.Hit(&deck);

                    LCD.WriteAt("PLAYER 2'S TURN", 70, 112);
                    DrawArrow(2, theme);
                    turn++;
                    passCount = 0;
                } else if (stand[0].Pressed(x, y, 1)) {
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    LCD.WriteAt("PLAYER 2'S TURN", 70, 112);
                    DrawArrow(2, theme);
                    turn++;
                    passCount++;
                }

            } else if (turn == 2) {

                if (hit[0].Pressed(x, y, 1)) {
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player2.Hit(&deck);

                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);
                    turn--;
                    passCount = 0;
                } else if (stand[0].Pressed(x, y, 1)){
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);
                    turn--;
                    passCount++;
                }

            }

            if (player1.getHandValue() > 21) { //bust conditions
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p1L++;
                p2W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 1 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 2 WINS", 7, 6);
            } else if (player2.getHandValue() > 21) {
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p2L++;
                p1W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 2 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 1 WINS", 7, 6);
            } else if (passCount >= 2) { // both stand condition
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                gameOver = 1;
                if (player1.getHandValue() > player2.getHandValue()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                } else if (player2.getHandValue() > player1.getHandValue()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                } else if (player1.getNoOfCards() < player2.getNoOfCards()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                } else if (player2.getNoOfCards() < player1.getNoOfCards()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                } else {
                    p1T++;
                    p2T++;
                    LCD.WriteRC("TIE", 6, 12);
                }

                
            }
            
            player1.DrawHand(theme);
            player2.DrawHand(theme);

            LCD.WriteAt("Count:", 110, 165);
            LCD.WriteAt(player1.getHandValue(), 185, 165);
            LCD.WriteAt("Count:", 110, 60);
            LCD.WriteAt(player2.getHandValue(), 185, 60);

        }

        if (gameOver == 1 && repeat == 1) {
            while (!LCD.Touch(&xtrash, &ytrash)) { }
            while (LCD.Touch(&x, &y)) { }

            LCD.Clear();
            DrawMenu(top, bottom);
            menuState = 0;
            gameOver = 0;
        }
        
        LCD.Update();

        Sleep(0.05);
    }

    return 0;
}
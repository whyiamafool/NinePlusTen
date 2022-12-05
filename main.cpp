// Complete Nine Plus Ten code for FEH's Software Development Project.
// References include the FEH Proteus Guide and the Simple Wikipedia page for Blackjack.

#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "FEHRandom.h"
#include <iostream>
#include <string>


// constants for game, color hex codes, and cpu computational time
#define CARDWIDTH 42
#define CARDHEIGHT 50

#define OCEANBORDER 0x076F94
#define SAND 0xF6BB99
#define CORAL 0xF55F51

#define BUCKEYERED 0xBB0000
#define BUCKEYEGRAY 0x666666
#define BUCKEYECARDGRAY 0xCBCBD1

#define CPUTime 0.5

// creates a class that holds all 52 card combinations, deals a random card, as well as a few variables holding the cards that have been dealt
// Aditya
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

// constructor, resets everything to 0
Deck::Deck() {
    dealtcardindex = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(dealtcards[i], "");
    }
}

// generates and returns a unique random card
//Aditya
char* Deck::DrawRandomCard() {
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

// resets everything to 0
void Deck::resetDeck() {
    dealtcardindex = 0;
    for (int i = 0; i < 52; i++) {
        strcpy(dealtcards[i], "");
    }
}


// holds the hand of a player. includes hand-specific variables and getters. player 2 is the CPU in single player mode.
// Aditya
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

// constructor, resets everything to 0 for a specific player
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

// draws an unique random card from the passed-in deck and adds it to the cardsInHand array
// Aaryan & Aditya
void Hand::Hit(Deck *deckptr) {
    strcpy(cardsInHand[noOfCards], deckptr -> DrawRandomCard());
    
    if (cardsInHand[noOfCards][0] == 'A') {
        noOfAces++;
    }

    noOfCards++;
}

// getter for number of cards
int Hand::getNoOfCards() {
    return noOfCards;
}

// getter for the value of the hand. includes some math on how the value of an Ace is calculated.
// Aditya
int Hand::getHandValue() {
    handValue = 0;
    aceDifferential = 0;

    for (int i = 0; i < noOfCards; i++) {
        if (cardsInHand[i][0] == 'K' || cardsInHand[i][0] == 'Q' || cardsInHand[i][0] == 'J' || cardsInHand[i][0] == '1') {
            handValue += 10;
        } else if (cardsInHand[i][0] == 'A') { // by default, the value of an Ace is 11
            handValue += 11;
        } else {
            handValue += (cardsInHand[i][0] - '0');
        }
    }

    while (handValue > 21 && noOfAces > aceDifferential) { // the value of an Ace only changes when the count is over 21 and there is an Ace to change the value of
        handValue -= 10;
        aceDifferential++;
    }

    return handValue;
}

// getter for the number of Aces
int Hand::getAces() {
    return noOfAces;
}

// resets everything to 0 for a specific player
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

// Uses the FEHIcon method to draw the hands of a specific player in a specific theme 
// Aaryan & Aditya
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

// 'AI' that makes intelligent moves based on player 1's hand for the single-player mode, the CPU is player 2.
// The CPU decision dots are based on the theme selected.
// Aditya
int Hand::CPUAIDecision(int theme, Hand p1) {
    if (theme == 0) { // three thinking dots 
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

    int hitOrNotToHit = 0; // default = stand

    if (p1.getHandValue() > 21) {
        //stand as game is over.
    } else if (handValue <= 11) {
        hitOrNotToHit = 1; // hit since it is impossible to bust with a count under or equal to 11
    } else if (handValue > p1.getHandValue()) { // risk starts here
        //stand since the CPU is winning currently
    } else if (handValue < p1.getHandValue()) {
        hitOrNotToHit = 1; // the CPU must hit as they are losing
    } else if (handValue = p1.getHandValue()) {
        if (noOfCards > p1.getNoOfCards()) {
            hitOrNotToHit = 1; // the CPU must hit as they are losing
        } else if (noOfCards == p1.getNoOfCards()) {
            float probMultiplier = (21 - handValue) / 13.0; // based on the probability the next card will work, and a randomly generated coefficient, the CPU will hit.
            if (probMultiplier > yoloCoefficient) {
                hitOrNotToHit = 1;
            }

            //debugging lines
            //std::cout << "pM: " << probMultiplier << std::endl;
        }
    }

    return hitOrNotToHit;
}

// All draw functions worked on both Aaryan and Aditya

// draws the menu with the top and bottom Icons passed in.
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

// draws the back button
void DrawBack() {
    FEHImage backimg;
    
    backimg.Open("BackFEH.pic");
    backimg.Draw(7, 7);
    backimg.Close();

    LCD.Update();
}

// draws the statistics
void DrawStatistics() {
    FEHImage statsimg;

    statsimg.Open("StatisticsFEH.pic");
    statsimg.Draw(79, 25);
    statsimg.Close();

    LCD.Update();
}

// draws the rules screen
void DrawRules() {
    FEHImage rulesimg;

    rulesimg.Open("InstructionsFEH.pic");
    rulesimg.Draw(0, 30);
    rulesimg.Close();

    LCD.Update();
}

// draws the theme select menu, text is in that specific theme
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

// draws the credits
void DrawCredits() {
    FEHImage creditsimg;

    creditsimg.Open("CreditsFEH.pic");
    creditsimg.Draw(0, 30);
    creditsimg.Close();

    LCD.Update();
}

// draws the game board according to a specific theme
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

// draws the hit and stand button based on a specific theme. The Hit and Stand FEHIcons are passed in to draw the array.
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

// draws the arrow indicating whose turn it is. the theme and player determines the color and direction the arrow faces.
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

// :)
void DrawEE() {
    FEHImage EEimg;

    EEimg.Open("EEV2FEH.pic");
    EEimg.Draw(286, 197);
    EEimg.Close();
}

// worked on by both Aaryan and Aditya
/* Entry point to the game */
int main() {

    int repeat = 1; // resets important game variables, creates FEHIcon::Icons.
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
        while (LCD.Touch(&x, &y)) { } // wait until a touch is made & released

        if (menuState == 0) { // 0 represents the main menu
            if (top[0].Pressed(x, y, 1)) { // single player mode first loop
                turn = 1; // reset vars
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
                player1.Hit(&deck); // sets up hands for player 1 and CPU
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

            } else if (top[1].Pressed(x, y, 1)) { // multi player mode first loop
                turn = 1; // reset vars
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
                player1.Hit(&deck); // sets up hands for player 1 and player 2
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
                
            } else if (bottom[0].Pressed(x, y, 1)){ // statistics page
                LCD.Clear();
                DrawBack();
                menuState = 1;

                DrawStatistics();
                LCD.SetFontColor(RED);
                LCD.WriteAt("Wins: ", 118, 65); // actual statistics
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
            } else if (bottom[1].Pressed(x, y, 1)){ // rules page
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
            } else if (bottom[3].Pressed(x, y, 1)){ // credits page
                LCD.Clear();
                DrawBack();
                menuState = 10;

                DrawCredits();
            } else if (6 <= x && x < (6 + 12) && 6 <= y && y < (6 + 9)) { // x out of application button.
                repeat = 0;
            }

        } else if (menuState == 1 || menuState == 2 || menuState == 10) { // any of the sub-menus / games
            if (7 <= x && x < (7 + 42) && 7 <= y && y < (7 + 14)) { // back button
                LCD.Clear();
                DrawMenu(top, bottom);
                menuState = 0;
                EE = 0;
            }

            if (menuState == 2) { // theme select
                if (52 <= x && x < 270 && 78 <= y && y < 123) { // casino hitbox
                    theme = 0;
                    DrawThemeSelect(theme);
                } else if (52 <= x && x < 270 && 127 <= y && y < 172) { // ocean hitbox
                    theme = 1;
                    DrawThemeSelect(theme);
                } else if (52 <= x && x < 270 && 176 <= y && y < 221) { // buckeyes hitbox
                    theme = 2;
                    DrawThemeSelect(theme);
                }
            }

            if (menuState == 10) { // :)
                if (86 <= x && x < (86+148) && 133 <= y && y < (133+11)) {
                    EE++;
                }
                if (EE >= 7) {
                    DrawEE();
                }
            }
        } else if (menuState == 3) { // actual game (1-player)
            if (turn == 1) { // matched for similarility

                if (hit[0].Pressed(x, y, 1)) { // if player 1 hits...
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player1.Hit(&deck); // hit and draw the hands
                    player1.DrawHand(theme);
                    player2.DrawHand(theme);

                    LCD.WriteAt("Count:", 110, 165);
                    LCD.WriteAt(player1.getHandValue(), 185, 165);
                    LCD.WriteAt("Count:", 110, 60);
                    LCD.WriteAt(player2.getHandValue(), 185, 60);

                    LCD.WriteAt("CPU'S TURN", 100, 112);
                    passCount = 0;
                    
                    CPUDecision = player2.CPUAIDecision(theme, player1); // CPU makes a decision
                    if (CPUDecision == 0) {
                        passCount++;
                    } else if (CPUDecision == 1) {
                        player2.Hit(&deck); // hit if CPU decides
                        passCount = 0; // resets stand/pass count
                    }

                    if (theme == 0) { // resets center text
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

                } else if (stand[0].Pressed(x, y, 1)) { // if player 1 stands...
                    LCD.Clear(BLACK);  // draw board, buttons, and hands
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

                    CPUDecision = player2.CPUAIDecision(theme, player1); // same as above
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

            // win conditions as follows
            if (player1.getHandValue() > 21) { //if player 1 busts
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p1L++; //statistics
                p2W++;
                gameOver = 1; // represents when the game is over to kill while loop
                LCD.WriteRC("PLAYER 1 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 2 WINS", 7, 6);
                LCD.WriteRC("CLICK TO EXIT", 8, 6);
            } else if (player2.getHandValue() > 21) { //if player 2 busts
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p2L++; //statistics
                p1W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 2 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 1 WINS", 7, 6);
                LCD.WriteRC("CLICK TO EXIT", 8, 6);
            } else if (passCount >= 2) { // both stand condition
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                gameOver = 1;
                if (player1.getHandValue() > player2.getHandValue()) { // individual win cases
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player2.getHandValue() > player1.getHandValue()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player1.getNoOfCards() < player2.getNoOfCards()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player2.getNoOfCards() < player1.getNoOfCards()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else {
                    p1T++;
                    p2T++;
                    LCD.WriteRC("TIE", 6, 12);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                }
            }
            
            player1.DrawHand(theme);
            player2.DrawHand(theme);

            LCD.WriteAt("Count:", 110, 165);
            LCD.WriteAt(player1.getHandValue(), 185, 165);
            LCD.WriteAt("Count:", 110, 60);
            LCD.WriteAt(player2.getHandValue(), 185, 60);

        } else if (menuState == 4) { // actual game (2-player)
            if (turn == 1) { // player 1's turn
                if (hit[0].Pressed(x, y, 1)) { // player 1 hits
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player1.Hit(&deck);

                    LCD.WriteAt("PLAYER 2'S TURN", 70, 112);
                    DrawArrow(2, theme);
                    turn++;
                    passCount = 0; // reset pass/stand count
                } else if (stand[0].Pressed(x, y, 1)) { // player 2 stands
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    LCD.WriteAt("PLAYER 2'S TURN", 70, 112);
                    DrawArrow(2, theme);
                    turn++;
                    passCount++;
                }

            } else if (turn == 2) { // player 2's turn
                if (hit[0].Pressed(x, y, 1)) { // player 1 hits
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    player2.Hit(&deck);

                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);
                    turn--;
                    passCount = 0; // reset pass/stand count
                } else if (stand[0].Pressed(x, y, 1)){ // player 2 stands
                    LCD.Clear(BLACK);
                    DrawBoard(theme);
                    DrawHitStand(hit, stand, theme);

                    LCD.WriteAt("PLAYER 1'S TURN", 70, 112);
                    DrawArrow(1, theme);
                    turn--;
                    passCount++;
                }
            }

            if (player1.getHandValue() > 21) { //same as above
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p1L++;
                p2W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 1 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 2 WINS", 7, 6);
                LCD.WriteRC("CLICK TO EXIT", 8, 6);
            } else if (player2.getHandValue() > 21) {
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                p2L++;
                p1W++;
                gameOver = 1;
                LCD.WriteRC("PLAYER 2 BUSTS", 6, 6);
                LCD.WriteRC("PLAYER 1 WINS", 7, 6);
                LCD.WriteRC("CLICK TO EXIT", 8, 6);
            } else if (passCount >= 2) { // both stand condition
                LCD.Clear(BLACK);
                DrawBoard(theme);
                DrawHitStand(hit, stand, theme);

                gameOver = 1;
                if (player1.getHandValue() > player2.getHandValue()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player2.getHandValue() > player1.getHandValue()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player1.getNoOfCards() < player2.getNoOfCards()) {
                    p2L++;
                    p1W++;
                    LCD.WriteRC("PLAYER 1 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else if (player2.getNoOfCards() < player1.getNoOfCards()) {
                    p1L++;
                    p2W++;
                    LCD.WriteRC("PLAYER 2 WINS", 6, 6);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                } else {
                    p1T++;
                    p2T++;
                    LCD.WriteRC("TIE", 6, 12);
                    LCD.WriteRC("CLICK TO EXIT", 8, 6);
                }
            }
            
            player1.DrawHand(theme);
            player2.DrawHand(theme);

            LCD.WriteAt("Count:", 110, 165);
            LCD.WriteAt(player1.getHandValue(), 185, 165);
            LCD.WriteAt("Count:", 110, 60);
            LCD.WriteAt(player2.getHandValue(), 185, 60);
        }

        if (gameOver == 1 && repeat == 1) { // if the game is over, exit to main menu when screen is clicked.
            while (!LCD.Touch(&xtrash, &ytrash)) { }
            while (LCD.Touch(&x, &y)) { }

            LCD.Clear();
            DrawMenu(top, bottom);
            menuState = 0;
            gameOver = 0;
        }
        
        LCD.Update();
        Sleep(0.05); // touch buffer
    }

    return 0;
}
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"

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

void DrawBack(FEHIcon::Icon *back) {
    FEHImage backimg;

    backimg.Open("BackFEH.pic");
    backimg.Draw(7, 7);
    backimg.Close();

    LCD.Update();
}

void DrawBoard() {

}

/* Entry point to the application */
int main() {

    float x, y, xtrash, ytrash;
    int menuState = 0;
    FEHIcon::Icon top[2], bottom[4], back[1];
    DrawMenu(top, bottom);

    FEHImage instructions;

    while (1) {
        LCD.ClearBuffer();

        while (!LCD.Touch(&xtrash, &ytrash)) { }
        while (LCD.Touch(&x, &y)) { }

        if (menuState == 0) { // 0 represents the main menu
            if (top[0].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBack(back);
                menuState = 1;
            
                // single player game goes here bruh
                LCD.WriteAt("game", 87, 111);
            } else if (top[1].Pressed(x, y, 1)) {
                LCD.Clear();
                DrawBack(back);
                menuState = 1;
                
                LCD.WriteAt("game", 87, 111);
            } else if (bottom[0].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack(back);
                menuState = 1;

                LCD.DrawRectangle(75, 5, 240, 35);
                LCD.WriteRC("STATISTICS", 1, 11);
                LCD.WriteRC("Wins: 5", 6, 9);
                LCD.WriteRC("Losses: 12", 7, 8);
            } else if (bottom[1].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack(back);
                menuState = 1;
                // rules page

                instructions.Open("InstructionsFEH.pic");
                instructions.Draw(0, 30);
                instructions.Close();
            } else if (bottom[2].Pressed(x, y, 1 )){
                LCD.Clear();
                DrawBack(back);
                menuState = 1;
                // theme select
                LCD.WriteRC("Restricted Area Please Leave", 6, 9);
            } else if (bottom[3].Pressed(x, y, 1)){
                LCD.Clear();
                DrawBack(back);
                menuState = 1;

                LCD.WriteRC("Written By:", 5, 8);
                LCD.WriteRC("Aditya Chittari", 7, 6);
                LCD.WriteRC("Aaryan Makavana", 8, 6);
            }
        } else { // any of the sub-menus / games
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
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"

void DrawMenu(FEHIcon::Icon *menu) {
    LCD.DrawRectangle(0,0,320,240);
    LCD.Update();

    //FEHIcon::Icon menu[4];

    char menu_labels[4][20] = {"PLAY","STATS","HOW TO PLAY","CREDITS"};
    FEHIcon::DrawIconArray(menu, 2, 2, 10, 10, 5, 5, menu_labels, GOLD, GREEN);
    
    LCD.Update();
    LCD.Update();
}

/* Entry point to the application */
int main() {

    float x, y, xtrash, ytrash;
    int menuState = -1;
    FEHIcon::Icon menu[4], back[1];
    char backlbl[1][20] = {"BACK"};
    DrawMenu(menu);

    FEHImage instructions;

    while (1) {
        LCD.ClearBuffer();

        while (!LCD.Touch(&xtrash, &ytrash)) { }
        while (LCD.Touch(&x, &y)) { }
        
        //LCD.DrawRectangle(0,0,320,240);

        if (menuState == -1) { // -1 represents main menu
            if (menu[0].Pressed(x, y, 1)) {
                LCD.Clear();
                menuState = 0;
                FEHIcon::DrawIconArray(back, 1, 1, 5, 200, 5, 250, backlbl, WHITE, WHITE);
                LCD.WriteAt("Game Started!", 87, 111);
            } else if (menu[1].Pressed(x, y, 1)) {
                LCD.Clear();
                menuState = 1;
                FEHIcon::DrawIconArray(back, 1, 1, 5, 200, 5, 250, backlbl, WHITE, WHITE);
                LCD.DrawRectangle(75, 5, 240, 35);
                LCD.WriteRC("STATISTICS", 1, 11);
    
                LCD.WriteRC("Wins: 5", 6, 9);
                LCD.WriteRC("Losses: 12", 7, 8);
            } else if (menu[2].Pressed(x, y, 1)) {
                LCD.Clear();
                menuState = 2;
                FEHIcon::DrawIconArray(back, 1, 1, 5, 200, 5, 250, backlbl, WHITE, WHITE);

                instructions.Open("InstructionsFEH.pic");
                instructions.Draw(0, 0);
                instructions.Close();
    
                //LCD.WriteRC("Instructions:", 5, 7);
                //LCD.WriteRC("(imagine)", 7, 9);
            } else if (menu[3].Pressed(x, y, 1)) {
                LCD.Clear();
                menuState = 3;
                FEHIcon::DrawIconArray(back, 1, 1, 5, 200, 5, 250, backlbl, WHITE, WHITE);
    
                LCD.WriteRC("Written By:", 5, 8);
                LCD.WriteRC("Aditya Chittari", 7, 6);
                LCD.WriteRC("Aaryan Makavana", 8, 6);
            }
        } else {
            if (back[0].Pressed(x, y, 1)) {
                LCD.Clear();
                menuState = -1;
                DrawMenu(menu);
            }
        }

        LCD.Update();

        Sleep(0.125);
    }

    return 0;
}
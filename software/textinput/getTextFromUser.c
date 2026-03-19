#include "getTextFromUser.h"

#include "../../hardware/keyboard.h"
#include "../graphics/graphics.h"
#include "../graphics/textbox/textMessages.h"


#define USER_TEXT_MAX 32

static char g_userText[USER_TEXT_MAX];

const char *getUserText(void)
{
    return g_userText;
}

void getTextFromUserIntoTextbox(const unsigned short *textBoxSprite,
                                 int x, int y,
                                 short textColour,
                                 const char *templateWithName)
{
    int len = 0;
    g_userText[0] = '\0';

    // render storage at a stable address so animations work

    char render[96];
    render[0] = '\0';
    int animCadence = 0;

    while (1) {
        update_keyboard();

        char c;
        while (keyboard_pop_char(&c)) {
            if (c == '\n') {
                // Only accept Enter when something was typed.
                if (len > 0) {
                    g_userText[len] = '\0';
                    return;
                }
                continue;
            }

            if (c == '\b') { //backspace pressed
                if (len > 0) len--;
                g_userText[len] = '\0';
                continue;
            }

            if (c >= ' ' && c <= '~') { //normal character input
                if (len < (USER_TEXT_MAX - 1)) {
                    g_userText[len++] = c;
                    g_userText[len] = '\0';
                }
            }
        }


        animCadence++;
        if (animCadence >= 3) { // update every 3 loop iterations
            animCadence = 0;
            formatTextWithNameToken(render, sizeof(render), templateWithName, g_userText);
            draw_textbox_animated_text(textBoxSprite, x, y, render, textColour);
        }
        wait_for_vsync();
    }
}


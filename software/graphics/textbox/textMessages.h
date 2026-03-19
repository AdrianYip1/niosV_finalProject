#pragma once

// `{NAME}` will be replaced with the user-entered name.

#include <stddef.h>
#include <string.h>

#define TEXTBOX_NAME_TOKEN "{NAME}"

static const char TEXT_TITLE[] = "PRESS SPACE TO START";

//question + (then) appended live name while typing.
static const char TEXTBOX_MSG_2[] = "Hello! What is your name? \n\{NAME}";
static const char TEXTBOX_MSG_3[] = "{NAME}... Is that correct?";
static const char TEXTBOX_MSG_4[] = "Ok! Welcome{NAME}";
static const char TEXTBOX_MSG_5[] = "placeholder text 123!@#$%^&*()";
static const char TEXTBOX_MSG_6[] = "more text ";

static const char *const TEXT_MESSAGES[] = {
    TEXT_TITLE,
    TEXTBOX_MSG_2,
    TEXTBOX_MSG_3,
    TEXTBOX_MSG_4,
    TEXTBOX_MSG_5,
    TEXTBOX_MSG_6,
};

#define TEXT_MESSAGES_COUNT (sizeof(TEXT_MESSAGES) / sizeof(TEXT_MESSAGES[0]))

// Replace {NAME} with name into out
// Returns a null-terminated string 
static inline void formatTextWithNameToken(char *out,
                                             size_t outSize,
                                             const char *templateStr,
                                             const char *name) {

    //defensive guards
    if (out == NULL || outSize == 0) return;
    out[0] = '\0';
    if (templateStr == NULL) return;
    if (name == NULL) name = "";

    const size_t tokenLen = strlen(TEXTBOX_NAME_TOKEN);
    size_t outPos = 0;
    const char *p = templateStr;

    while (*p != '\0' && outPos + 1 < outSize) {
        const char *t = strstr(p, TEXTBOX_NAME_TOKEN); //find next token location
        if (t == NULL) {
            // No more tokens, copy remainder.
            const size_t rem = strlen(p);
            const size_t canCopy = (rem < (outSize - 1 - outPos)) ? rem : (outSize - 1 - outPos);
            if (canCopy > 0) {
                memcpy(out + outPos, p, canCopy);
                outPos += canCopy;
            }
            break;
        }

        // Copy part before the token.
        const size_t before = (size_t)(t - p);
        const size_t canCopyBefore = (before < (outSize - 1 - outPos)) ? before : (outSize - 1 - outPos);
        if (canCopyBefore > 0) {
            memcpy(out + outPos, p, canCopyBefore);
            outPos += canCopyBefore;
        }

        if (outPos + 1 >= outSize) break;

        // Copy the name token replacement.
        const size_t nlen = strlen(name);
        const size_t canCopyName = (nlen < (outSize - 1 - outPos)) ? nlen : (outSize - 1 - outPos);
        if (canCopyName > 0) {
            memcpy(out + outPos, name, canCopyName);
            outPos += canCopyName;
        }

        p = t + tokenLen;
    }

    out[outPos] = '\0';
}


#pragma once

static const char TEXT_TITLE[] = "Text for testing 123";
static const char TEXTBOX_MSG_2[] = "Second message!";
static const char TEXTBOX_MSG_3[] = "Another message";


#define TEXTMSG_TITLE 0
#define TEXTMSG_SECOND 1
#define TEXTMSG_THIRD 2

static const char *const TEXT_MESSAGES[] = {
    TEXT_TITLE,
    TEXTBOX_MSG_2,
    TEXTBOX_MSG_3,
};

#define TEXT_MESSAGES_COUNT (sizeof(TEXT_MESSAGES) / sizeof(TEXT_MESSAGES[0]))


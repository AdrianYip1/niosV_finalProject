#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

/*
Screen-space coordinate system: origin (0,0) at top-left, x increases right,
y increases down. Screen is SCREEN_WIDTH x SCREEN_HEIGHT (320x240).
Constants named _X/_Y are absolute screen positions.
Constants named _FROM_, _X0, _STEP_, _DX, _DY are relative offsets.
 */

//Battle action icons (bottom-left cluster)
#define BATTLE_ICON_FIGHT_X 0
#define BATTLE_ICON_FIGHT_Y (240 - 3 - BATTLE_ICON_FIGHT_HEIGHT)

#define BATTLE_ICON_BAG_X (BATTLE_ICON_FIGHT_X + BATTLE_ICON_FIGHT_WIDTH - 3)
#define BATTLE_ICON_BAG_Y (BATTLE_ICON_FIGHT_Y)

#define BATTLE_ICON_RUN_X (BATTLE_ICON_BAG_X)
#define BATTLE_ICON_RUN_Y (230 - BATTLE_ICON_SMALL_HEIGHT + 12)

//Battle party grid
#define BATTLE_PARTY_X (320 / 2 + 14)
#define BATTLE_PARTY_Y (240 - 91 + 1)

// HP bars
#define OPP_HP_EMPTY_X 0
#define OPP_HP_EMPTY_Y 15
#define MY_HP_EMPTY_X  (340 - MY_HP_EMPTY_WIDTH - 20)
#define MY_HP_EMPTY_Y  (TEXTBOX_Y - MY_HP_EMPTY_HEIGHT - 20)

#define HP_WIDTH  48
#define HP_HEIGHT 4

#define myHP_X (MY_HP_EMPTY_X + 62)
#define myHP_Y (MY_HP_EMPTY_Y + 20)
#define myHP_SHOWN_X (MY_HP_EMPTY_X + 62)
#define myHP_SHOWN_Y (MY_HP_EMPTY_Y + 20)

#define myLVL_X  (MY_HP_EMPTY_X + 98)
#define myLVL_Y  (MY_HP_EMPTY_Y + 7)
#define oppLVL_X (OPP_HP_EMPTY_X + 85)
#define oppLVL_Y (OPP_HP_EMPTY_Y + 7)

#define CAUGHT_X  (OPP_HP_EMPTY_X + 5)
#define CAUGHT_Y  (OPP_HP_EMPTY_Y + 18)
#define STATUS_X  (CAUGHT_X + 9)
#define STATUS_Y  CAUGHT_Y

#define MYSTATUS_X (myHP_X - 37)
#define MYSTATUS_Y (myHP_SHOWN_Y - 2)

#define MYNAME_X  (MY_HP_EMPTY_X + 15)
#define MYNAME_Y  myLVL_Y
#define OPPNAME_X (OPP_HP_EMPTY_X + 5)
#define OPPNAME_Y oppLVL_Y

#define EXP_X (MY_HP_EMPTY_X + 30)
#define EXP_Y (MY_HP_EMPTY_Y + 38)
#define EXP_WIDTH  88
#define EXP_HEIGHT 2

#define TOTAL_HPNUM_X (MY_HP_EMPTY_X + 65)
#define REMAINING_HP_X (MY_HP_EMPTY_X + 97)
#define HPNUM_Y (MY_HP_EMPTY_Y + 27)

// PP display (offset from attack sprite top-left)
#define CURRENT_PP_X_FROM_ATTACK_SPRITE 73
#define CURRENT_PP_Y_FROM_ATTACK_SPRITE 24
#define TOTAL_PP_X_FROM_CURRENT_PP 22
#define TOTAL_PP_Y_FROM_CURRENT_PP 0

// Spacebar prompt
#define SPACEBAR_X ((SCREEN_WIDTH - SPACEBAR_WIDTH) / 2)
#define SPACEBAR_Y (TEXTBOX_Y - SPACEBAR_HEIGHT - 6)
#define SPACEBAR_TITLE_Y (SCREEN_HEIGHT - SPACEBAR_HEIGHT - 5)
#define SMALL_SPACEBAR_X (SPACEBAR_X + SPACEBAR_WIDTH + 75)
#define SMALL_SPACEBAR_Y (SPACEBAR_Y + SPACEBAR_HEIGHT + 50)

// Title screen text 
#define TITLE_TEXT_PIXEL_WIDTH ((int)((sizeof(TEXT_TITLE) - 1) * 8))
#define TITLE_TEXT_X_CENTERED ((SCREEN_WIDTH - TITLE_TEXT_PIXEL_WIDTH) / 2)
#define TITLE_TEXT_Y_ABOVE_BAR (SPACEBAR_TITLE_Y - 12)

// Battle party switch slots (row 1: top, row 2: bottom)
#define PARTY_1_X (170 - 2)
#define PARTY_1_Y (240 - 91 - 18)
#define PARTY_2_X (210 + 1)
#define PARTY_2_Y (240 - 91 - 18)
#define PARTY_3_X (250 + 3)
#define PARTY_3_Y (240 - 91 - 18)
#define PARTY_4_X (173 - 2)
#define PARTY_4_Y (240 - 91 + 24)
#define PARTY_5_X (213 + 1)
#define PARTY_5_Y (240 - 91 + 24)
#define PARTY_6_X (253 + 3)
#define PARTY_6_Y (240 - 91 + 24)

#define battleBackdropY (240 - 91)

// PC menu 
#define PC_MENU_ARROW_Y (20 - 9)
#define PC_MENU_LEFT_ARROW_X 3
#define PC_MENU_RIGHT_ARROW_X ((340 / 2) + 20 + 20)

#define PC_MENU_GRID_COLS 5
#define PC_MENU_GRID_ROWS 5
#define PC_MENU_GRID_X0 (10 + 2)
#define PC_MENU_GRID_Y0 (40 + 4 - 5)
#define PC_MENU_GRID_STEP_X 42
#define PC_MENU_GRID_STEP_Y (40 - 10)

#define PC_MENU_PARTY_BOX_X 200
#define PC_MENU_PARTY_BOX_Y (120 - (PC_MENU_PARTY_BOX_HEIGHT / 2))
#define PC_MENU_PARTY_COLS 2
#define PC_MENU_PARTY_ROWS 3
#define PC_MENU_PARTY_X0 (PC_MENU_PARTY_BOX_X + 10 + 20)
#define PC_MENU_PARTY_Y0 (PC_MENU_PARTY_BOX_Y + 10 - 2)
#define PC_MENU_PARTY_STEP_X (53 - 3)
#define PC_MENU_PARTY_STEP_Y (50 - 10 - 5 - 5)

#define PC_MENU_PARTY_RIGHT_COL_DY 25
#define PC_MENU_PARTY_ROW1_DY (-7)
#define PC_MENU_PARTY_ROW2_DY (-20)
#define PC_MENU_PARTY_RIGHT_COL_DX (-3)

// Bag / item UI 
#define USE_LAST_ICON_X (60)
#define USE_LAST_ICON_Y (240 - BATTLE_BAG_USE_LAST_ITEM_HEIGHT - 1)

#define ITEM_X 20
#define ITEM_Y 24
#define ITEM_NAME_X 20
#define ITEM_NAME_Y 8

#define LAST_USED_ITEM_X 20
#define LAST_USED_ITEM_Y 10

#define DEFINITION_X 20
#define DEFINITION_Y 10
#define DEFINITION_X_NAME 52
#define DEFINITION_Y_NAME 15
#define DEFINITION_X_TEXT 21
#define DEFINITION_Y_TEXT 45

#define ITEM_AMOUNT_X 54
#define ITEM_AMOUNT_Y 31

// Restore items are slightly offset from Poké Ball items due to pixel size differences
#define RESTORE_ITEM_X (ITEM_X - 6)
#define RESTORE_ITEM_Y (ITEM_Y - 5)

// Battle area sprites
#define AREA_FRONT_X 160
#define AREA_FRONT_Y 70
#define AREA_BACK_X 0
#define AREA_BACK_Y 120

// Party menu for the nonselected slot
#define NON_SELECTED_POKEMON_X 5
#define NON_SELECTED_POKEMON_Y 8
#define NON_SELECTED_POKEMON_HP_X 63
#define NON_SELECTED_POKEMON_HP_Y 24
#define NON_SELECTED_POKEMON_HP_WIDTH 48
#define NON_SELECTED_POKEMON_HP_HEIGHT 4
#define NON_SELECTED_POKEMON_CURRENT_HP_TEXT_X 67
#define NON_SELECTED_POKEMON_CURRENT_HP_TEXT_Y 39
#define NON_SELECTED_POKEMON_MAX_HP_TEXT_X 93
#define NON_SELECTED_POKEMON_MAX_HP_TEXT_Y 39
#define NON_SELECTED_POKEMON_LEVEL_TEXT_X 23
#define NON_SELECTED_POKEMON_LEVEL_TEXT_Y 39
#define POKEMON_TEXT_MENU_X 42
#define POKEMON_TEXT_MENU_Y 16
#define NON_SELECTED_POKEMON_STATUS_X 28
#define NON_SELECTED_POKEMON_STATUS_Y 24

// Selected slot is drawn offset from unselected 
#define SELECTED_POKEMON_DIFFERENCE_Y (-3)
#define SELECTED_POKEMON_DIFFERENCE_X (-1)

// Party menu (continue / cancel)
#define continue_x 201
#define continue_y 165
#define CANCEL_X (continue_x - 1)
#define CANCEL_Y (continue_y - 1)

#endif

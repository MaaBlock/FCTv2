#pragma once

namespace FCT {

enum KeyCode : unsigned int {
    KC_NONE = 0,
    KC_ESCAPE = 256,
    KC_ENTER = 257,
    KC_TAB = 258,
    KC_BACKSPACE = 259,
    KC_INSERT = 260,
    KC_DELETE = 261,
    KC_RIGHT = 262,
    KC_LEFT = 263,
    KC_DOWN = 264,
    KC_UP = 265,
    KC_PAGE_UP = 266,
    KC_PAGE_DOWN = 267,
    KC_HOME = 268,
    KC_END = 269,

    KC_A = 65,
    KC_B = 66,
    KC_C = 67,
    KC_D = 68,
    KC_E = 69,
    KC_F = 70,
    KC_G = 71,
    KC_H = 72,
    KC_I = 73,
    KC_J = 74,
    KC_K = 75,
    KC_L = 76,
    KC_M = 77,
    KC_N = 78,
    KC_O = 79,
    KC_P = 80,
    KC_Q = 81,
    KC_R = 82,
    KC_S = 83,
    KC_T = 84,
    KC_U = 85,
    KC_V = 86,
    KC_W = 87,
    KC_X = 88,
    KC_Y = 89,
    KC_Z = 90,

    KC_0 = 48,
    KC_1 = 49,
    KC_2 = 50,
    KC_3 = 51,
    KC_4 = 52,
    KC_5 = 53,
    KC_6 = 54,
    KC_7 = 55,
    KC_8 = 56,
    KC_9 = 57,

    KC_F1 = 290,
    KC_F2 = 291,
    KC_F3 = 292,
    KC_F4 = 293,
    KC_F5 = 294,
    KC_F6 = 295,
    KC_F7 = 296,
    KC_F8 = 297,
    KC_F9 = 298,
    KC_F10 = 299,
    KC_F11 = 300,
    KC_F12 = 301,

    KC_LEFT_SHIFT = 340,
    KC_RIGHT_SHIFT = 344,
    KC_LEFT_CONTROL = 341,
    KC_RIGHT_CONTROL = 345,
    KC_LEFT_ALT = 342,
    KC_RIGHT_ALT = 346,
    KC_LEFT_SUPER = 343,
    KC_RIGHT_SUPER = 347,

    KC_NUMPAD_0 = 320,
    KC_NUMPAD_1 = 321,
    KC_NUMPAD_2 = 322,
    KC_NUMPAD_3 = 323,
    KC_NUMPAD_4 = 324,
    KC_NUMPAD_5 = 325,
    KC_NUMPAD_6 = 326,
    KC_NUMPAD_7 = 327,
    KC_NUMPAD_8 = 328,
    KC_NUMPAD_9 = 329,
    KC_NUMPAD_DECIMAL = 330,
    KC_NUMPAD_DIVIDE = 331,
    KC_NUMPAD_MULTIPLY = 332,
    KC_NUMPAD_SUBTRACT = 333,
    KC_NUMPAD_ADD = 334,
    KC_NUMPAD_ENTER = 335, 
    KC_NUMPAD_EQUAL = 336,

    KC_SPACE = 32,
    KC_APOSTROPHE = 39,  // '
    KC_COMMA = 44,       // ,
    KC_MINUS = 45,       // -
    KC_PERIOD = 46,      // .
    KC_SLASH = 47,       // /
    KC_SEMICOLON = 59,   // ;
    KC_EQUAL = 61,       // =
    KC_LEFT_BRACKET = 91, // [
    KC_BACKSLASH = 92,   // \ 
    KC_RIGHT_BRACKET = 93,// ]
    KC_GRAVE_ACCENT = 96, // `


    KC_KEY_COUNT
};

} // namespace FCT

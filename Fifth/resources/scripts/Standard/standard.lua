-- standard.lua

math.randomseed(os.time())

CollisionLayers = {
    LAYER0      = 1,
    LAYER1      = 2,
    LAYER2      = 4,
    LAYER3      = 8,
    LAYER4      = 16,
    LAYER5      = 32,
    LAYER6      = 64,
    LAYER7      = 128
}

EntityProperty = {
    COLLIDABLE   = 1,
    GRAVITY_AFFECT = 2,
    HIDDEN       = 4,
    STATIC       = 8,
    FLIP         = 16,
    FLIP_FREEZED = 32
}

SpriteStateTypes = {
    IDLE        = 0,
    ASCENDING   = 1,
    DESCENDING  = 2,
    FLYING      = 3,
    WALKING     = 4,
    SNEAKING    = 5,
    RUNNING     = 6,
    DUCKING     = 7,
    TOTAL_SPRITESTATETYPES = 8
}

ChatBubbleType = {
    SAY = 0,
    YELL = 1,
    WHISPER = 2,
    INSTANT_TALK = 3
}

RenderFlags = {
    CLEAR = 0,
    COLLISION_BORDERS   = 1,
    COLLISION_AREA      = 2,
    COLLISION_GRID      = 4,
    COLLISION_FULL_AREA = 8,
    ENTITY_GRID         = 16,
    RESERVED_FLAG       = 32,
    RESERVED_FLAG_2     = 64,
    RENDER_COMBAT_TEXT  = 128
}

LogType = {
    SUCCESS = 0,
    ERROR   = 1,
    WARNING = 2,
    ALERT   = 3
}

KeyCode = {
    _UNKNOWN = 0,

    _RETURN = 13,
    _ESCAPE = 27,
    _BACKSPACE = 8,
    _TAB = 9,
    _SPACE = 32,
    _EXCLAIM = 33,
    _QUOTEDBL = 34,
    _HASH = 35,
    _PERCENT = 37,
    _DOLLAR = 36,
    _AMPERSAND = 38,
    _QUOTE = 39,
    _LEFTPAREN = 40,
    _RIGHTPAREN = 41,
    _ASTERISK = 42,
    _PLUS = 43,
    _COMMA = 44,
    _MINUS = 45,
    _PERIOD = 46,
    _SLASH = 47,
    _0 = 48,
    _1 = 49,
    _2 = 50,
    _3 = 51,
    _4 = 52,
    _5 = 53,
    _6 = 54,
    _7 = 55,
    _8 = 56,
    _9 = 57,
    _COLON = 58,
    _SEMICOLON = 59,
    _LESS = 60,
    _EQUALS = 61,
    _GREATER = 62,
    _QUESTION = 63,
    _AT = 64,

    _LEFTBRACKET = 91,
    _BACKSLASH = 92,
    _RIGHTBRACKET = 93,
    _CARET = 94,
    _UNDERSCORE = 95,
    _BACKQUOTE = 96,
    _a = 97,
    _b = 98,
    _c = 99,
    _d = 100,
    _e = 101,
    _f = 102,
    _g = 103,
    _h = 104,
    _i = 105,
    _j = 106,
    _k = 107,
    _l = 108,
    _m = 109,
    _n = 110,
    _o = 111,
    _p = 112,
    _q = 113,
    _r = 114,
    _s = 115,
    _t = 116,
    _u = 117,
    _v = 118,
    _w = 119,
    _x = 120,
    _y = 121,
    _z = 122,
    _ou = 246,
    _ao = 228,
    _uo = 229,

    _PARA = 167,
    _ARROW_UP = 1073741906,
    _ARROW_DOWN = 1073741905,

    LEFTMOUSEDOWN = "LEFTMOUSEDOWN",
    RIGHTMOUSEDOWN = "RIGHTMOUSEDOWN"
}

ScanCode = {
    _UNKNOWN = 0,

    _A = 4,
    _B = 5,
    _C = 6,
    _D = 7,
    _E = 8,
    _F = 9,
    _G = 10,
    _H = 11,
    _I = 12,
    _J = 13,
    _K = 14,
    _L = 15,
    _M = 16,
    _N = 17,
    _O = 18,
    _P = 19,
    _Q = 20,
    _R = 21,
    _S = 22,
    _T = 23,
    _U = 24,
    _V = 25,
    _W = 26,
    _X = 27,
    _Y = 28,
    _Z = 29,

    _1 = 30,
    _2 = 31,
    _3 = 32,
    _4 = 33,
    _5 = 34,
    _6 = 35,
    _7 = 36,
    _8 = 37,
    _9 = 38,
    _0 = 39,

    _RETURN = 40,
    _ESCAPE = 41,
    _BACKSPACE = 42,
    _TAB = 43,
    _SPACE = 44,

    _MINUS = 45,
    _EQUALS = 46,
    _LEFTBRACKET = 47,
    _RIGHTBRACKET = 48,
    _BACKSLASH = 49,

    _NONUSHASH = 50,
    _SEMICOLON = 51,
    _APOSTROPHE = 52,

    _GRAVE = 53,
    _COMMA = 54,
    _PERIOD = 55,
    _SLASH = 56,

    _CAPSLOCK = 57,

    _F1 = 58,
    _F2 = 59,
    _F3 = 60,
    _F4 = 61,
    _F5 = 62,
    _F6 = 63,
    _F7 = 64,
    _F8 = 65,
    _F9 = 66,
    _F10 = 67,
    _F11 = 68,
    _F12 = 69,

    _PRINTSCREEN = 70,
    _SCROLLLOCK = 71,
    _PAUSE = 72,
    _INSERT = 73,
    _HOME = 74,
    _PAGEUP = 75,
    _DELETE = 76,
    _END = 77,
    _PAGEDOWN = 78,
    _RIGHT = 79,
    _LEFT = 80,
    _DOWN = 81,
    _UP = 82,

    _NUMLOCKCLEAR = 83,
    _KP_DIVIDE = 84,
    _KP_MULTIPLY = 85,
    _KP_MINUS = 86,
    _KP_PLUS = 87,
    _KP_ENTER = 88,
    _KP_1 = 89,
    _KP_2 = 90,
    _KP_3 = 91,
    _KP_4 = 92,
    _KP_5 = 93,
    _KP_6 = 94,
    _KP_7 = 95,
    _KP_8 = 96,
    _KP_9 = 97,
    _KP_0 = 98,
    _KP_PERIOD = 99,

    _NONUSBACKSLASH = 100,
    _APPLICATION = 101,
    _POWER = 102,
    _KP_EQUALS = 103,
    _F13 = 104,
    _F14 = 105,
    _F15 = 106,
    _F16 = 107,
    _F17 = 108,
    _F18 = 109,
    _F19 = 110,
    _F20 = 111,
    _F21 = 112,
    _F22 = 113,
    _F23 = 114,
    _F24 = 115,
    _EXECUTE = 116,
    _HELP = 117,
    _MENU = 118,
    _SELECT = 119,
    _STOP = 120,
    _AGAIN = 121,
    _UNDO = 122,
    _CUT = 123,
    _COPY = 124,
    _PASTE = 125,
    _FIND = 126,
    _MUTE = 127,
    _VOLUMEUP = 128,
    _VOLUMEDOWN = 129,

    _KP_COMMA = 133,
    _KP_EQUALSAS400 = 134,

    _INTERNATIONAL1 = 135,
    _INTERNATIONAL2 = 136,
    _INTERNATIONAL3 = 137,
    _INTERNATIONAL4 = 138,
    _INTERNATIONAL5 = 139,
    _INTERNATIONAL6 = 140,
    _INTERNATIONAL7 = 141,
    _INTERNATIONAL8 = 142,
    _INTERNATIONAL9 = 143,
    _LANG1 = 144,
    _LANG2 = 145,
    _LANG3 = 146,
    _LANG4 = 147,
    _LANG5 = 148,
    _LANG6 = 149,
    _LANG7 = 150,
    _LANG8 = 151,
    _LANG9 = 152,

    _ALTERASE = 153,
    _SYSREQ = 154,
    _CANCEL = 155,
    _CLEAR = 156,
    _PRIOR = 157,
    _RETURN2 = 158,
    _SEPARATOR = 159,
    _OUT = 160,
    _OPER = 161,
    _CLEARAGAIN = 162,
    _CRSEL = 163,
    _EXSEL = 164,

    _KP_00 = 176,
    _KP_000 = 177,
    _THOUSANDSSEPARATOR = 178,
    _DECIMALSEPARATOR = 179,
    _CURRENCYUNIT = 180,
    _CURRENCYSUBUNIT = 181,
    _KP_LEFTPAREN = 182,
    _KP_RIGHTPAREN = 183,
    _KP_LEFTBRACE = 184,
    _KP_RIGHTBRACE = 185,
    _KP_TAB = 186,
    _KP_BACKSPACE = 187,
    _KP_A = 188,
    _KP_B = 189,
    _KP_C = 190,
    _KP_D = 191,
    _KP_E = 192,
    _KP_F = 193,
    _KP_XOR = 194,
    _KP_POWER = 195,
    _KP_PERCENT = 196,
    _KP_LESS = 197,
    _KP_GREATER = 198,
    _KP_AMPERSAND = 199,
    _KP_DBLAMPERSAND = 200,
    _KP_VERTICALBAR = 201,
    _KP_DBLVERTICALBAR = 202,
    _KP_COLON = 203,
    _KP_HASH = 204,
    _KP_SPACE = 205,
    _KP_AT = 206,
    _KP_EXCLAM = 207,
    _KP_MEMSTORE = 208,
    _KP_MEMRECALL = 209,
    _KP_MEMCLEAR = 210,
    _KP_MEMADD = 211,
    _KP_MEMSUBTRACT = 212,
    _KP_MEMMULTIPLY = 213,
    _KP_MEMDIVIDE = 214,
    _KP_PLUSMINUS = 215,
    _KP_CLEAR = 216,
    _KP_CLEARENTRY = 217,
    _KP_BINARY = 218,
    _KP_OCTAL = 219,
    _KP_DECIMAL = 220,
    _KP_HEXADECIMAL = 221,

    _LCTRL = 224,
    _LSHIFT = 225,
    _LALT = 226,
    _LGUI = 227,
    _RCTRL = 228,
    _RSHIFT = 229,
    _RALT = 230,
    _RGUI = 231,

    _MODE = 257,

    _AUDIONEXT = 258,
    _AUDIOPREV = 259,
    _AUDIOSTOP = 260,
    _AUDIOPLAY = 261,
    _AUDIOMUTE = 262,
    _MEDIASELECT = 263,
    _WWW = 264,
    _MAIL = 265,
    _CALCULATOR = 266,
    _COMPUTER = 267,
    _AC_SEARCH = 268,
    _AC_HOME = 269,
    _AC_BACK = 270,
    _AC_FORWARD = 271,
    _AC_STOP = 272,
    _AC_REFRESH = 273,
    _AC_BOOKMARKS = 274,

    _BRIGHTNESSDOWN = 275,
    _BRIGHTNESSUP = 276,
    _DISPLAYSWITCH = 277,
    _KBDILLUMTOGGLE = 278,
    _KBDILLUMDOWN = 279,
    _KBDILLUMUP = 280,
    _EJECT = 281,
    _SLEEP = 282,

    _APP1 = 283,
    _APP2 = 284
}

function toKeyCode(code)
    return BitOR(code, math.pow(2, 30))
end

json = nil
perlin = nil

function init(path)
    game:setLuaPath(path)
end

function requireModule(path, mod)
    oldPath = package.path
    package.path = package.path .. path
    toReturn = require(mod)
    package.path = oldPath
    return toReturn
end

function initPackages()
    _G.json = requireModule("json.lua", "json")
    _G.perlin = requireModule("perlin.lua", "perlin")
end

function getVal(var, val)
    return (val ~= nil and val or var)
end

function Autotable(dim)
    local MT = {};
        for i=1, dim do
            MT[i] = {__index = function(t, k)
                if i < dim then
                    t[k] = setmetatable({}, MT[i+1])
                    return t[k];
                end
            end}
    end
    return setmetatable({}, MT[1]);
end

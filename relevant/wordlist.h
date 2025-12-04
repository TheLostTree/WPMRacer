
#include <stdint.h>
#include "random.h"
#include "avr/pgmspace.h"
#include <string.h> 
#include "serial.h"

#pragma region Words

// top 255 words from monkeytype 1k wordlist
const char wordlist_0[] PROGMEM = "the";
const char wordlist_1[] PROGMEM = "of";
const char wordlist_2[] PROGMEM = "to";
const char wordlist_3[] PROGMEM = "and";
const char wordlist_4[] PROGMEM = "a";
const char wordlist_5[] PROGMEM = "in";
const char wordlist_6[] PROGMEM = "is";
const char wordlist_7[] PROGMEM = "it";
const char wordlist_8[] PROGMEM = "you";
const char wordlist_9[] PROGMEM = "that";
const char wordlist_10[] PROGMEM = "he";
const char wordlist_11[] PROGMEM = "was";
const char wordlist_12[] PROGMEM = "for";
const char wordlist_13[] PROGMEM = "on";
const char wordlist_14[] PROGMEM = "are";
const char wordlist_15[] PROGMEM = "with";
const char wordlist_16[] PROGMEM = "as";
const char wordlist_17[] PROGMEM = "I";
const char wordlist_18[] PROGMEM = "his";
const char wordlist_19[] PROGMEM = "they";
const char wordlist_20[] PROGMEM = "be";
const char wordlist_21[] PROGMEM = "at";
const char wordlist_22[] PROGMEM = "one";
const char wordlist_23[] PROGMEM = "have";
const char wordlist_24[] PROGMEM = "this";
const char wordlist_25[] PROGMEM = "from";
const char wordlist_26[] PROGMEM = "or";
const char wordlist_27[] PROGMEM = "had";
const char wordlist_28[] PROGMEM = "by";
const char wordlist_29[] PROGMEM = "not";
const char wordlist_30[] PROGMEM = "word";
const char wordlist_31[] PROGMEM = "but";
const char wordlist_32[] PROGMEM = "what";
const char wordlist_33[] PROGMEM = "some";
const char wordlist_34[] PROGMEM = "we";
const char wordlist_35[] PROGMEM = "can";
const char wordlist_36[] PROGMEM = "out";
const char wordlist_37[] PROGMEM = "other";
const char wordlist_38[] PROGMEM = "were";
const char wordlist_39[] PROGMEM = "all";
const char wordlist_40[] PROGMEM = "there";
const char wordlist_41[] PROGMEM = "when";
const char wordlist_42[] PROGMEM = "up";
const char wordlist_43[] PROGMEM = "use";
const char wordlist_44[] PROGMEM = "your";
const char wordlist_45[] PROGMEM = "how";
const char wordlist_46[] PROGMEM = "said";
const char wordlist_47[] PROGMEM = "an";
const char wordlist_48[] PROGMEM = "each";
const char wordlist_49[] PROGMEM = "she";
const char wordlist_50[] PROGMEM = "which";
const char wordlist_51[] PROGMEM = "do";
const char wordlist_52[] PROGMEM = "their";
const char wordlist_53[] PROGMEM = "time";
const char wordlist_54[] PROGMEM = "if";
const char wordlist_55[] PROGMEM = "will";
const char wordlist_56[] PROGMEM = "way";
const char wordlist_57[] PROGMEM = "about";
const char wordlist_58[] PROGMEM = "many";
const char wordlist_59[] PROGMEM = "then";
const char wordlist_60[] PROGMEM = "them";
const char wordlist_61[] PROGMEM = "write";
const char wordlist_62[] PROGMEM = "would";
const char wordlist_63[] PROGMEM = "like";
const char wordlist_64[] PROGMEM = "so";
const char wordlist_65[] PROGMEM = "these";
const char wordlist_66[] PROGMEM = "her";
const char wordlist_67[] PROGMEM = "long";
const char wordlist_68[] PROGMEM = "make";
const char wordlist_69[] PROGMEM = "thing";
const char wordlist_70[] PROGMEM = "see";
const char wordlist_71[] PROGMEM = "him";
const char wordlist_72[] PROGMEM = "two";
const char wordlist_73[] PROGMEM = "has";
const char wordlist_74[] PROGMEM = "look";
const char wordlist_75[] PROGMEM = "more";
const char wordlist_76[] PROGMEM = "day";
const char wordlist_77[] PROGMEM = "could";
const char wordlist_78[] PROGMEM = "go";
const char wordlist_79[] PROGMEM = "come";
const char wordlist_80[] PROGMEM = "did";
const char wordlist_81[] PROGMEM = "number";
const char wordlist_82[] PROGMEM = "sound";
const char wordlist_83[] PROGMEM = "no";
const char wordlist_84[] PROGMEM = "most";
const char wordlist_85[] PROGMEM = "people";
const char wordlist_86[] PROGMEM = "my";
const char wordlist_87[] PROGMEM = "over";
const char wordlist_88[] PROGMEM = "know";
const char wordlist_89[] PROGMEM = "water";
const char wordlist_90[] PROGMEM = "than";
const char wordlist_91[] PROGMEM = "call";
const char wordlist_92[] PROGMEM = "first";
const char wordlist_93[] PROGMEM = "who";
const char wordlist_94[] PROGMEM = "may";
const char wordlist_95[] PROGMEM = "down";
const char wordlist_96[] PROGMEM = "side";
const char wordlist_97[] PROGMEM = "been";
const char wordlist_98[] PROGMEM = "now";
const char wordlist_99[] PROGMEM = "find";
const char wordlist_100[] PROGMEM = "any";
const char wordlist_101[] PROGMEM = "new";
const char wordlist_102[] PROGMEM = "work";
const char wordlist_103[] PROGMEM = "part";
const char wordlist_104[] PROGMEM = "take";
const char wordlist_105[] PROGMEM = "get";
const char wordlist_106[] PROGMEM = "place";
const char wordlist_107[] PROGMEM = "made";
const char wordlist_108[] PROGMEM = "live";
const char wordlist_109[] PROGMEM = "where";
const char wordlist_110[] PROGMEM = "after";
const char wordlist_111[] PROGMEM = "back";
const char wordlist_112[] PROGMEM = "little";
const char wordlist_113[] PROGMEM = "only";
const char wordlist_114[] PROGMEM = "round";
const char wordlist_115[] PROGMEM = "man";
const char wordlist_116[] PROGMEM = "year";
const char wordlist_117[] PROGMEM = "came";
const char wordlist_118[] PROGMEM = "show";
const char wordlist_119[] PROGMEM = "every";
const char wordlist_120[] PROGMEM = "good";
const char wordlist_121[] PROGMEM = "me";
const char wordlist_122[] PROGMEM = "give";
const char wordlist_123[] PROGMEM = "our";
const char wordlist_124[] PROGMEM = "under";
const char wordlist_125[] PROGMEM = "name";
const char wordlist_126[] PROGMEM = "very";
const char wordlist_127[] PROGMEM = "through";
const char wordlist_128[] PROGMEM = "just";
const char wordlist_129[] PROGMEM = "form";
const char wordlist_130[] PROGMEM = "sentence";
const char wordlist_131[] PROGMEM = "great";
const char wordlist_132[] PROGMEM = "think";
const char wordlist_133[] PROGMEM = "say";
const char wordlist_134[] PROGMEM = "help";
const char wordlist_135[] PROGMEM = "low";
const char wordlist_136[] PROGMEM = "line";
const char wordlist_137[] PROGMEM = "differ";
const char wordlist_138[] PROGMEM = "turn";
const char wordlist_139[] PROGMEM = "cause";
const char wordlist_140[] PROGMEM = "much";
const char wordlist_141[] PROGMEM = "mean";
const char wordlist_142[] PROGMEM = "before";
const char wordlist_143[] PROGMEM = "move";
const char wordlist_144[] PROGMEM = "right";
const char wordlist_145[] PROGMEM = "boy";
const char wordlist_146[] PROGMEM = "old";
const char wordlist_147[] PROGMEM = "too";
const char wordlist_148[] PROGMEM = "same";
const char wordlist_149[] PROGMEM = "tell";
const char wordlist_150[] PROGMEM = "does";
const char wordlist_151[] PROGMEM = "set";
const char wordlist_152[] PROGMEM = "three";
const char wordlist_153[] PROGMEM = "want";
const char wordlist_154[] PROGMEM = "air";
const char wordlist_155[] PROGMEM = "well";
const char wordlist_156[] PROGMEM = "also";
const char wordlist_157[] PROGMEM = "play";
const char wordlist_158[] PROGMEM = "small";
const char wordlist_159[] PROGMEM = "end";
const char wordlist_160[] PROGMEM = "put";
const char wordlist_161[] PROGMEM = "home";
const char wordlist_162[] PROGMEM = "read";
const char wordlist_163[] PROGMEM = "hand";
const char wordlist_164[] PROGMEM = "port";
const char wordlist_165[] PROGMEM = "large";
const char wordlist_166[] PROGMEM = "spell";
const char wordlist_167[] PROGMEM = "add";
const char wordlist_168[] PROGMEM = "even";
const char wordlist_169[] PROGMEM = "land";
const char wordlist_170[] PROGMEM = "here";
const char wordlist_171[] PROGMEM = "must";
const char wordlist_172[] PROGMEM = "big";
const char wordlist_173[] PROGMEM = "high";
const char wordlist_174[] PROGMEM = "such";
const char wordlist_175[] PROGMEM = "follow";
const char wordlist_176[] PROGMEM = "act";
const char wordlist_177[] PROGMEM = "why";
const char wordlist_178[] PROGMEM = "ask";
const char wordlist_179[] PROGMEM = "men";
const char wordlist_180[] PROGMEM = "change";
const char wordlist_181[] PROGMEM = "went";
const char wordlist_182[] PROGMEM = "light";
const char wordlist_183[] PROGMEM = "kind";
const char wordlist_184[] PROGMEM = "off";
const char wordlist_185[] PROGMEM = "need";
const char wordlist_186[] PROGMEM = "house";
const char wordlist_187[] PROGMEM = "picture";
const char wordlist_188[] PROGMEM = "try";
const char wordlist_189[] PROGMEM = "us";
const char wordlist_190[] PROGMEM = "again";
const char wordlist_191[] PROGMEM = "animal";
const char wordlist_192[] PROGMEM = "point";
const char wordlist_193[] PROGMEM = "mother";
const char wordlist_194[] PROGMEM = "world";
const char wordlist_195[] PROGMEM = "near";
const char wordlist_196[] PROGMEM = "build";
const char wordlist_197[] PROGMEM = "self";
const char wordlist_198[] PROGMEM = "earth";
const char wordlist_199[] PROGMEM = "father";
const char wordlist_200[] PROGMEM = "head";
const char wordlist_201[] PROGMEM = "stand";
const char wordlist_202[] PROGMEM = "own";
const char wordlist_203[] PROGMEM = "page";
const char wordlist_204[] PROGMEM = "should";
const char wordlist_205[] PROGMEM = "country";
const char wordlist_206[] PROGMEM = "found";
const char wordlist_207[] PROGMEM = "answer";
const char wordlist_208[] PROGMEM = "school";
const char wordlist_209[] PROGMEM = "grow";
const char wordlist_210[] PROGMEM = "study";
const char wordlist_211[] PROGMEM = "still";
const char wordlist_212[] PROGMEM = "learn";
const char wordlist_213[] PROGMEM = "plant";
const char wordlist_214[] PROGMEM = "cover";
const char wordlist_215[] PROGMEM = "food";
const char wordlist_216[] PROGMEM = "sun";
const char wordlist_217[] PROGMEM = "four";
const char wordlist_218[] PROGMEM = "between";
const char wordlist_219[] PROGMEM = "state";
const char wordlist_220[] PROGMEM = "keep";
const char wordlist_221[] PROGMEM = "eye";
const char wordlist_222[] PROGMEM = "never";
const char wordlist_223[] PROGMEM = "last";
const char wordlist_224[] PROGMEM = "let";
const char wordlist_225[] PROGMEM = "thought";
const char wordlist_226[] PROGMEM = "city";
const char wordlist_227[] PROGMEM = "tree";
const char wordlist_228[] PROGMEM = "cross";
const char wordlist_229[] PROGMEM = "farm";
const char wordlist_230[] PROGMEM = "hard";
const char wordlist_231[] PROGMEM = "start";
const char wordlist_232[] PROGMEM = "might";
const char wordlist_233[] PROGMEM = "story";
const char wordlist_234[] PROGMEM = "saw";
const char wordlist_235[] PROGMEM = "far";
const char wordlist_236[] PROGMEM = "sea";
const char wordlist_237[] PROGMEM = "draw";
const char wordlist_238[] PROGMEM = "left";
const char wordlist_239[] PROGMEM = "late";
const char wordlist_240[] PROGMEM = "run";
const char wordlist_241[] PROGMEM = "while";
const char wordlist_242[] PROGMEM = "press";
const char wordlist_243[] PROGMEM = "close";
const char wordlist_244[] PROGMEM = "night";
const char wordlist_245[] PROGMEM = "real";
const char wordlist_246[] PROGMEM = "life";
const char wordlist_247[] PROGMEM = "few";
const char wordlist_248[] PROGMEM = "north";
const char wordlist_249[] PROGMEM = "open";
const char wordlist_250[] PROGMEM = "seem";
const char wordlist_251[] PROGMEM = "together";
const char wordlist_252[] PROGMEM = "next";
const char wordlist_253[] PROGMEM = "white";
const char wordlist_254[] PROGMEM = "children";
const char wordlist_255[] PROGMEM = "";
PGM_P const wordlist[] PROGMEM = {
	wordlist_0,
	wordlist_1,
	wordlist_2,
	wordlist_3,
	wordlist_4,
	wordlist_5,
	wordlist_6,
	wordlist_7,
	wordlist_8,
	wordlist_9,
	wordlist_10,
	wordlist_11,
	wordlist_12,
	wordlist_13,
	wordlist_14,
	wordlist_15,
	wordlist_16,
	wordlist_17,
	wordlist_18,
	wordlist_19,
	wordlist_20,
	wordlist_21,
	wordlist_22,
	wordlist_23,
	wordlist_24,
	wordlist_25,
	wordlist_26,
	wordlist_27,
	wordlist_28,
	wordlist_29,
	wordlist_30,
	wordlist_31,
	wordlist_32,
	wordlist_33,
	wordlist_34,
	wordlist_35,
	wordlist_36,
	wordlist_37,
	wordlist_38,
	wordlist_39,
	wordlist_40,
	wordlist_41,
	wordlist_42,
	wordlist_43,
	wordlist_44,
	wordlist_45,
	wordlist_46,
	wordlist_47,
	wordlist_48,
	wordlist_49,
	wordlist_50,
	wordlist_51,
	wordlist_52,
	wordlist_53,
	wordlist_54,
	wordlist_55,
	wordlist_56,
	wordlist_57,
	wordlist_58,
	wordlist_59,
	wordlist_60,
	wordlist_61,
	wordlist_62,
	wordlist_63,
	wordlist_64,
	wordlist_65,
	wordlist_66,
	wordlist_67,
	wordlist_68,
	wordlist_69,
	wordlist_70,
	wordlist_71,
	wordlist_72,
	wordlist_73,
	wordlist_74,
	wordlist_75,
	wordlist_76,
	wordlist_77,
	wordlist_78,
	wordlist_79,
	wordlist_80,
	wordlist_81,
	wordlist_82,
	wordlist_83,
	wordlist_84,
	wordlist_85,
	wordlist_86,
	wordlist_87,
	wordlist_88,
	wordlist_89,
	wordlist_90,
	wordlist_91,
	wordlist_92,
	wordlist_93,
	wordlist_94,
	wordlist_95,
	wordlist_96,
	wordlist_97,
	wordlist_98,
	wordlist_99,
	wordlist_100,
	wordlist_101,
	wordlist_102,
	wordlist_103,
	wordlist_104,
	wordlist_105,
	wordlist_106,
	wordlist_107,
	wordlist_108,
	wordlist_109,
	wordlist_110,
	wordlist_111,
	wordlist_112,
	wordlist_113,
	wordlist_114,
	wordlist_115,
	wordlist_116,
	wordlist_117,
	wordlist_118,
	wordlist_119,
	wordlist_120,
	wordlist_121,
	wordlist_122,
	wordlist_123,
	wordlist_124,
	wordlist_125,
	wordlist_126,
	wordlist_127,
	wordlist_128,
	wordlist_129,
	wordlist_130,
	wordlist_131,
	wordlist_132,
	wordlist_133,
	wordlist_134,
	wordlist_135,
	wordlist_136,
	wordlist_137,
	wordlist_138,
	wordlist_139,
	wordlist_140,
	wordlist_141,
	wordlist_142,
	wordlist_143,
	wordlist_144,
	wordlist_145,
	wordlist_146,
	wordlist_147,
	wordlist_148,
	wordlist_149,
	wordlist_150,
	wordlist_151,
	wordlist_152,
	wordlist_153,
	wordlist_154,
	wordlist_155,
	wordlist_156,
	wordlist_157,
	wordlist_158,
	wordlist_159,
	wordlist_160,
	wordlist_161,
	wordlist_162,
	wordlist_163,
	wordlist_164,
	wordlist_165,
	wordlist_166,
	wordlist_167,
	wordlist_168,
	wordlist_169,
	wordlist_170,
	wordlist_171,
	wordlist_172,
	wordlist_173,
	wordlist_174,
	wordlist_175,
	wordlist_176,
	wordlist_177,
	wordlist_178,
	wordlist_179,
	wordlist_180,
	wordlist_181,
	wordlist_182,
	wordlist_183,
	wordlist_184,
	wordlist_185,
	wordlist_186,
	wordlist_187,
	wordlist_188,
	wordlist_189,
	wordlist_190,
	wordlist_191,
	wordlist_192,
	wordlist_193,
	wordlist_194,
	wordlist_195,
	wordlist_196,
	wordlist_197,
	wordlist_198,
	wordlist_199,
	wordlist_200,
	wordlist_201,
	wordlist_202,
	wordlist_203,
	wordlist_204,
	wordlist_205,
	wordlist_206,
	wordlist_207,
	wordlist_208,
	wordlist_209,
	wordlist_210,
	wordlist_211,
	wordlist_212,
	wordlist_213,
	wordlist_214,
	wordlist_215,
	wordlist_216,
	wordlist_217,
	wordlist_218,
	wordlist_219,
	wordlist_220,
	wordlist_221,
	wordlist_222,
	wordlist_223,
	wordlist_224,
	wordlist_225,
	wordlist_226,
	wordlist_227,
	wordlist_228,
	wordlist_229,
	wordlist_230,
	wordlist_231,
	wordlist_232,
	wordlist_233,
	wordlist_234,
	wordlist_235,
	wordlist_236,
	wordlist_237,
	wordlist_238,
	wordlist_239,
	wordlist_240,
	wordlist_241,
	wordlist_242,
	wordlist_243,
	wordlist_244,
	wordlist_245,
	wordlist_246,
	wordlist_247,
	wordlist_248,
	wordlist_249,
	wordlist_250,
	wordlist_251,
	wordlist_252,
	wordlist_253,
	wordlist_254,
	wordlist_255,
};
#pragma endregion


uint16_t get_word(uint8_t idx, char * buffer, size_t buffer_size){
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(wordlist[idx])));
}

// tee hee constepr (i want to fit as much of the wordlist as possible into the avr 2k flash)
// constexpr uint16_t WORDLIST_SIZE = sizeof(wordlist) / sizeof(wordlist[0]);

const char* get_next_word(xorshift32* rng){
    uint32_t r = next_rand32(rng);
    uint16_t index = r & 0xFF; //256
    return wordlist[index];
}
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <utility>
#include <set>
#include <array>
#ifndef CS241_SCANNER_H
#define CS241_SCANNER_H
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include <ostream>
#include <iostream>

/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains helpers for asm.cc and should not need to be modified by
 * you. However, its comments contain important information and you should
 * read the entire file before you start modifying asm.cc.
 */

class Token;

/* Scans a single line of input and produces a list of tokens.
 *
 * Scan returns tokens with the following kinds:
 * ID: identifiers and keywords.
 * LABEL: labels (identifiers ending in a colon).
 * WORD: the special ".word" keyword.
 * COMMA: a comma.
 * LPAREN: a left parenthesis.
 * RPAREN: a right parenthesis.
 * INT: a signed or unsigned 32-bit integer written in decimal.
 * HEXINT: an unsigned 32-bit integer written in hexadecimal.
 * REG: a register between $0 and $31.
 */

std::vector<Token> scan(const std::string &input);

/* A scanned token produced by the scanner.
 * The "kind" tells us what kind of token it is
 * while the "lexeme" tells us exactly what text
 * the programmer typed. For example, the token
 * "abc" might have kind "ID" and lexeme "abc".
 *
 * While you can create tokens with any kind and
 * lexeme, the list of kinds produced by the
 * starter code can be found in the documentation
 * for scan above.
 */
class Token {
public:
    enum Kind {
        ID = 0,
        NUM,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        RETURN,
        IF,
        ELSE,
        WHILE,
        PRINTLN,
        WAIN,
        BECOMES,
        INT,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PCT,
        COMMA,
        SEMI,
        NEW,
        DELETE,
        LBRACK,
        RBRACK,
        AMP,
        null,
        COMMENT,
        WHITESPACE
    };
    
private:
    Kind kind;
    std::string lexeme;
    
public:
    Token(Kind kind, std::string lexeme);
    
    Kind getKind() const;
    const std::string &getLexeme() const;
    
    /* Only works on tokens of type INT, HEXINT, or REG
     *
     * Although all results will be integers, this function
     * returns a long since the result may be either signed
     * or unsigned, and thus may lie anywhere in the range
     * -2147483648 .. 4294967295
     */
    int64_t toLong() const;
    
};

std::ostream &operator<<(std::ostream &out, const Token &tok);

/* An exception class thrown when an error is encountered while scanning.
 */
class ScanningFailure {
    std::string message;
    
public:
    ScanningFailure(std::string message);
    
    // Returns the message associated with the exception.
    const std::string &what() const;
};

#endif



Token::Token(Token::Kind kind, std::string lexeme):
kind(kind), lexeme(std::move(lexeme)) {}

Token:: Kind Token::getKind() const { return kind; }
const std::string &Token::getLexeme() const { return lexeme; }

std::ostream &operator<<(std::ostream &out, const Token &tok) {
    switch (tok.getKind()) {
        case Token::ID:         out << "ID";         break;
        case Token::NUM:        out << "NUM";        break;
        case Token::LPAREN:     out << "LPAREN";     break;
        case Token::RPAREN:     out << "RPAREN";     break;
        case Token::LBRACE:     out << "LBRACE";     break;
        case Token::RBRACE:     out << "RBRACE";     break;
        case Token::RETURN:     out << "RETURN";     break;
        case Token::IF:         out << "IF";         break;
        case Token::ELSE:       out << "ELSE";       break;
        case Token::WHILE:      out << "WHILE";      break;
        case Token::PRINTLN:    out << "PRINTLN";    break;
        case Token::WAIN:       out << "WAIN";       break;
        case Token::BECOMES:    out << "BECOMES";    break;
        case Token::INT:        out << "INT";        break;
        case Token::EQ:         out << "EQ";         break;
        case Token::NE:         out << "NE";         break;
        case Token::LT:         out << "LT";         break;
        case Token::GT:         out << "GT";         break;
        case Token::LE:         out << "LE";         break;
        case Token::GE:         out << "GE";         break;
        case Token::PLUS:       out << "PLUS";       break;
        case Token::MINUS:      out << "MINUS";      break;
        case Token::STAR:       out << "STAR";       break;
        case Token::SLASH:      out << "SLASH";      break;
        case Token::PCT:        out << "PCT";        break;
        case Token::COMMA:      out << "COMMA";      break;
        case Token::SEMI:       out << "SEMI";       break;
        case Token::NEW:        out << "NEW";        break;
        case Token::DELETE:     out << "DELETE";     break;
        case Token::LBRACK:     out << "LBRACK";     break;
        case Token::RBRACK:     out << "RBRACK";     break;
        case Token::AMP:        out << "AMP";        break;
        case Token::null:       out << "NULL";       break;
        case Token::WHITESPACE: out << "WHITESPACE"; break;
        case Token::COMMENT:    out << "COMMENT";    break;
            }
            out << " " << tok.getLexeme() << std::endl;
            
            return out;
}
            
int64_t Token::toLong() const {
    std::istringstream iss;
    int64_t result;
    if (kind == NUM) {
        iss.str(lexeme);
    }
    iss >> result;
    return result;
}
            
ScanningFailure::ScanningFailure(std::string message):
    message(std::move(message)) {}
            
const std::string &ScanningFailure::what() const { return message; }
            
            
class AsmDFA {
public:
    enum State {
            ID = 0,
            NUM,
            ZERO,
            LPAREN,
            RPAREN,
            LBRACE,
            RBRACE,
            RETURN,
            R,
            RE,
            RET,
            RETU,
            RETUR,
            IF,
            I,
            ELSE,
            E,
            EL,
            ELS,
            WHILE,
            W,
            WH,
            WHI,
            WHIL,
            PRINTLN,
            P,
            PR,
            PRI,
            PRIN,
            PRINT,
            PRINTL,
            WAIN,
            WA,
            WAI,
            BECOMES,
            INT,
            IN,
            EQ,
            NOTEQ,
            NOTE,
            LT,
            GT,
            LE,
            GE,
            PLUS,
            MINUS,
            STAR,
            SLASH,
            PCT,
            COMMA,
            SEMI,
            NEW,
            N,
            NE,
            DELETE,
            D,
            DE,
            DEL,
            DELE,
            DELET,
            LBRACK,
            RBRACK,
            AMP,
            null,
            n,
            nu,
            nul,
            WHITESPACE,
            COMMENT,
            
            FAIL,
            START,
            
            // Hack to let this be used easily in arrays. This should always be the
            // final element in the enum, and should always point to the previous
            // element.
            
            LARGEST_STATE = START
            };
            
private:
            /* A set of all accepting states for the DFA.
             * Currently non-accepting states are not actually present anywhere
             * in memory, but a list can be found in the constructor.
             */
        std::set<State> acceptingStates;
            
            /*
             * The transition function for the DFA, stored as a map.
             */
            
        std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;
            
            /*
             * Converts a state to a kind to allow construction of Tokens from States.
             * Throws an exception if conversion is not possible.
             */
        Token::Kind stateToKind(State s) const {
            switch(s) {
        case ID:         return Token::ID;
        case NUM:        return Token::NUM;
        case ZERO:       return Token::NUM;
        case LPAREN:     return Token::LPAREN;
        case RPAREN:     return Token::RPAREN;
        case LBRACE:     return Token::LBRACE;
        case RBRACE:     return Token::RBRACE;
        case R:          return Token::ID;
        case RE:         return Token::ID;
        case RET:        return Token::ID;
        case RETU:       return Token::ID;
        case RETUR:      return Token::ID;
        case RETURN:     return Token::RETURN;
        case I:          return Token::ID;
        case IF:         return Token::IF;
        case E:          return Token::ID;
        case EL:         return Token::ID;
        case ELS:        return Token::ID;
        case ELSE:       return Token::ELSE;
        case W:          return Token::ID;
        case WH:         return Token::ID;
        case WHI:        return Token::ID;
        case WHIL:       return Token::ID;
        case WHILE:      return Token::WHILE;
        case P:          return Token::ID;
        case PR:         return Token::ID;
        case PRI:        return Token::ID;
        case PRIN:       return Token::ID;
        case PRINT:      return Token::ID;
        case PRINTL:     return Token::ID;
        case PRINTLN:    return Token::PRINTLN;
        case WA:         return Token::ID;
        case WAI:        return Token::ID;
        case WAIN:       return Token::WAIN;
        case BECOMES:    return Token::BECOMES;
        case IN:         return Token::ID;
        case INT:        return Token::INT;
        case EQ:         return Token::EQ;
        case NOTEQ:      return Token::NE;
        case LT:         return Token::LT;
        case GT:         return Token::GT;
        case LE:         return Token::LE;
        case GE:         return Token::GE;
        case PLUS:       return Token::PLUS;
        case MINUS:      return Token::MINUS;
        case STAR:       return Token::STAR;
        case PCT:        return Token::PCT;
        case SLASH:      return Token::SLASH;
        case COMMA:      return Token::COMMA;
        case SEMI:       return Token::SEMI;
        case N:          return Token::ID;
        case NE:         return Token::ID;
        case NEW:        return Token::NEW;
        case D:          return Token::ID;
        case DE:         return Token::ID;
        case DEL:        return Token::ID;
        case DELE:       return Token::ID;
        case DELET:      return Token::ID;
        case DELETE:     return Token::DELETE;
        case LBRACK:     return Token::LBRACK;
        case RBRACK:     return Token::RBRACK;
        case AMP:        return Token::AMP;
        case n:          return Token::ID;
        case nu:         return Token::ID;
        case nul:        return Token::ID;
        case null:       return Token::null;
        case WHITESPACE: return Token::WHITESPACE;
        case COMMENT:    return Token::COMMENT;
        default: throw ScanningFailure("ERROR: Cannot convert state to kind.");
            }
        }
            
            
public:
            
        std::vector<Token> simplifiedMaximalMunch(const std::string &input) const {
            std::vector<Token> result;
            
            State state = start();
            std::string munchedInput;
            
            // We can't use a range-based for loop effectively here
            // since the iterator doesn't always increment.
            for (std::string::const_iterator inputPosn = input.begin();
                 inputPosn != input.end();) {
            
                State oldState = state;
                state = transition(state, *inputPosn);
            
                if (!failed(state)) {
                    munchedInput += *inputPosn;
                    oldState = state;
            
                    ++inputPosn;
                }
            
                if (inputPosn == input.end() || failed(state)) {
                    if (accept(oldState)) {
                        result.push_back(Token(stateToKind(oldState), munchedInput));
            
                        munchedInput = "";
                        state = start();
                    } else {
                        if (failed(state)) {
                        munchedInput += *inputPosn;
                    }
                    throw ScanningFailure("ERROR: Simplified maximal munch failed on input: "
                                  + munchedInput);
                    }
                }
            }
            
            return result;
        }
            
            /* Initializes the accepting states for the DFA.
             */
        AsmDFA() {
            acceptingStates = {ID, NUM, LPAREN, RPAREN, LBRACE, RBRACE, R, RE, RET, RETU, RETUR, RETURN, I, IF, E, EL, ELS, ELSE,
            W, WH, WHI, WHIL, WHILE, P, PR, PRI, PRIN, PRINT, PRINTL, PRINTLN, WA, WAI,WAIN, BECOMES, IN, INT,
            ZERO, EQ, NOTEQ, LT, GT, LE, GE, PLUS, MINUS, STAR, SLASH, LBRACK, RBRACK, AMP, WHITESPACE, COMMENT,
            PCT, COMMA, SEMI, N, NE, NEW, D, DE, DEL, DELE, DELET, DELETE, n, nu, nul, null
            };
            
            
            // Initialize transitions for the DFA
            for (size_t i = 0; i < transitionFunction.size(); ++i) {
                for (size_t j = 0; j < transitionFunction[0].size(); ++j) {
                    transitionFunction[i][j] = FAIL;
                }
            }
            
            registerTransition(START, "(", LPAREN);
            registerTransition(START, ")", RPAREN);
            registerTransition(START, "{", LBRACE);
            registerTransition(START, "}", RBRACE);
            registerTransition(START, "=", BECOMES);
            registerTransition(START, "!", NOTE);
            registerTransition(START, "123456789", NUM);
            registerTransition(START, "0", ZERO);
            registerTransition(START, "<", LT);
            registerTransition(START, ">", GT);
            registerTransition(START, "+", PLUS);
            registerTransition(START, "-", MINUS);
            registerTransition(START, "*", STAR);
            registerTransition(START, "/", SLASH);
            registerTransition(START, "%", PCT);
            registerTransition(START, ",", COMMA);
            registerTransition(START, ";", SEMI);
            registerTransition(START, "[", LBRACK);
            registerTransition(START, "]", RBRACK);
            registerTransition(START, "&", AMP);
            registerTransition(START, "w", W);
            registerTransition(START, "r", R);
            registerTransition(START, "i", I);
            registerTransition(START, "e", E);
            registerTransition(START, "n", N);
            registerTransition(START, "d", D);
            registerTransition(START, "N", n);
            registerTransition(START, "p", P);
            registerTransition(START, "abcfghjklmoqstuvxyz", ID);
            registerTransition(START, "ABCDEFGHIJKLMOPQRSTUVWXYZ", ID);
            registerTransition(START, isspace, WHITESPACE);
            registerTransition(BECOMES, "=", EQ);
            registerTransition(NOTE, "=", NOTEQ);
            registerTransition(LT, "=", LE);
            registerTransition(GT, "=", GE);
            registerTransition(SLASH, "/", COMMENT);
            registerTransition(W, "a", WA);
            registerTransition(W, "bcdefgijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(W, isdigit, ID);
            registerTransition(WA, "i", WAI);
            registerTransition(WA, "abcdefghjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(WA, isdigit, ID);
            registerTransition(WAI, "n", WAIN);
            registerTransition(WAI, "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(WAI, isdigit, ID);
            registerTransition(WAIN, isalnum, ID);
            registerTransition(W, "h", WH);
            registerTransition(WH, "i", WHI);
            registerTransition(WH, "abcdefghjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(WH, isdigit, ID);
            registerTransition(WHI, "l", WHIL);
            registerTransition(WHI, "abcdefghijkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(WHI, isdigit, ID);
            registerTransition(WHIL, "e", WHILE);
            registerTransition(WHIL, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(WHIL, isdigit, ID);
            registerTransition(WHILE, isalnum, ID);
            registerTransition(R, "e", RE);
            registerTransition(R, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(R, isdigit, ID);
            registerTransition(RE, "t", RET);
            registerTransition(RE, "abcdefghijklmnopqrsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(RE, isdigit, ID);
            registerTransition(RET, "u", RETU);
            registerTransition(RET, "abcdefghijklmnopqrstvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(RET, isdigit, ID);
            registerTransition(RETU, "r", RETUR);
            registerTransition(RETU, "abcdefghijklmnopqstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(RETU, isdigit, ID);
            registerTransition(RETUR, "n", RETURN);
            registerTransition(RETUR, "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(RETUR, isdigit, ID);
            registerTransition(RETURN, isalnum, ID);
            registerTransition(I, "f", IF);
            registerTransition(I, "abcdeghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(I, isdigit, ID);
            registerTransition(IF, isalnum, ID);
            registerTransition(I, "n", IN);
            registerTransition(IN, "t", INT);
            registerTransition(IN, "abcdefghijklmnopqrsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(IN, isdigit, ID);
            registerTransition(INT, isalnum, ID);
            registerTransition(E, "l", EL);
            registerTransition(E, "abcdefghijkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(E, isdigit, ID);
            registerTransition(EL, "s", ELS);
            registerTransition(EL, "abcdefghijklmnopqrtuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(EL, isdigit, ID);
            registerTransition(ELS, "e", ELSE);
            registerTransition(ELS, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(ELS, isdigit, ID);
            registerTransition(ELSE, isalnum, ID);
            registerTransition(N, "e", NE);
            registerTransition(N, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(N, isdigit, ID);
            registerTransition(NE, "w", NEW);
            registerTransition(NE, "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(NE, isdigit, ID);
            registerTransition(NEW, isalnum, ID);
            registerTransition(D, "e", DE);
            registerTransition(D, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(D, isdigit, ID);
            registerTransition(DE, "l", DEL);
            registerTransition(DE, "abcdefghijkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(DE, isdigit, ID);
            registerTransition(DEL, "e", DELE);
            registerTransition(DEL, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(DEL, isdigit, ID);
            registerTransition(DELE, "t", DELET);
            registerTransition(DELE, "abcdefghijklmnopqrsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(DELE, isdigit, ID);
            registerTransition(DELET, "e", DELETE);
            registerTransition(DELET, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(DELET, isdigit, ID);
            registerTransition(DELETE, isalnum, ID);
            registerTransition(n, "U", nu);
            registerTransition(n, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTVWXYZ", ID);
            registerTransition(n, isdigit, ID);
            registerTransition(nu, "L", nul);
            registerTransition(nu, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKMNOPQRSTUVWXYZ", ID);
            registerTransition(nu, isdigit, ID);
            registerTransition(nul, "L", null);
            registerTransition(nul, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKMNOPQRSTUVWXYZ", ID);
            registerTransition(nul, isdigit, ID);
            registerTransition(null, isalnum, ID);
            registerTransition(P, "r", PR);
            registerTransition(P, "abcdefghijklmnopqstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(P, isdigit, ID);
            registerTransition(PR, "i", PRI);
            registerTransition(PR, "abcdefghjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(PR, isdigit, ID);
            registerTransition(PRI, "n", PRIN);
            registerTransition(PRI, "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(PRI, isdigit, ID);
            registerTransition(PRIN, "t", PRINT);
            registerTransition(PRIN, "abcdefghijklmnopqrsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(PRIN, isdigit, ID);
            registerTransition(PRINT, "l", PRINTL);
            registerTransition(PRINT, "abcdefghijkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(PRINT, isdigit, ID);
            registerTransition(PRINTL, "n", PRINTLN);
            registerTransition(PRINTL, "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", ID);
            registerTransition(PRINTL, isdigit, ID);
            registerTransition(PRINTLN, isalnum, ID);
            registerTransition(ID, isalnum, ID);
            registerTransition(NUM, isdigit, NUM);
            
            registerTransition(COMMENT, [](int c) -> int { return c != '\n'; },
                               COMMENT);
            registerTransition(WHITESPACE, isspace, WHITESPACE);
        }
            
            // Register a transition on all chars in chars
        void registerTransition(State oldState, const std::string &chars,
                                    State newState) {
            for (char c : chars) {
                transitionFunction[oldState][c] = newState;
            }
        }
            
            // Register a transition on all chars matching test
            // For some reason the cctype functions all use ints, hence the function
            // argument type.
        void registerTransition(State oldState, int (*test)(int), State newState) {
            
            for (int c = 0; c < 128; ++c) {
                if (test(c)) {
                    transitionFunction[oldState][c] = newState;
                }
            }
        }
            
            /* Returns the state corresponding to following a transition
             * from the given starting state on the given character,
             * or a special fail state if the transition does not exist.
             */
        State transition(State state, char nextChar) const {
            return transitionFunction[state][nextChar];
        }
            
            /* Checks whether the state returned by transition
             * corresponds to failure to transition.
             */
        bool failed(State state) const { return state == FAIL; }
            
            /* Checks whether the state returned by transition
             * is an accepting state.
             */
        bool accept(State state) const {
            return acceptingStates.count(state) > 0;
        }
            
            /* Returns the starting state of the DFA
             */
        State start() const { return START; }
};
            
std::vector<Token> scan(const std::string &input) {
    static AsmDFA theDFA;
            
    std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);
            
    std::vector<Token> newTokens;
            
    for(auto token = tokens.begin();token != tokens.end(); ++token) {
        if (token->getKind() != Token::WHITESPACE && token->getKind() != Token::COMMENT) {
            if (token->getKind() == Token::NUM||token->getKind() == Token::ID||token->getKind() == Token::RETURN||
                token->getKind() == Token::IF||token->getKind() == Token::ELSE||token->getKind() == Token::WHILE||
                token->getKind() == Token::PRINTLN||token->getKind() == Token::WAIN||token->getKind() == Token::INT||
                token->getKind() == Token::NEW||token->getKind() == Token::null||token->getKind() == Token::DELETE) {
                auto next = token+1;
                if (next->getKind() == Token::NUM||next->getKind() == Token::ID||next->getKind() == Token::RETURN||
                    next->getKind() == Token::IF||next->getKind() == Token::ELSE||next->getKind() == Token::WHILE||
                    next->getKind() == Token::PRINTLN||next->getKind() == Token::WAIN||next->getKind() == Token::INT||
                    next->getKind() == Token::NEW||next->getKind() == Token::null||next->getKind() == Token::DELETE) {
                    throw ScanningFailure("ERROR: Lexer error");
                }
            }
            if (token->getKind() == Token::EQ||token->getKind() == Token::NE||token->getKind() == Token::LT||
                token->getKind() == Token::GT||token->getKind() == Token::LE||token->getKind() == Token::GE||
                token->getKind() == Token::BECOMES) {
                auto next = token+1;
                if (next->getKind() == Token::EQ||next->getKind() == Token::NE||next->getKind() == Token::LT||
                    next->getKind() == Token::GT||next->getKind() == Token::LE||next->getKind() == Token::GE||
                    next->getKind() == Token::BECOMES) {
                    throw ScanningFailure("ERROR: Lexer error");
                }
            }
            if (token->getKind() == Token::NUM) {
                if (token->toLong() > 2147483647){
                    throw ScanningFailure("ERROR: Numeric literal out of range");
                }
            }
            newTokens.push_back(*token);
        }
    }
            
    return newTokens;
}
            
            
int main(){
    std::string line;
    std::vector<Token> tokens;
    try {
        while (getline(std::cin, line)) {
            std::vector<Token> tokenLine = scan(line);
            for (auto &token : tokenLine) {
                tokens.push_back(token);
            }
        }
        for (auto token: tokens) {
            std::cout << token;
        }
    } catch (ScanningFailure &f) {
        std::cerr << f.what() << std::endl;
        return 1;
    }
}
            
            

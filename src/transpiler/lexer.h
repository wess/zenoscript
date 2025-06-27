#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_ATOM,
    
    // Keywords
    TOKEN_STRUCT,
    TOKEN_TRAIT,
    TOKEN_IMPL,
    TOKEN_FOR,
    TOKEN_LET,
    TOKEN_MATCH,
    TOKEN_WHEN,
    TOKEN_RETURN,
    
    // Operators
    TOKEN_PIPE,           // |>
    TOKEN_ARROW,          // =>
    TOKEN_ASSIGN,         // =
    TOKEN_COLON,          // :
    TOKEN_DOT,            // .
    TOKEN_UNDERSCORE,     // _
    
    // Delimiters
    TOKEN_LBRACE,         // {
    TOKEN_RBRACE,         // }
    TOKEN_LPAREN,         // (
    TOKEN_RPAREN,         // )
    TOKEN_LANGLE,         // <
    TOKEN_RANGLE,         // >
    TOKEN_LBRACKET,       // [
    TOKEN_RBRACKET,       // ]
    TOKEN_SEMICOLON,      // ;
    TOKEN_COMMA,          // ,
    
    // Whitespace and comments
    TOKEN_NEWLINE,
    TOKEN_COMMENT,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
    int length;
} Token;

typedef struct {
    const char* source;
    int pos;
    int line;
    int column;
    int length;
} Lexer;

// Function prototypes
Lexer* lexer_new(const char* source);
void lexer_free(Lexer* lexer);
Token lexer_next_token(Lexer* lexer);
void token_free(Token* token);
const char* token_type_to_string(TokenType type);

#endif
#include "lexer.h"

// Keyword mapping
static struct {
    const char* keyword;
    TokenType type;
} keywords[] = {
    {"struct", TOKEN_STRUCT},
    {"trait", TOKEN_TRAIT},
    {"impl", TOKEN_IMPL},
    {"for", TOKEN_FOR},
    {"let", TOKEN_LET},
    {"match", TOKEN_MATCH},
    {"when", TOKEN_WHEN},
    {"return", TOKEN_RETURN},
    {NULL, TOKEN_IDENTIFIER}
};

Lexer* lexer_new(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(source);
    return lexer;
}

void lexer_free(Lexer* lexer) {
    if (lexer) {
        free(lexer);
    }
}

void token_free(Token* token) {
    if (token && token->value) {
        free(token->value);
        token->value = NULL;
    }
}

static char lexer_peek(Lexer* lexer) {
    if (lexer->pos >= lexer->length) {
        return '\0';
    }
    return lexer->source[lexer->pos];
}

static char lexer_advance(Lexer* lexer) {
    if (lexer->pos >= lexer->length) {
        return '\0';
    }
    
    char c = lexer->source[lexer->pos++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

static char lexer_peek_ahead(Lexer* lexer, int offset) {
    int pos = lexer->pos + offset;
    if (pos >= lexer->length) {
        return '\0';
    }
    return lexer->source[pos];
}

static void lexer_skip_whitespace(Lexer* lexer) {
    while (lexer_peek(lexer) && isspace(lexer_peek(lexer)) && lexer_peek(lexer) != '\n') {
        lexer_advance(lexer);
    }
}

static Token lexer_make_token(TokenType type, const char* value, int line, int column) {
    Token token;
    token.type = type;
    token.value = value ? strdup(value) : NULL;
    token.line = line;
    token.column = column;
    token.length = value ? strlen(value) : 0;
    return token;
}

static Token lexer_read_identifier(Lexer* lexer) {
    int start_pos = lexer->pos;
    int line = lexer->line;
    int column = lexer->column;
    
    while (lexer_peek(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
        lexer_advance(lexer);
    }
    
    int length = lexer->pos - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    // Check if it's a keyword
    for (int i = 0; keywords[i].keyword != NULL; i++) {
        if (strcmp(value, keywords[i].keyword) == 0) {
            Token token = lexer_make_token(keywords[i].type, value, line, column);
            free(value);
            return token;
        }
    }
    
    Token token = lexer_make_token(TOKEN_IDENTIFIER, value, line, column);
    free(value);
    return token;
}

static Token lexer_read_number(Lexer* lexer) {
    int start_pos = lexer->pos;
    int line = lexer->line;
    int column = lexer->column;
    
    while (lexer_peek(lexer) && isdigit(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
    
    // Handle decimal numbers
    if (lexer_peek(lexer) == '.' && isdigit(lexer_peek_ahead(lexer, 1))) {
        lexer_advance(lexer); // consume '.'
        while (lexer_peek(lexer) && isdigit(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
    }
    
    int length = lexer->pos - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = lexer_make_token(TOKEN_NUMBER, value, line, column);
    free(value);
    return token;
}

static Token lexer_read_string(Lexer* lexer) {
    int line = lexer->line;
    int column = lexer->column;
    
    lexer_advance(lexer); // consume opening quote
    
    char* value = malloc(1024); // Start with reasonable buffer
    int capacity = 1024;
    int length = 0;
    
    while (lexer_peek(lexer) && lexer_peek(lexer) != '"') {
        if (lexer_peek(lexer) == '\\') {
            lexer_advance(lexer); // consume backslash
            char escaped = lexer_advance(lexer);
            
            // Handle escape sequences
            switch (escaped) {
                case 'n': value[length++] = '\n'; break;
                case 't': value[length++] = '\t'; break;
                case 'r': value[length++] = '\r'; break;
                case '\\': value[length++] = '\\'; break;
                case '"': value[length++] = '"'; break;
                default: 
                    value[length++] = '\\';
                    value[length++] = escaped;
                    break;
            }
        } else {
            value[length++] = lexer_advance(lexer);
        }
        
        // Resize buffer if needed
        if (length >= capacity - 1) {
            capacity *= 2;
            value = realloc(value, capacity);
        }
    }
    
    if (lexer_peek(lexer) == '"') {
        lexer_advance(lexer); // consume closing quote
    }
    
    value[length] = '\0';
    Token token = lexer_make_token(TOKEN_STRING, value, line, column);
    free(value);
    return token;
}

static Token lexer_read_atom(Lexer* lexer) {
    int line = lexer->line;
    int column = lexer->column;
    
    lexer_advance(lexer); // consume ':'
    
    if (!isalpha(lexer_peek(lexer)) && lexer_peek(lexer) != '_') {
        return lexer_make_token(TOKEN_COLON, ":", line, column);
    }
    
    int start_pos = lexer->pos;
    while (lexer_peek(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
        lexer_advance(lexer);
    }
    
    int length = lexer->pos - start_pos + 1; // +1 for the ':'
    char* value = malloc(length + 1);
    value[0] = ':';
    strncpy(value + 1, lexer->source + start_pos, length - 1);
    value[length] = '\0';
    
    Token token = lexer_make_token(TOKEN_ATOM, value, line, column);
    free(value);
    return token;
}

Token lexer_next_token(Lexer* lexer) {
    lexer_skip_whitespace(lexer);
    
    if (lexer->pos >= lexer->length) {
        return lexer_make_token(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    char c = lexer_peek(lexer);
    int line = lexer->line;
    int column = lexer->column;
    
    // Single character tokens
    switch (c) {
        case '\n':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_NEWLINE, "\n", line, column);
            
        case '{':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_LBRACE, "{", line, column);
            
        case '}':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_RBRACE, "}", line, column);
            
        case '(':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_LPAREN, "(", line, column);
            
        case ')':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_RPAREN, ")", line, column);
            
        case '<':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_LANGLE, "<", line, column);
            
        case '>':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_RANGLE, ">", line, column);
            
        case '[':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_LBRACKET, "[", line, column);
            
        case ']':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_RBRACKET, "]", line, column);
            
        case ';':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_SEMICOLON, ";", line, column);
            
        case ',':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_COMMA, ",", line, column);
            
        case '.':
            lexer_advance(lexer);
            return lexer_make_token(TOKEN_DOT, ".", line, column);
            
        case '_':
            if (!isalnum(lexer_peek_ahead(lexer, 1))) {
                lexer_advance(lexer);
                return lexer_make_token(TOKEN_UNDERSCORE, "_", line, column);
            }
            break;
            
        case '"':
            return lexer_read_string(lexer);
            
        case ':':
            return lexer_read_atom(lexer);
    }
    
    // Multi-character tokens
    if (c == '|' && lexer_peek_ahead(lexer, 1) == '>') {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return lexer_make_token(TOKEN_PIPE, "|>", line, column);
    }
    
    if (c == '=' && lexer_peek_ahead(lexer, 1) == '>') {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return lexer_make_token(TOKEN_ARROW, "=>", line, column);
    }
    
    if (c == '=') {
        lexer_advance(lexer);
        return lexer_make_token(TOKEN_ASSIGN, "=", line, column);
    }
    
    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        return lexer_read_identifier(lexer);
    }
    
    // Numbers
    if (isdigit(c)) {
        return lexer_read_number(lexer);
    }
    
    // Unknown character
    lexer_advance(lexer);
    char* error_value = malloc(2);
    error_value[0] = c;
    error_value[1] = '\0';
    Token token = lexer_make_token(TOKEN_ERROR, error_value, line, column);
    free(error_value);
    return token;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_ATOM: return "ATOM";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_TRAIT: return "TRAIT";
        case TOKEN_IMPL: return "IMPL";
        case TOKEN_FOR: return "FOR";
        case TOKEN_LET: return "LET";
        case TOKEN_MATCH: return "MATCH";
        case TOKEN_WHEN: return "WHEN";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_COLON: return "COLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_UNDERSCORE: return "UNDERSCORE";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LANGLE: return "LANGLE";
        case TOKEN_RANGLE: return "RANGLE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
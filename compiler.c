// make run file={filename}

#include <stdio.h>
#include <string.h>

#define MAX_TOKEN 100

// Funcion para determinar si un caracter es una letra (a-z, A-Z)
int es_letra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Funcion para determinar si un caracter es un digito (0-9)
int es_digito(char c) { 
    return (c >= '0' && c <= '9');
}

// Funcion para determinar si un caracter es alfanumerico (letras y digitos)
int es_alfanumerico(char c) {
    return es_letra(c) || es_digito(c);
}

// Funcion para determinar si un caracter es un espacio en blanco
// (espacio, tabulador, nueva linea, retorno de carro)
int es_espacio(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

// Palabras clave del lenguaje
const char* palabras_clave[] = {
    // Procedural Programming Language Keywords
    "int", "float", "char", "long", "double", "string", "enum", "pair", "bool", "void", "const", "static",

    // Object Oriented Programming Language Keywords
    "class", "struct", "this", "public", "private", "protected", "extends", "implements", "override", "virtual", "new", "delete",

    // Mixed Programming Language Keywords

    // Control Structures & Logic Keywords
    "if", "else", "switch", "case", "break", "continue", "while", "for", "do",

    // Functions and Program Structure Keywords
    "main", "return", "void", "function", "using", "namespace", "include"

    // Error Handling Keywords
    "try", "catch", "throw", "finally",

    // Boolean Logic Keywords
    "true", "false", "null",

    // Map and Pair Methods
    "first", "second"
};

// Definicion de la cantidad de palabras clave
int token_id = sizeof(palabras_clave)/sizeof(palabras_clave[0]);

// Funcion para determinar si una palabra es una palabra clave
// (comparando con la lista de palabras clave previamente definidas)
int es_palabra_clave(const char* palabra) {
    for (int i = 0; i < sizeof(palabras_clave)/sizeof(palabras_clave[0]); i++) {
        if (strcmp(palabra, palabras_clave[i]) == 0)
            return 1;
    }
    return 0;
}

// Estados posibles del DFA
typedef enum {
    INICIO,
    ID,
    NUM,
    STR,
    OP,
    SEP
} Estado;

int main(int argc, char* argv[]) {
    // Comprobar si se recibierons los argumentos minimos
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    // Abrir el archivo en modo lectura
    FILE* archivo = fopen(argv[1], "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    // Inicializar variables
    char c, buffer[MAX_TOKEN];
    int index = 0;

    // Inicializar el estado del DFA en INICIO
    Estado estado = INICIO;

    while ((c = fgetc(archivo)) != EOF) {
        switch (estado) {
        // Estado inicial: Recibir un caracter y determinar el estado
        // correspondiente (ID, NUM, STR, OP, SEP)
        case INICIO:
            if (es_letra(c) || c == '_') {
                buffer[index++] = c;
                estado = ID;
            } else if (es_digito(c)) {
                buffer[index++] = c;
                estado = NUM;
            } else if (c == '"') {
                buffer[index++] = c;
                estado = STR;
            } else if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<') {
                buffer[index++] = c;
                estado = OP;
            } else if (c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}') {
                printf("<SEPARADOR, \"%c\">\n", c);
            } else if (!es_espacio(c)) {
                printf("<DESCONOCIDO, \"%c\">\n", c);
            }
            break;

        case ID:
            if (es_alfanumerico(c) || c == '_') {
                buffer[index++] = c;
            } else {
                buffer[index] = '\0';
                int es_palabra = 0;
                for (int i = 0; i < sizeof(palabras_clave) / sizeof(palabras_clave[0]); i++){
                    if (strcmp(buffer, palabras_clave[i]) == 0){
                        printf("<%d, PALABRA_CLAVE, \"%s\">\n", i, buffer);
                        es_palabra = 1;
                        break;
                    }
                }
                if (!es_palabra){
                    printf("<IDENTIFICADOR, \"%s\">\n", buffer);
                }
                index = 0;
                estado = INICIO;
                ungetc(c, archivo);
            }
            break;

        case NUM:
            if (es_digito(c)) {
                buffer[index++] = c;
            } else {
                buffer[index] = '\0';
                printf("<NUMERO, \"%s\">\n", buffer);
                index = 0;
                estado = INICIO;
                ungetc(c, archivo);
            }
            break;

        case STR:
            buffer[index++] = c;
            if (c == '"') {
                buffer[index] = '\0';
                printf("<STRING, \"%s\">\n", buffer);
                index = 0;
                estado = INICIO;
            }
            break;

        case OP:
            buffer[index] = '\0';
            printf("<OPERADOR, \"%s\">\n", buffer);
            index = 0;
            estado = INICIO;
            ungetc(c, archivo);
            break;

        default:
            break;
        }
    }

    fclose(archivo);
    return 0;
}

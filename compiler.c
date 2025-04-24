// make run file={filename}

#include <stdio.h>
#include <string.h>

#define MAX_TOKEN 100

int es_letra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int es_digito(char c) {
    return (c >= '0' && c <= '9');
}

int es_alfanumerico(char c) {
    return es_letra(c) || es_digito(c);
}

int es_espacio(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

// Palabras clave del lenguaje
const char* palabras_clave[] = {
    "int", "float", "char", "if", "else", "while", "for", "return", "void"
};

int es_palabra_clave(const char* palabra) {
    for (int i = 0; i < sizeof(palabras_clave)/sizeof(palabras_clave[0]); i++) {
        if (strcmp(palabra, palabras_clave[i]) == 0)
            return 1;
    }
    return 0;
}

// Estados posibles
typedef enum {
    INICIO,
    ID,
    NUM,
    STR,
    OP,
    SEP
} Estado;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    FILE* archivo = fopen(argv[1], "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    char c, buffer[MAX_TOKEN];
    int index = 0;
    Estado estado = INICIO;

    while ((c = fgetc(archivo)) != EOF) {
        switch (estado) {
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
                    if (es_palabra_clave(buffer)) {
                        printf("<PALABRA_CLAVE, \"%s\">\n", buffer);
                    } else {
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

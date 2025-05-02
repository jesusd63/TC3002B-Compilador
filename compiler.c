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

// Separadores del lenguaje
const char* separadores[] = {
    "(", ")", "{", "}",
};

// Palabras clave del lenguaje
const char* palabras_clave[] = {
    // Object Oriented Programming Language Keywords
    "class", "struct", "public", "private", "protected", "extends", "implements", "override", "virtual",
};

// Definicion de los simbolos del lenguaje
const char* simbolos[] = {
    ";", "*", ",", "&", "<", ">", ".", ":", "="
};

// Definicion de la cantidad de palabras clave
int identifier_id = 1 + sizeof(separadores)/sizeof(separadores[0]) + sizeof(palabras_clave)/sizeof(palabras_clave[0]) + sizeof(simbolos)/sizeof(simbolos[0]);
int string_id;

int identifier_count = 0;
int number_count = 0;
int string_count = 0;


// Estados posibles del DFA
typedef enum {
    INICIO, //Estado inicial
    ID, // Procesando un Identificador
    NUM, // Procesando un Numero (1-9)
    STR, // Procesando un String
    OP, // Procesando un Operador ()
    SEP, // Procesando un Separador
    ERROR // Estado de error
} Estado;

int main(int argc, char* argv[]) {
    // Comprobar si se recibieron los argumentos minimos
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    // Variables para los IDs de los Strings
    string_id = identifier_id+1;


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
            } else if (c == ';' || c == '*' ||c == ',' || c == '&' || c == '<' || c == '>' || c == '.' || c == ':' || c == '=') {
                buffer[index++] = c;
                estado = OP;
            } else if (c == '(' || c == ')' || c == '{' || c == '}') {
                buffer[index++] = c;
                estado = SEP;
            } else if(c == '[' || c == ']' || c == '?' || c == '!' || c == '+' || c == '-' || c == '/' || c == '%' || c == '^' || c == '|') {
                buffer[index++] = c;
                buffer[index] = '\0';
                identifier_count++;
                printf("<%d, %d>\n", identifier_id, identifier_count);
                index = 0;
            } else if (!es_espacio(c)) {
                printf("Error");
                estado = ERROR;
            }
            break;

        case ID:
            // ===== Estado ID =====
            if (es_alfanumerico(c) || c == '_') {
                buffer[index++] = c;
            } else {
                buffer[index] = '\0';
                int es_palabra = 0;
                // Verificar si coincide con alguna palabra clave
                for (int i = 0; i < sizeof(palabras_clave) / sizeof(palabras_clave[0]); i++){
                    if (strcmp(buffer, palabras_clave[i]) == 0){
                        // Si es palabra clave: imprimir su token (Sumando el ID de los separadores)
                        printf("<%d>\n", i + 1 + sizeof(separadores) / sizeof(separadores[0]));
                        es_palabra = 1;
                        break;
                    }
                }
                if (!es_palabra){
                    identifier_count++;
                    // Si es identificador: asignar ID secuencial
                    printf("<%d, %d>\n", identifier_id, identifier_count);
                }
                index = 0;
                estado = INICIO;
                ungetc(c, archivo);
            }
            break;

        case NUM:
            // ===== Estado NUM =====
            if (es_digito(c)) {
                buffer[index++] = c;
            } else {
                buffer[index] = '\0';
                // No se genera token explícito para NUM
                index = 0;
                estado = INICIO;
                ungetc(c, archivo);
            }
            break;

        case STR:
            // ===== Estado STR =====
            buffer[index++] = c;
            if (c == '"') {
                buffer[index] = '\0';
                string_count++;
                // Es String: asignar ID secuencial
                printf("<%d, %d>\n", string_id, string_count);
                index = 0;
                estado = INICIO;
            }
            break;

        case OP:
            // ===== Estado OP =====
            buffer[index] = '\0';
            for (int i = 0; i < sizeof(simbolos) / sizeof(simbolos[0]); i++){
                if (strcmp(buffer, simbolos[i]) == 0){
                    // Si es simbolo: imprimir su token (Sumando el ID de los separadores y palabras clave)
                    printf("<%d>\n", i + 1 + (sizeof(separadores) / sizeof(separadores[0])) + (sizeof(palabras_clave) / sizeof(palabras_clave[0])));
                    break;
                }
            }
            index = 0;
            estado = INICIO;
            ungetc(c, archivo);
            break;
        
        case SEP:
            // ===== Estado SEP =====
            buffer[index] = '\0';
            for (int i = 0; i < sizeof(separadores) / sizeof(separadores[0]); i++){
                if (strcmp(buffer, separadores[i]) == 0){
                    // Si es separador: imprimir su token
                    printf("<%d>\n", i+1);
                    break;
                }
            }
            index = 0;
            estado = INICIO;
            ungetc(c, archivo);
            break;

        default:
            break;
        }

        // Si se llega a un estado de error, terminar
        if(estado == ERROR) {
            break;
        }
    }

    fclose(archivo);
    return 0;
}

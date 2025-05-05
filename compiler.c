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

// Funcion para determinar si un caracter es un delimitador
int es_delimitador(char c) {
    return c == ';' || c == ',' || c == '.' || c == ':' || c == '=' || c == '*' || c == '&' || c == '<' || c == '>';
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

// Marcar el token de identificadores y strings
int identifier_id = 1 + sizeof(separadores)/sizeof(separadores[0]) + sizeof(palabras_clave)/sizeof(palabras_clave[0]) + sizeof(simbolos)/sizeof(simbolos[0]);
int string_id;

// Contadores de tokens
int identifier_count = 0;
int string_count = 0;

// Funcion que determina si un estado es un estado aceptador
int Accept(int estado){
    if(estado == 1 || estado == 2 || estado == 3 || estado == 4 || estado == 5 || estado == 7 || estado == 9){
        return 1; // Aceptar el token
    }
    return 0; // No aceptar el token
}

// Funcion que determina si un estado es un estado de error
int Error(int estado){
    return estado == 10; // Estado de error
}

// Tabla de transiciones del DFA
// Estado 0: INICIO
// Estado 10: ERROR

int TransitionTable[10][10] = {
    [0] = {6, 6, 5, 5, 1, 2, 3, 4, 8, 10}, // Estado de inicio
    [1] = {0}, // Estado de (
    [2] = {0}, // Estado de )
    [3] = {0}, // Estado de {
    [4] = {0}, // Estado de }
    [5] = {0}, // Estado de aceptacion de otros caracteres
    [6] = {6, 6, 6, 7, 7, 7, 7, 7, 7, 10}, // Estado de identificador
    [7] = {0}, // Estado de aceptacion de identificador
    [8] = {8, 8, 8, 8, 8, 8, 8, 8, 9, 8}, // Estado de string
    [9] = {0} // Estado de aceptacion de string
}; 

// Funcion que determina la columna de la tabla de transiciones segun el caracter
int char_to_col(char c) {
    if (es_letra(c)){
        return 0; // Columna para letras
    } 
    if (c == '_'){
        return 1; // Columna para undersore
    } 
    if (es_digito(c)){
        return 2; // Columna para digitos
    }
    if (es_espacio(c) || es_delimitador(c)){
        return 3; // Columna para delimitadores 
    } 
    if (c == '('){
        return 4; // Columna para paréntesis izquierdo
    } 
    if (c == ')'){
        return 5; // Columna para paréntesis derecho
    }
    if (c == '{'){
        return 6; // Columna para llave izquierda
    }
    if (c == '}'){
        return 7; // Columna para llave derecha
    }
    if (c == '"'){
        return 8; // Columna para comillas
    }
    else {
        return 9; // Columna para caracteres no reconocidos
    }

    return -1;
}

// Funcion que determina el nuevo estado del DFA segun el estado actual y el caracter
int T(int estado, char c) {
    int col = char_to_col(c);
    return TransitionTable[estado][col]; // Retorna el nuevo estado
}

// Funcion que determina si un estado puede seguir aceptando caracteres
int Advance(int estado, char c) {
    // Si estamos en un estado aceptador, no avanzar (porque ya tenemos un token completo)
    if (Accept(estado)) return 0;

    // En cualquier otro caso, seguimos avanzando
    return 1;
}

// Funcion que imprime el token
void record_token(char* buffer, int index, int estado) {
    // Terminar el string
    buffer[index] = '\0';

    // Estados aceptadores de parentesis y llaves
    for(int i = 1; i < 5; i++){
        if(estado == i){
            // Imprimir el token
            printf("%d, %s\n", i, buffer);
        }
    }

    // Estado aceptador de delimitadores y otros caracteres
    if(estado == 5) {
        // Si el caracter es un espacio, ignorarlo 
        if(es_espacio(buffer[0])){
            return;
        }
        // Buscar si el caracter es un simbolo reservado
        for(int i = 0; i < sizeof(simbolos)/sizeof(simbolos[0]); i++){
            if(strcmp(buffer, simbolos[i]) == 0){
                printf("%d, %s\n",i+sizeof(palabras_clave)/sizeof(palabras_clave[0])+sizeof(separadores)/sizeof(separadores[0])+1, buffer);
                return;
            }
        }
        // Sumar a la cuenta de delimitadores
        identifier_count++;
        // Imprimir el token
        printf("%d, %d, %s\n" , identifier_id, identifier_count, buffer);
    
    // Estado aceptador de identificadores
    } else if (estado == 7) { // 
        // Buscar si la palabra es una palabra clave
        for(int i = 0; i < sizeof(palabras_clave)/sizeof(palabras_clave[0]); i++){
            if(strcmp(buffer, palabras_clave[i]) == 0){
                // Imprimir el token
                printf("%d, %s\n",i+sizeof(separadores)/sizeof(separadores[0])+1, buffer);
                return;
            }
        }
        // Sumar a la cuenta de identificadores
        identifier_count++;
        // Imprimir el token
        printf("%d, %d, %s\n" , identifier_id, identifier_count, buffer);
    }
    // Estado aceptador de strings 
    else if (estado == 9){
        // Sumar a la cuenta de strings
        string_count++;
        // Imprimir el token
        printf("%d, %d, %s\n" , string_id, string_count, buffer);
    }
}

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

    // Leer el primer caracter del archivo
    c = fgetc(archivo);
    while (c != EOF) {
        // Inicializar el estado del DFA en INICIO
        int estado = 0;
        // Reiniciar el buffer
        index = 0;
        while(!Accept(estado) && !Error(estado)){
            // Determinar el nuevo estado del DFA
            estado = T(estado, c);
            // Guardar el caracter en el buffer
            buffer[index++] = c;

            if(Advance(estado, c)){
                // Leer el siguiente caracter
                c = fgetc(archivo);
            }
        }
        // El estado es aceptador o de error
        if(Accept(estado)){
            // Si el caracter es un espacio, ignorarlo
            if(es_espacio(buffer[0])){
                // Leer el siguiente caracter
                c = fgetc(archivo);
                continue;
            }
            // Si el estado es 7 (aceptor de identificador)
            if(estado == 7){
                // Ignorar el ultimo caracter
                index--; 
                // Devolver el caracter al flujo de entrada
                ungetc(c, archivo);
            }
            // Imprimir el token
            record_token(buffer, index, estado);
            // Leer el siguiente caracter
            c = fgetc(archivo);
        } else{ // Si el estado es de error
            printf("Error: token no reconocido.\n");
            // Terminar el programa
            break;
        }
    }

    // Cerrar el archivo
    fclose(archivo);
    return 0;
}

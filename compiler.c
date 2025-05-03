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

// Definicion de la cantidad de palabras clave
int identifier_id = 1 + sizeof(separadores)/sizeof(separadores[0]) + sizeof(palabras_clave)/sizeof(palabras_clave[0]) + sizeof(simbolos)/sizeof(simbolos[0]);
int string_id;

int identifier_count = 0;
int number_count = 0;
int string_count = 0;

int Accept(int estado){
    if(estado == 1 || estado == 2 || estado == 3 || estado == 4 || estado == 5 || estado == 7 || estado == 9){
        return 1; // Aceptar el token
    }
    return 0; // No aceptar el token
}

int Error(int estado){
    return estado == 10; // Estado de error
}

int TransitionTable[10][10] = {
    [0] = {6, 6, 5, 5, 1, 2, 3, 4, 8, 5}, // Estado de inicio
    [1] = {0}, // Estado inicial
    [2] = {0}, // Estado de identificador
    [3] = {0}, // Estado de numero
    [4] = {0}, // Estado de cadena
    [5] = {0}, // Estado de separador
    [6] = {6, 6, 6, 7, 5, 5, 5, 5, 5, 5},
    [7] = {0}, // Estado de simbolo
    [8] = {8, 8, 8, 8, 8, 8, 8, 8, 9, 8}, // Estado de palabra clave
    [9] = {0} // Estado de fin
}; 

int char_to_col(char c) {
    //printf("Caracter a cambiar: %c\n", c);
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
        return 9; // Columna para espacio en blanco
    }

    return -1; // Caracter no reconocido
}

int T(int estado, char c) {
    int col = char_to_col(c);
    return TransitionTable[estado][col]; // Retorna el nuevo estado
}

int Advance(int estado, char c) {
    // Si estamos procesando una cadena, siempre avanzamos
    if (estado == 4 || estado == 8 || estado == 9) return 1;
    
    // Si estamos en un estado aceptador, no avanzar (porque ya tenemos un token completo)
    if (Accept(estado)) return 0;

    // En cualquier otro caso, seguimos avanzando
    return 1;
}

void record_token(char* buffer, int index, int estado) {
    buffer[index] = '\0'; // Terminar el string
    if (estado == 1 ) { // 
        printf("1, %s\n", buffer);
    } else if (estado == 2) { // 
        printf("2, %s\n", buffer);
    } else if (estado == 3) { // 
        printf("3, %s\n", buffer);
    } else if (estado == 4) { // 
        printf("4, %s\n", buffer);
    } else if (estado == 5) { // 
        if(es_espacio(buffer[0])){
            return;
        }
        for(int i = 0; i < sizeof(simbolos)/sizeof(simbolos[0]); i++){
            if(strcmp(buffer, simbolos[i]) == 0){
                printf("%d, %s\n",i+sizeof(palabras_clave)/sizeof(palabras_clave[0])+sizeof(separadores)/sizeof(separadores[0])+1, buffer);
                return;
            }
        }
        identifier_count++;
        printf("%d, %d, %s\n" , identifier_id, identifier_count, buffer); // Imprimir el token
    } else if (estado == 7) { // 
        for(int i = 0; i < sizeof(palabras_clave)/sizeof(palabras_clave[0]); i++){
            printf("Comparando: %s con %s\n", buffer, palabras_clave[i]);
            if(strcmp(buffer, palabras_clave[i]) == 0){
                printf("PALABRA CLAVE");
                printf("%d, %s\n",i+sizeof(separadores)/sizeof(separadores[0])+1, buffer);
                return;
            }
        }
        identifier_count++;
        printf("%d, %d, %s\n" , identifier_id, identifier_count, buffer); // Imprimir el token
    } else if (estado == 9){
        string_count++;
        printf("%d, %d, %s\n" , string_id, string_count, buffer); // Imprimir el token
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

    // Inicializar el estado del DFA en INICIO
    c = fgetc(archivo);
    while (c != EOF) {
        int estado = 0;
        index = 0; // Reiniciar el buffer
        while(!Accept(estado) && !Error(estado)){
            //printf("Estado inicial: %d, Caracter: %c\n", estado, c);
            estado = T(estado, c);
            //printf("Estado: %d, Caracter: %c\n", estado, c);
            buffer[index++] = c; // Guardar el caracter en el buffer
            if(Advance(estado, c)){
                c = fgetc(archivo); // Leer el siguiente caracter
            }
        }
        if(Accept(estado)){
            record_token(buffer, index, estado);
            if(estado == 7){
                ungetc(c, archivo); // Leer el siguiente caracter
            }
            c = fgetc(archivo); // Leer el siguiente caracter
        } else{
            printf("Error: token no reconocido eres un estupido vuelvelo a hacer por favor.\n");
        }
    }

    fclose(archivo);
    return 0;
}

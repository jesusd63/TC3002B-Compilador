# TC3002B - Analizador Léxico

## Descripción

Este proyecto consiste en un compilador que recibe como entrada un texto escrito en C, y determina si sigue uno de los siguientes paradigmas:

- **Programación Orientada a Objetos**
- **Programación Procedural**
- **Mezcla de ambos**

## Objetivo

El propósito principal de la **fase léxica** del compilador es identificar los **tokens** o partes válidas del lenguaje de programación en el orden en que aparecen en el archivo de entrada.

## Funcionalidades

- Detectar **palabras clave** necesarias y **símbolos especiales** del lenguaje.
- **Descartar lexemas innecesarios** o irrelevantes.
- Generar los **mensajes de error necesarios** si se encuentran problemas léxicos.
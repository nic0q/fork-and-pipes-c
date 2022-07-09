# Laboratorio 1 Sistemas Operativos 2022-1
Este laboratorio consiste en la elaboración de un programa que permite leer un archivo de texto y calcular una serie de propiedades utilizando funciones
como fork, pipe, execve y dup2, las cuales se guardan en un archivo de texto.<br/>
* Para una explicación completa revise el [enunciado de laboratorio](https://github.com/nic0q/LAB1-SISTOPE/blob/master/lab1_SO12022.pdf)

## Significado Flags:
- -i: Nombre del archivo de entrada con las visibilidades
- -o: Nombre del archivo de salida
- -n: Número de discos a crear
- -d: Ancho de disco (unidades)
- -b: Si esta activa, se muestra la cantidad de cálculos que cada proceso realizó

## Instrucciones
Se recomienda clonar el repositorio:
```sh
git clone https://github.com/nic0q/LAB1-SISTOPE
```
Ejecutar de la siguiente forma: (**En el repositorio se dispone de test100.csv y test1000.csv**)
- Compilar con el makefile **make**
- Ejecutando un test por defecto con **make run**
- Manualmente con: ./lab1 -i test10000.csv -o propiedades.txt -n 4 -d 100 -b

## Realizado por
[John](https://github.com/PodssilDev)<br/>
[Nícolas](https://github.com/nic0q)<br/>

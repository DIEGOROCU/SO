#!/bin/bash

# $# contiene el numero de argumentos recibidos; se comprueba que haya exactamente uno.
if [[ $# -ne 1 ]]; then
    # $0 contiene el nombre con el que se ha ejecutado el script.
    echo "Uso: $0 directorio"
    exit 1
fi

# $1 contiene el primer argumento recibido, que sera la ruta del directorio.
DIR="$1"

# La flag -d comprueba si la ruta existe y corresponde a un directorio.
if [[ -d "$DIR" ]]; then
    # $DIR contiene la ruta indicada; -r borra recursivamente y -f fuerza el borrado.
    rm -rf "$DIR"
fi

# $DIR indica donde crear el directorio; -p crea tambien las rutas intermedias.
mkdir -p "$DIR"  
# $DIR indica el directorio al que se cambia.
cd "$DIR"

mkdir   subdir
touch   fichero1
# La flag -n evita anadir un salto de linea, dejando exactamente 10 caracteres.
echo -n "0123456789" > fichero2
# La flag -s crea un enlace simbolico: apunta desde enlaceS hacia fichero2.
ln -s   fichero2 enlaceS
ln      fichero2 enlaceH

# Se recorre cada elemento creado y stat muestra sus atributos.
for fichero in subdir fichero1 fichero2 enlaceS enlaceH; do
    # $fichero contiene el nombre del elemento actual.
    stat "$fichero"
done


// Actividad Integradora 1

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>

using namespace std;

// función auxiliar para leer un archivo de texto y almacenar el contenido en una variable
string leeArchivo(string filename) {
    ifstream file(filename);
    string content;
    
    // leemos el archivo
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            content += line;
        }
        file.close();
    }
    return content; // retornamos el archivo
}


// preprocesamiento del patron con la función que encuentra el sufijo más largo que también sea prefijo
// longest preffix suffix
vector<int>lps(string pattern) {
    vector<int>result(pattern.size()); // definimos el vector de longitudes
    result[0] = 0; // inicializamos el primer elemento con 0

    int i = 1, j = 0;

    // llenamos el vector resultado iterando el patron
    while(i < pattern.size()) {
        if(pattern[j] == pattern[i]) { // si el caractér en j del patrón es igual al caractér en i, añadimos en la posición i del vector resultado la longitud del prefijo más largo que también es el sufijo 
            result[i] = j + 1; // esta operación permite conocer desde que índice empezar a comparar en el algoritmo KMP en caso de que haya un mismatch
            i++;
            j++;
        } else { // en caso de que los caractéres no sean iguales
            if(j == 0) {  // si j es 0, añadir 0 en la posición i del vector resultado
                result[i] = 0;
                i++;
            } // si j no es 0, j toma el valor de la posición j-1 del vector resultado para considerar casos que pueden suceder en el input string
            else j = result[j - 1];
        }
    }

    return result; // retornamos resultado

}

// algoritmo KMP que utiliza el preprocesamiento del patron para saber a donde retroceder
int KMP(string text, string pattern) {
    vector<int>V = lps(pattern); // vector del preprocesamiento del string
    int i = 0, j = 0; 

    // iteramos sobre el texto
    while(i < text.size()) {
        if(j == pattern.size()) {
            return i - j; // si acabamos de recorrer el patron, entonces el patron ya se encontró en el texto y el indice en donde empieza el patron en el texto es i - j
        }
        if(text[i] == pattern[j]) { // si hay un match en los caracteres, seguimos checando los demás caracteres
            i++;
            j++;
        } else { // si no hay match en los caracteres
            if(j==0) i++; // si estamos en el primer caracter del patron, checamos si el siguiente caracter del texto hace match
            else j = V[j - 1]; // si no estamos en el primer caracter del patron, utilizamos el vector de preprocesamiento del patron para saber a que posición hace sentido regresar si es que en el substring del patron hasta  j - 1 hay un prefijo que también sea sufijo
        }
    }

    return -1; // en caso de que no se encuentre el patrón regresamos -1 


}

// función para calcular el minimo entre dos numeros
int min(int a, int b) {
    if(a <= b) return a;
    else return b;
}


// algoritmo de manacher
tuple<int,int>manacher(string text) {
    // preprocesamiento del texto
    // se busca tener un string del tipo:  'abba' --> '$#a#b#b#a#@'
    string T = "$#";

    // construimos el string 
    for(int i = 0; i < text.size(); i++) {
        T += text[i];
        T+= "#";
    }
    T+="@";

   // vector con los valores de la longitud de cada palindromo encontrado en el texto
   vector<int>P(T.size());

   int maximumLengthP = 0, centerIndex = 0, center = 0, rightBoundary = 0;

   // recorremos el string 'T' y vamos encontrando un nuevo palindromo centrado en el indice 'c' que tiene como limite derecho el caractér en la posicion 'rightBoundary'
   for(int i = 1; i < T.size() - 1; i++) {
    // calculamos el espejo del indice actual
    // dado un palindromo 'abcdcba', el espejo del caracter 'b' en el indice '1' es el caractér 'b' en el indice '5'
    // si nuestro indice actual es 'i = 5', entonces el indice espejo se encuentra de la siguiente manera: 2*center - i = 2*3 - 5 = 1
    int mirror = 2*center - i;

    // checamos si nuestro indice actual está dentro de el límite derecho del palindromo actual centrado en en índice 'center'
    if(i < rightBoundary) {
        // si es asi, el valor en la posición i del arreglo de longitudes será igual al valor más pequeño entre lo que tenga el arreglo en el índice mirror y lo máximo que se puede expandir el palíndromo sin sobrepasar el límite derecho; es decir, rightBoundary - i
        P[i] = min(rightBoundary - i, P[mirror]);
    }

    // empezamos a comparar caractéres sin comparar desde los que ya se saben que nos iguales (el paso anterior es para conocer los caracteres que ya no se tienen que comparar)
    while(T[i - (1 + P[i])] == T[i + (1 + P[i])]) {
        P[i]++;
        
    }

    // cehcamos si el limite derecho del palindromo centrado en 'i' es mayor al limite derecho del palindromo centrado en 'center'
    if(i + P[i] > rightBoundary) {
        // si es asi, actualizamos el palindromo central
        center = i;
        rightBoundary = i + P[i];

        // encontramos el palindromo con la maxima longitud para conocer su indice en el arreglo de longitudes P
        if (P[i] > maximumLengthP) {
                maximumLengthP = P[i];
                centerIndex = i;
            }
    }
   }

   // determinamos el indice en el que empieza el palindromo más largo conocido por la longitud más larga que se registra en el vector P
   int startIndex = (centerIndex - maximumLengthP) / 2;
   // determinamos el indice en el que termina el palindromo más largo
   int endIndex = (startIndex + maximumLengthP) - 1;

   // regresamos una tupla con el indice en donde empieza y termina
   return make_tuple(startIndex, endIndex);

}

    void muestraResultado(vector<string>transmissions, vector<string>mcodes) {
        // verificamos si los códigos maliciosos se encuentran dentro de los archivos de transmisión
        for(int i = 0; i < transmissions.size(); i++) {
            cout << "Chequeo del archivo de transmision " << i+1 << "\n";
            for(int j = 0; j < mcodes.size(); j++) {
                int index = KMP(transmissions[i], mcodes[j]);
                if(index == -1) cout << "False" << " " << "El codigo malicioso \"mcode" << j+1 << "\" NO se encuentra en el archivo \"transmission" << i+1 << "\"" << "\n";
                else cout << "True" << " " << "El codigo malicioso \"mcode" << j+1 << "\" se encuentra en el archivo \"transmission" << i+1 << "\" empezando en el indice: " << index << "\n";
            }
            cout << endl;
        }

        cout << endl;

        // verificamos si hay codigo espejeado en los archivos de transmisión
        for(int i=0; i < transmissions.size(); i++) {
                tuple<int,int>result = manacher(transmissions[i]); 
                int start = get<0>(result);
                int end = get<1>(result);
                cout << "Archivo de transimision " << i + 1 << "\n";
                cout << "Posicion inicial del palindromo mas largo encontrado: " << start << "\n";
                cout << "Posicion final del palindromo mas largo encontrado: " << end << "\n";
                cout << endl;
        }

    }


int main() {

    vector<string>transmissions;
    vector<string>mcodes;
    // archivos de transmisión
    string transmission1 = leeArchivo("./transmission1.txt");
    string transmission2 = leeArchivo("./transmission2.txt");

    // archivos de código malicioso
    string mcode1 = leeArchivo("./mcode1.txt");
    string mcode2 = leeArchivo("./mcode2.txt");
    string mcode3 = leeArchivo("./mcode3.txt");

    // agregamos los archivos de transmisión al vector de transmisiones
    transmissions.push_back(transmission1);
    transmissions.push_back(transmission2);

    // agregamos los archivos de codigo malicioso al vector mcodes
    mcodes.push_back(mcode1);
    mcodes.push_back(mcode2);
    mcodes.push_back(mcode3);

    // llamamos la función que muestra los resultados del análisis para cada archivo de transmisión
    muestraResultado(transmissions, mcodes);

    return 0;
}
// Actividad Integradora 1

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

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


int M, N;

int** generaMatrizLcs(string first_text, string second_text) {
    int smallest;
    string str1 = "$" + first_text;
    string str2 = "$" + second_text;
    
    M = str1.length();
    N = str2.length();

    int** mat = new int*[M];

    for(int i = 0; i < M; i++) {
        mat[i] = new int[N];
        mat[i][0] = 0;
    }
    

    for(int i = 0; i < N; i++) {
        mat[0][i] = 0;
    }

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            cout << mat[i][j] << " " << endl;
        }
        cout << endl;
    }

    return mat; 

}

// longest common substring

tuple<int, int> lcs(string first_text, string second_text) {
    int** lcs_mat = generaMatrizLcs(first_text, second_text);
    int max_length = 0;  // Longitud máxima del LCS
    int ending_index = 0;  // Índice de finalización del LCS

    // Itera a través de la matriz para encontrar el LCS
    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            if (first_text[i - 1] == second_text[j - 1]) {
                lcs_mat[i][j] = lcs_mat[i - 1][j - 1] + 1;
                if (lcs_mat[i][j] > max_length) {
                    max_length = lcs_mat[i][j];
                    ending_index = i - 1;
                }
            } else {
                lcs_mat[i][j] = 0;
            }
        }
    }

    // Calcula el índice de inicio del LCS
    int starting_index = ending_index - max_length + 1;

    // Libera la memoria de la matriz
    for (int i = 0; i < M; i++) {
        delete[] lcs_mat[i];
    }
    delete[] lcs_mat;

    return make_tuple(starting_index, ending_index);
}


int main() {

    // vector<int>V = lps("abcabc");

    // for(int i = 0; i < V.size(); i++) {
    //     cout << V[i] << " ";
    // }

    // int index = KMP("xabcabxabcabcxmnsDiUHIABFAODBñCBUbsUFWudbAIUDausISDayduAiugdyusbdIEsabidindaifalfibfisubkjfdbiafbasfa", "ISDay");

    // cout << "pattern is at: " << index;


    // tuple<int,int>result = manacher("ssabccbaiq");
    // // tuple<int,int>result = manacher("bxaeaabababaso");
    // // manacher("lscmxsxaabbaqa");

    // int start = get<0>(result);
    // int end = get<1>(result);
    // cout << endl;

    // std::cout << "start of longest palindrome " << start << std::endl;
    // std::cout << "end of longest palindrome: " << end << std::endl;


    int** t = generaMatrizLcs("abba", "mana");



    return 0;
}
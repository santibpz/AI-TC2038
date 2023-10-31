// Actividad Integradora 1

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

// preprocesamiento del patron con la función que encuentra el sufijo más largo que también sea el prefijo
// longest preffix suffix
vector<int>lps(string pattern) {
    vector<int>result(pattern.size());
    result[0] = 0;

    int i = 1, j = 0;

    while(i < pattern.size()) {
        if(pattern[j] == pattern[i]) {
            result[i] = j + 1;
            i++;
            j++;
        } else {
            if(j == 0) {
                result[i] = 0;
                i++;
            }
            else j = result[j - 1];
        }
    }

    return result;

}

// algoritmo KMP que utiliza el preprocesamiento del patron para saber a donde retroceder
int KMP(string text, string pattern) {
    vector<int>V = lps(pattern); // vector del preprocesamiento del string
    int i = 0, j = 0; 
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

    return -1;


}

int min(int a, int b) {
    if(a <= b) return a;
    else return b;
}

// algoritmo de manacher

tuple<int,int>manacher(string text) {
    // preprocesamiento del texto
    string T = "$#";

    for(int i = 0; i < text.size(); i++) {
        T += text[i];
        T+= "#";
    }

    T+="@";

   // vector con los valores de la longitud de cada palindromo encontrado en el texto
   vector<int>P(T.size());
   int maximumLengthP = 0, centerIndex = 0, center = 0, rightBoundary = 0;

   for(int i = 1; i < T.size() - 1; i++) {
    int mirror = 2*center - i;

    if(i < rightBoundary) {
        P[i] = min(rightBoundary - i, P[mirror]);
    }

    while(T[i - (1 + P[i])] == T[i + (1 + P[i])]) {
        P[i]++;
        
    }

    if(i + P[i] > rightBoundary) {
        center = i;
        rightBoundary = i + P[i];

        // encontramos la palindromo con la maxima longitud para conocer su indice en el arreglo de longitudes P
        if (P[i] > maximumLengthP) {
                maximumLengthP = P[i];
                centerIndex = i;
            }
    }
   }

   int startIndex = (centerIndex - maximumLengthP) / 2;
   int endIndex = (startIndex + maximumLengthP) - 1;
//    cout << "maximumLengthP is: " << maximumLengthP << endl;
//    cout << "start INDEX is: " << startIndex << endl;  // importante START INDEX
//    cout << "end INDEX is: " << endIndex << endl;  // importante START INDEX

   cout << "original string: " << text << endl;

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

// tuple<int, int>lcs(string first_text, string second_text) {
//     int** lcs_mat = generaMatrizLcs(first_text, second_text);

// }

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
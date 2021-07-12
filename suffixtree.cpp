#include <iostream>
#include <vector>
#include <math.h>
#include <string>
using namespace std;


struct suffixNode {
    suffixNode(string s) {
        this->s = s;
    }
    suffixNode(){}

    vector<pair<int, int>> documentos; // .first=documentID .second=ocurrencias del sufijo
    vector<suffixNode*> aristas;
    string s;
};

struct suffixTree {
    int nDocs;
    suffixTree(int nDocumentos) {
        this->nDocs = nDocumentos;
    }
    suffixNode* root=new suffixNode("root");
    void insert(string s, int documentId);
    void printSuffixes(string s, suffixNode* r);
    void print();
    void query(string s);
    void buildSuffixTree();
};

void suffixTree::insert(string s, int documentId) {
    s += "$";
    suffixNode* nodoActual = root;

    if (root->aristas.size() == 0) {//caso que root no tiene aristas
        suffixNode* a = new suffixNode(s);
        nodoActual->aristas.push_back(a);
        nodoActual->aristas[nodoActual->aristas.size()-1]->documentos.push_back(make_pair(documentId, 1));
        //cout << "return 1 " << endl;
        return;
    }
    int indiceS=0;
    while (true) {

        int i;//indice para el vector
        bool br = false;
        for (i = 0; i < nodoActual->aristas.size(); i++) {
            if (nodoActual->aristas[i]->s[0] == s[indiceS]) {
                nodoActual = nodoActual->aristas[i];
                br = true;
                break;
            }
        }
        if (i == nodoActual->aristas.size() && nodoActual->s[nodoActual->s.size()-1]!='$' && !br) {//caso que esta en nodo y no encuentra la primera letra
            string temp = s.substr(indiceS);
            suffixNode* a = new suffixNode(temp);
            nodoActual->aristas.push_back(a);
            nodoActual->aristas[nodoActual->aristas.size() - 1]->documentos.push_back(make_pair(documentId, 1));
            //cout << "return 2 " << endl;
            return;
        }
        //estoy en el nodo con la primera letra igual

        //si ya estaba indexada la palabra y encuentra el nodo hoja, busca el documento
        //si ya estaba en el vector documentos, si no esta crea uno para el id que se
        //tiene
        if (s.substr(indiceS, nodoActual->s.size()) == nodoActual->s) {
            if (nodoActual->s[nodoActual->s.size() - 1] == '$') {
                for (int k = 0; k < nodoActual->documentos.size(); k++) {
                    if (nodoActual->documentos[k].first == documentId) {
                        nodoActual->documentos[k].second++;
                        //cout << "return 3 " << endl;
                        return;
                    }
                }
                nodoActual->documentos.push_back(make_pair(documentId, 1));
                //cout << "return 4 " << endl;
                return;
            }
            indiceS += nodoActual->s.size();
            continue;
        }
        int j, indiceAux=0;
        for (j = indiceS; j < nodoActual->s.size() && j < s.size(); j++, indiceAux++) {
            if (nodoActual->s[j] != s[indiceAux]) {
                break;
            }
        }

        suffixNode* aux = new suffixNode(nodoActual->s.substr(indiceAux));
        //cout << "s.substr  " << nodoActual->s.substr(indiceAux) << endl;
        aux->aristas = nodoActual->aristas;
        aux->documentos = nodoActual->documentos;
        nodoActual->aristas.clear();
        nodoActual->documentos.clear();
        nodoActual->aristas.push_back(aux);
        suffixNode* aux2 = new suffixNode(s.substr(j));
        nodoActual->aristas.push_back(aux2);
        nodoActual->s.erase(indiceAux, nodoActual->s.size() - indiceAux);
        //cout << "return 5 " << endl;
        return;

    }

}

void suffixTree::print() {
    string s = "";
    for (int i = 0; i < root->aristas.size(); i++) {
        printSuffixes(s, root->aristas[i]);
    }
}

void suffixTree::printSuffixes(string s, suffixNode* r) {
    if (r->aristas.size() == 0) {
        cout << "suffix :"<< s + r->s << endl;
        return;
    }
    for (int i = 0; i < r->aristas.size(); i++) {
        //cout << s + r->s << endl;
        //cout << "enviando i: " << i << endl;
        printSuffixes(s+r->s, r->aristas[i]);
    }
}

void suffixTree::query(string s) {
    s += "$";
    suffixNode* nodoActual = root;
    if (root->aristas.size() == 0) {//caso que root no tiene aristas
        cout << "return 1 " << endl;
        return;
    }
    int indiceS = 0;
    while (true) {

        int i;//indice para el vector
        for (i = 0; i < nodoActual->aristas.size(); i++) {
            if (nodoActual->aristas[i]->s[0] == s[indiceS]) {
                nodoActual = nodoActual->aristas[i];
                break;
            }
        }
        if (i == nodoActual->aristas.size() && nodoActual->s[nodoActual->s.size() - 1] != '$') {
            cout << "return 2 " << endl;
            return;
        }
        
        if (s.substr(indiceS, nodoActual->s.size()) == nodoActual->s) {
            //cout << "nodo actual " << nodoActual->s << endl;
            //cout << "substr " << s.substr(indiceS, nodoActual->s.size()) << endl;
            //cout << nodoActual->s[s.size() - 1] << " caracter" << endl;
            if (nodoActual->s[nodoActual->s.size() - 1] == '$') {
                //encontro la palabra
                //aqui imprimir contenidos //////////////////////////////////////////
                cout << "cantidad de documentos en los que aparece : " << nodoActual->documentos.size() << endl;
                for (int cont = 0; cont < nodoActual->documentos.size(); cont++) {
                    cout << "document id: " << nodoActual->documentos[cont].first << endl
                     << " puntuacion: " << nodoActual->documentos[cont].second * log(nDocs / nodoActual->documentos.size()) << endl;
                }
                //cout << "return 3 " << endl;
                return;
            }
            //sigue avanzando, es igual la cadena pero todavia no termina el suffijo
            indiceS += nodoActual->s.size();
            //cout << "CONTINUE " <<  "   indiceS: " << indiceS << endl;
            continue;
        }
        else {
            //no encontro ocurrencia en el arbol
           // cout << "return 4 " << endl;
            return;
        }

    }

}

void suffixTree::buildSuffixTree() {

}

int main()
{
    
    suffixTree t(4);
    t.insert("a", 1);
    t.insert("a", 1);
    t.insert("a", 1);
    t.insert("a", 2);
    t.insert("a", 3);
    t.insert("a", 4);
    t.insert("ba", 1);
    t.insert("aba", 1);
    t.insert("baba", 1);
    t.insert("ababa", 1);
    t.insert("ca", 1);
    t.insert("aca", 1);
    t.insert("xasda", 1);
    t.insert("xddsa", 1);
    t.print();
    t.query("a");
}

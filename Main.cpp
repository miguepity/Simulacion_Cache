#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
/*  Bytes en la Cache
      0->Valido
      1->Modificado
      2->Etiqueta
      3->Datos        */
double tiempo;
unsigned char cache[64][8];
unsigned char ram[4096];
unsigned char leer(int,int);
void Ordenar(int);
void escribir(int,int,int);
int n, k, m, etiqueta, puntero;

int main(int argc, char *argv[]){
  //Inicializacion de variables
  puntero=0;
  tiempo=0.00;
  n = 5;
  k = 8;
  m = 64;
  fstream input;
  input.open("Datos.txt", ios::in);
  string str;
  int i=0;
  //Se cargan a ram
  while (getline(input, str)){
    ram[i]=atoi(str.c_str());
    i++;
  }

  cout<<"Sin Cache"<<endl;
  Ordenar(0);
  cout<<"Memoria Directa"<<endl;
  Ordenar(1);
  cout<<"Memoria Asociativa"<<endl;
  Ordenar(2);

  return 0;
}



unsigned char leer(int i, int tipo){
  int b = -1, l = -1, etiqueta = -1;
  if(tipo==0){
    //Cuando es sin cache
    tiempo+=0.1;
    return ram[i];
  }else if(tipo==1){
    //Cuando es Directo
    int b = i/k;
    int l = b%m;
    etiqueta = b/m;
    if(int(cache[i][0])==0){
      //Cuando es invalido - NO ESTA EN CACHE
      tiempo+=0.11;
      //Valido
      cache[l][0]=1;
      //Etiqueta
      cache[l][2]=etiqueta;
      //Modificado
      cache[l][1]=0;
    }else{
      //Cuando es valido - ESTA EN CACHE
      if (int(cache[l][2]) == etiqueta) {
        //Cuando la etiqueta coincide - ESTA EN CACHE
        tiempo += 0.01;
      }else{
        //Cuando la etiqueta coincide - NO ESTA EN CACHE
        if (int(cache[l][1])==1){
          //Cuando esta modificada
          tiempo += 0.22;
        } else {
          //Cuando no esta modificada
          tiempo += 0.11;
        }
        cache[l][2]=etiqueta;
        cache[l][0]=1;
        cache[l][1]=0;
      }
    }
    return ram[i];
  }else if(tipo==2){
    //Cuando es Asociativa
    etiqueta = i / k;
    if(puntero>=8){
      puntero=0;
    }
    if(int(cache[puntero][0])==0){
      //Cuando es invalido - NO ESTA EN CACHE
      tiempo+=0.11;
      //Valido
      cache[puntero][0]=1;
      //Etiqueta
      cache[puntero][2]=etiqueta;
      //Modificado
      cache[puntero][1]=0;
    }else{
      //Cuando es valido - ESTA EN CACHE
      if (int(cache[puntero][2]) == etiqueta) {
        //Cuando la etiqueta coincide - ESTA EN CACHE
        tiempo += 0.01;
      }else{
        //Cuando la etiqueta coincide - NO ESTA EN CACHE
        if (int(cache[puntero][1])==1){
          //Cuando esta modificada
          tiempo += 0.22;
        } else {
          //Cuando no esta modificada
          tiempo += 0.11;
        }
        cache[puntero][2]=etiqueta;
        cache[puntero][0]=1;
        cache[puntero][1]=0;
      }

    }
    return ram[i];
    puntero++;

  }else if(tipo==3){
    //Cuando es Asociativa por Conjunto









  }else{
    cout<<"Tipo invalido"<< endl;
  }
  return 0;
}

void escribir(int i, int tipo, int valor){
  if(tipo==0){
    //Cuando es sin cache
    ram[i]=valor;
    tiempo+=0.1;
  }else if(tipo==1){
    //Cuando es Directo
    int b = i/k;
    int l = b%m;
    int etiqueta = b/m;
    if(int(cache[i][0])==0){
      //Cuando es invalido - NO ESTA EN CACHE
      tiempo+=0.11;
      //Modificado
      cache[l][1]=1;
    }else{
      //Cuando es valido - ESTA EN CACHE
      if (cache[l][2] == etiqueta) {
        //Cuando coincide con la etiqueta - ESTA EN CACHE
        tiempo += 0.01;
        //Modificado
        cache[l][1] = 1;
      } else {
        //Cuando no coincide con la etiqueta - NO ESTA EN CACHE
        if (cache[l][1]==1) {
          //Cuando esta modificada - HAY QUE GUARDAR
          tiempo += 0.22;
        } else {
          //Cuando esta modificada - SE ESCRIBE EN CACHE
          tiempo += 0.11;
        }
        cache[l][0] = 1;
        cache[l][2] = etiqueta;
        cache[l][1] = 1;
      }
    }
    ram[i]=valor;
  }else if(tipo==2){
    //Cuando es Asociativa
    etiqueta = i/k;
    if(puntero>=8){
      puntero=0;
    }
    if(int(cache[puntero][0])==0){
      //Cuando es invalido - NO ESTA EN CACHE
      tiempo+=0.11;
      //Modificado
      cache[puntero][1]=1;
    }else{
      //Cuando es valido - ESTA EN CACHE
      if (cache[puntero][2] == etiqueta) {
        //Cuando coincide con la etiqueta - ESTA EN CACHE
        tiempo += 0.01;
        //Modificado
        cache[puntero][1] = 1;
      } else {
        //Cuando no coincide con la etiqueta - NO ESTA EN CACHE
        if (cache[puntero][1]==1) {
          //Cuando esta modificada - HAY QUE GUARDAR
          tiempo += 0.22;
        } else {
          //Cuando esta modificada - SE ESCRIBE EN CACHE
          tiempo += 0.11;
        }
        cache[puntero][0] = 1;
        cache[puntero][2] = etiqueta;
        cache[puntero][1] = 1;
      }
    }
    ram[i]=valor;
    puntero++;
  }else if(tipo==3){
    //Cuando es Asociativa por Conjunto










  }else{
    cout<<"Tipo invalido"<< endl;
  }
}

void Ordenar(int tipo){
  //Se inicializa/limpia la cache
  for (int i = 0; i < 64; i++){
    for (int j = 0; j < 8; j++){
      cache[i][j]=0;
    }
  }
  //Se ordena dependiendo del tipo
  unsigned char temp;
  for (int i=0;i<=n-2;i++){
    for (int j=i+1;j<=n-1;j++){
      if (leer(i,tipo)>leer(j,tipo)){
        temp=leer(i,tipo);
        escribir(i, tipo,leer(j,tipo));
        escribir(j,tipo,temp);
      }
    }
  }
 // Imprime la matriz
// for (int i = 0; i < 4096; i++){
//   cout<<int(ram[i])<<endl;
// }
  cout<<"tiempo: "<<tiempo<<" μs"<<endl;
  tiempo=0;
}

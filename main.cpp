#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include <chrono>

using namespace std;

struct punkt
{
    int x_punktu;
    int y_punktu;
    float wartosc_f;
    int koszt_dojscia;

};

const int rozmiar_mapy = 25;
int mapa[rozmiar_mapy][rozmiar_mapy];
int cel_x = 24;
int cel_y = 24;
int start_x = 0;
int start_y = 0;
bool wart=true;
int index=0;
string fileName = "inputs/25x25-open.txt";


vector<punkt> lista_otwarta;
vector<punkt> lista_zamknieta;
char tablica_rodzicow[rozmiar_mapy][rozmiar_mapy];


void wczyt(string nazwa_pliku)
{
    ifstream plik(nazwa_pliku.c_str());
    for(int i = 0; i < rozmiar_mapy; i++)
    {
        for(int j = 0; j < rozmiar_mapy; j++)
            {
            plik >> mapa[i][j];
            }
    }
    plik.close();

     for(int i = 0; i < rozmiar_mapy; i++)
    {
        for(int j = 0; j < rozmiar_mapy; j++)
        tablica_rodzicow[i][j]='0';

    }
}

void zmien_kolor(int numer_kol)
{
    HANDLE wyj;
    wyj = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(wyj,numer_kol);
}

void wyswietl()
{
    for(int i = 0; i < rozmiar_mapy; i++)
    {
        for(int j = 0; j < rozmiar_mapy; j++)
        {
             if(mapa[i][j]==3)
             {
                 zmien_kolor(4);
                 cout << mapa[i][j] << " ";
             }
             if(mapa[i][j]==5)
             {
                 zmien_kolor(6);
                 cout << mapa[i][j] << " ";
             }
             if(mapa[i][j]==0)
             {
                 zmien_kolor(3);
                 cout << mapa[i][j] << " ";
             }
        }
    cout << "\n";
    }
}

int wylicz_koszt(int x_start,int y_start,int x,int y)
{
    int koszt=0;
    bool wart=true;
    while( wart )
    {
        if(x_start==x && y_start==y ){
            wart=false;
            return koszt;
        }else{
            if(tablica_rodzicow[x][y]=='a')
            {
              x++;

            }else if(tablica_rodzicow[x][y]=='b')
            {
              y--;

            }else if(tablica_rodzicow[x][y]=='c')
            {
              x--;

            }else if(tablica_rodzicow[x][y]=='d')
            {
              y++;
            }
            koszt++;
        }
    }
    return koszt;
}


bool czy_na_liscie(vector<punkt> lista,int x,int y)
{
    for(int i=0;i<lista.size();i++)
    {
        if(lista[i].x_punktu==x && lista[i].y_punktu==y)
        {
            return true;
        }
    }
    return false;

}

punkt najmniejsze_f(){
	punkt najmniejsze = lista_otwarta[0];

	for(int i = 0; i < lista_otwarta.size(); i++)
	{
		if(lista_otwarta[i].wartosc_f < najmniejsze.wartosc_f){
			najmniejsze = lista_otwarta[i];
			break;
		}
	}
	return najmniejsze;
}


float heurystyka(int poz_x, int poz_y,int koszt)
{
    return sqrt(pow(poz_x-cel_x,2)+pow(poz_y-cel_y,2))+koszt;
}

void dodaj_potomka(int x,int y)
{
    punkt Otwarte;

    //dó³
    if(x+1<rozmiar_mapy && mapa[x+1][y]!=5 && czy_na_liscie(lista_zamknieta,x+1,y)==0)
    {
        if(tablica_rodzicow[x+1][y]=='0')
        {
            tablica_rodzicow[x+1][y]='c'; //c => góra
        }

        Otwarte.x_punktu=x+1;
        Otwarte.y_punktu=y;
        Otwarte.koszt_dojscia=wylicz_koszt(start_x,start_y,x+1,y);
        Otwarte.wartosc_f=heurystyka(x+1,y,Otwarte.koszt_dojscia);
        lista_otwarta.push_back(Otwarte);

    }

    //lewa
    if(y-1>=0 && mapa[x][y-1]!=5 && czy_na_liscie(lista_zamknieta,x,y-1)==0)
    {
        if(tablica_rodzicow[x][y-1]=='0')
        {
        tablica_rodzicow[x][y-1]='d'; //d => prawa
        }

        Otwarte.x_punktu=x;
        Otwarte.y_punktu=y-1;
        Otwarte.koszt_dojscia=wylicz_koszt(start_x,start_y,x,y-1);
        Otwarte.wartosc_f=heurystyka(x,y-1,Otwarte.koszt_dojscia);
        lista_otwarta.push_back(Otwarte);

    }

    //góra
    if(x-1>=0 && mapa[x-1][y]!=5 && czy_na_liscie(lista_zamknieta,x-1,y)==0)
    {
        if(tablica_rodzicow[x-1][y]=='0')
        {
           tablica_rodzicow[x-1][y]='a'; //a => dó³
        }

        Otwarte.x_punktu=x-1;
        Otwarte.y_punktu=y;
        Otwarte.koszt_dojscia=wylicz_koszt(start_x,start_y,x-1,y);
        Otwarte.wartosc_f=heurystyka(x-1,y,Otwarte.koszt_dojscia);
        lista_otwarta.push_back(Otwarte);

    }

    //prawa
    if(y+1<rozmiar_mapy && mapa[x][y+1]!=5 && czy_na_liscie(lista_zamknieta,x,y+1)==0)
    {
        if(tablica_rodzicow[x][y+1]=='0')
        {
            tablica_rodzicow[x][y+1]='b'; //b => lewa
        }

        Otwarte.x_punktu=x;
        Otwarte.y_punktu=y+1;
        Otwarte.koszt_dojscia=wylicz_koszt(start_x,start_y,x,y+1);
        Otwarte.wartosc_f=heurystyka(x,y+1,Otwarte.koszt_dojscia);
        lista_otwarta.push_back(Otwarte);

    }
}

void dodaj_zamknieta()
{
   punkt wartosc;

   wartosc = najmniejsze_f();

   for(int i=0; i<lista_otwarta.size(); i++)
    {
        if(lista_otwarta[i].x_punktu == wartosc.x_punktu && lista_otwarta[i].y_punktu == wartosc.y_punktu)
        {
            lista_otwarta.erase(lista_otwarta.begin()+i);
        }
   }

   lista_zamknieta.push_back(wartosc);

}

int kroki = 0;
void wynik(int x_start,int y_start,int x,int y)
{
    bool wart=true;

    while( wart )
    {
    if(x_start==x && y_start==y ){

        wart=false;

    }else if(tablica_rodzicow[x][y]=='a')
    {
        mapa[x][y]=3;
        x++;

    }else if(tablica_rodzicow[x][y]=='b')
    {
         mapa[x][y]=3;
         y--;

    }else if(tablica_rodzicow[x][y]=='c')
    {
         mapa[x][y]=3;
         x--;

    }else if(tablica_rodzicow[x][y]=='d')
    {
         mapa[x][y]=3;
         y++;
    }
    kroki++;
    }
}

void algorytm()
{
    wczyt(fileName);
    wyswietl();

    while(wart){
        zmien_kolor(7);
        /*cout<<"Podaj kordy startowe (x,y): ";
        cin >> start_x;
        cin >> start_y;*/

        if(start_x>=0 && start_x<rozmiar_mapy && start_y>=0 && start_y<rozmiar_mapy && mapa[start_x][start_y]!=5 )
        {
         /*   cout<<"\nPodaj kordy koncowe (x,y): ";
            cin >> cel_x;
            cin >> cel_y;*/
            if(cel_x>=0 && cel_x<rozmiar_mapy && cel_y>=0 && cel_y<rozmiar_mapy && mapa[cel_x][cel_y]!=5)
            {

                punkt start;
                start.x_punktu=start_x;
                start.y_punktu=start_y;
                lista_zamknieta.push_back(start);
                mapa[start.x_punktu][start.y_punktu]=3;
                dodaj_potomka(start.x_punktu,start.y_punktu);

                cout<<"\n";

                do
                {
                    if(lista_otwarta.size()==0)
                    {

                    cout<<"Brak mozliwosci dojscia do celu.\n\n";
                    exit(0);
                    }

                ++index;
                dodaj_zamknieta();
                dodaj_potomka(lista_zamknieta[index].x_punktu,lista_zamknieta[index].y_punktu);

                }while(!czy_na_liscie(lista_zamknieta,cel_x,cel_y));

                cout<<"\n\n";

                wynik(start.x_punktu,start.y_punktu,cel_x,cel_y);
                system("cls");
                wyswietl();
                zmien_kolor(7);
                cout<<"\nStart ("<<start_x<<","<<start_y<<") || ";
                cout<<"Cel ("<<cel_x<<","<<cel_y<<")";
                wart=false;
            }
            else
            {
                system("cls");
                wyswietl();
            }
        }
        else
        {
            system("cls");
            wyswietl();
        }
    }
}

int main()
{
    auto start = chrono::steady_clock::now();
    algorytm();
    auto stop = chrono::steady_clock::now();
    auto czas = chrono::duration_cast<chrono::microseconds>( stop - start ).count();
    cout << "\n" << "Ilosc krokow: " <<kroki;
    cout << "\n" << "Czas wykoniania: " << (float)czas / (float)pow(10,6) << "s";
    return 0;
}

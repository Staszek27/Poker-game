#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
typedef long long ll;

const ll  S = (ll) 1e17;
const unsigned int B = 1000 * 1000;
const int stala_symulacji = 1000;
const pair <int, int> board_size = {35, 100};
const bool debug = false;
int itt;
struct Stol;
int bot_michala(int id_gracza, int ktora_runda, Stol &S);
int bot_artura(int id_gracza, int ktora_runda, Stol &S);

int rnd(int poc, int kon){
    return poc + rand() % (kon - poc + 1);
}

string str(int x){
    if (x == 0)
        return "0";
    string res = "";
    while (x){
        res += (char) (x % 10) + '0';
        x /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

struct karta{
    string kolor;
    string figura; 
    
    
    // UWAGA, MOZLIWE ZE W KONSTRUKTORACH NALEZY DAC PUBLIC
    karta(string x, string y){
        kolor  = x;
        figura = y;
    }
    
    karta(int X){
        if (! (0 <= X and X <= 51)){
            cout << "Nie ma karty o numerze: " << X << endl;
            exit(0);
        }
        
        if (X / 13 == 0) kolor = "pik"; 
        if (X / 13 == 1) kolor = "trefl"; 
        if (X / 13 == 2) kolor = "karo"; 
        if (X / 13 == 3) kolor = "kier"; 
        
        if (X % 13 <= 7){
            figura = "";
            figura += (char) (X % 13) + '2'; 
        } 
        if (X % 13 == 8)  figura = "10";
        if (X % 13 == 9)  figura = "J";
        if (X % 13 == 10) figura = "Q";
        if (X % 13 == 11) figura = "K";
        if (X % 13 == 12) figura = "A";
    }
    
    int id_karty(){
        int res = 0;
        res += (kolor == "pik")   * 0 * 13;
        res += (kolor == "trefl") * 1 * 13;
        res += (kolor == "karo")  * 2 * 13;
        res += (kolor == "kier")  * 3 * 13;
        
        res += (figura == "2")  * 0;
        res += (figura == "3")  * 1;
        res += (figura == "4")  * 2;
        res += (figura == "5")  * 3;
        res += (figura == "6")  * 4;
        res += (figura == "7")  * 5;
        res += (figura == "8")  * 6;
        res += (figura == "9")  * 7;
        res += (figura == "10") * 8;
        res += (figura == "J")  * 9;
        res += (figura == "Q")  * 10;
        res += (figura == "K")  * 11;
        res += (figura == "A")  * 12;
        
        return res;  
    }
    
    int moc_karty(){
        return id_karty() % 13;
    }
    
    string nazwa(){
        return figura + " " + kolor;
    }
};

struct ulozenie{ 
    int id_gracza;
    ll moc = -1;
    string nazwa;
    vector <karta> karty;
        
    vector <int> zlicz;
    vector <pair <int, int> > X;
    
    ulozenie(vector <karta> Z, int x){
        
        karty = Z;
        
        // pre-processing
        
        zlicz.resize(13);
        for (int i = 0; i < 13; i ++)
            zlicz[i] = 0;
        
        for (auto e : karty)
            zlicz[e.moc_karty()] ++;
        
        for (int i = 0; i < karty.size(); i ++)
            X.push_back({karty[i].id_karty(), i});
        sort(X.begin(), X.end());
        
        // end of pre-processing 
        
        (
            pokor()  or 
            kareta() or
            full()   or
            kolor()  or
            strit()  or
            trojka() or
            para2()  or
            para()   or
            wysoka()
        ); 
        id_gracza = x;
    }
    
    bool pokor(){    
        int ile = 0, prev = 0, border;
        
        for (int i = X.size() - 1; i >= 0; i --){
            int x = X[i].first;
            int y = X[i].second;
            
            if (ile == 0 or border > x or prev != x + 1){
                border = x / 13 * 13; 
                prev = x;
                ile = 1;
                continue;
            }
            
            ile ++;
            prev = x;
            if (ile == 5){
                nazwa = "poker " + karty[y].kolor + " od " + karty[y].figura;
                moc   = S * 8 + karty[y].moc_karty() * (S / 100); 
                return true;
            }
        }
        
        return false;
    }
    
    
    bool kareta(){
        for (int i = 0; i < 13; i ++)
            if (zlicz[i] == 4)  
                for (int j = 12; j >= 0; j --)
                    if (zlicz[j] > 0 and zlicz[j] < 4){
                        nazwa = "kareta " + karta(i).kolor;
                        moc   = S * 7 + i * (S / 100) + j * (S / 10000); 
                        return true;     
                    }
            
        return false;
    }
    
    
    bool full(){
        for (int i = 12; i >= 0; i --)
            if (zlicz[i] >= 3)
                for (int j = 12; j >= 0; j --)
                    if (i != j and zlicz[j] >= 2){
                        nazwa = "full " + karta(i).figura + " na " + karta(j).figura;
                        moc   =  S * 6 + i * (S / 100) + j * (S / 10000);
                        return true;
                    }
        return false;                    
    }
    
    
    bool kolor(){
        int ile = 0, border;
        
        for (int i = X.size() - 1; i >= 0; i --){
            int x = X[i].first;
            int y = X[i].second;
            
            if (ile == 0 or border > x){
                border = x / 13 * 13; 
                ile = 1;
                continue;
            }
            
            ile ++;
            if (ile == 5){
                nazwa = "kolor " + karty[y].kolor;
                moc   = S * 5 + karty[X[i + 4].second].moc_karty() * (S / (ll) 100)
                              + karty[X[i + 3].second].moc_karty() * (S / (ll) 10000)
                              + karty[X[i + 2].second].moc_karty() * (S / (ll) 1000000)
                              + karty[X[i + 1].second].moc_karty() * (S / (ll) 100000000); 
                              + karty[X[i + 0].second].moc_karty() * (S / (ll) 10000000000); 
                return true;
            }
        }
        return false;
    }
    
    bool strit(){
        int act = 0;
        for (int i = 12; i >= 0; i --){
            if (zlicz[i] == 0){
                act = 0;
                continue;
            }
                
            
            act ++;
            if (act == 5){
                nazwa = "strit od " + karta(i).figura;
                moc   = S * 4 + i * (S / (ll) 100);
                return true; 
            }
        }
        return false;
    }
    
    bool trojka(){
        for (int i = 12; i >= 0; i --)
            if (zlicz[i] >= 3){
                vector <int> kickery;
                for (auto e : karty)
                    if (e.moc_karty() != i)
                        kickery.push_back(e.moc_karty());
                
                sort(kickery.begin(), kickery.end());
                
                nazwa = "trojka " + karta(i).figura;
                moc = S * 3 + i * (S / (ll) 100)
                            + kickery[kickery.size() - 1] * (S / (ll) 10000)
                            + kickery[kickery.size() - 2] * (S / (ll) 1000000); 
                return true;    
            }
        return false;
    }
    
    bool para2(){
        for (int i = 12; i >= 0; i --)
            if (zlicz[i] >= 2)
                for (int j = i - 1; j >= 0; j --)
                    if (zlicz[j] >= 2){
                        vector <int> kickery;
                        for (auto e : karty)
                            if (e.moc_karty() != i and e.moc_karty() != j)
                                kickery.push_back(e.moc_karty());
                                
                        sort(kickery.begin(), kickery.end());
                                
                        nazwa = "dwie pary " + karta(i).figura + " & " + karta(j).figura;
                        moc    = S * 2 + i * (S / (ll) 100)
                                       + j * (S / (ll) 10000)
                                       + kickery[kickery.size() - 1] * (S / (ll) 1000000);
                        return true;
                    }
        return false;
    }
    
    bool para(){
        for (int i = 12; i >= 0; i --)
            if (zlicz[i] >= 2){
                vector <int> kickery;
                for (auto e : karty)
                    if (e.moc_karty() != i)
                        kickery.push_back(e.moc_karty());
                        
                sort(kickery.begin(), kickery.end());  
                      
                nazwa = "para " + karta(i).figura;
                moc   = S * 1 + i * (S / (ll) 100)
                              + kickery[kickery.size() - 1] * (S / (ll) 10000)
                              + kickery[kickery.size() - 2] * (S / (ll) 1000000)
                              + kickery[kickery.size() - 3] * (S / (ll) 100000000);
                return true;
            }
       return false;
    }
    
    bool wysoka(){
        for (int i = 12; i >= 0; i --)
            if (zlicz[i]){
                vector <int> kickery;
                for (int j = i - 1; kickery.size() != 4; j --)
                    if (zlicz[j])
                        kickery.push_back(j);
                        
                sort(kickery.begin(), kickery.end());
                
                nazwa = "wysoka karta " + karta(i).figura;
                moc   = S * 0 + i * (S / (ll) 100)
                              + kickery[kickery.size() - 1] * (S / (ll) 10000)
                              + kickery[kickery.size() - 2] * (S / (ll) 1000000)
                              + kickery[kickery.size() - 3] * (S / (ll) 100000000)
                              + kickery[kickery.size() - 4] * (S / (ll) 10000000000);
                return true;
            }
        cout << "Blad!!!!! nie wybrano zadnego ulozenia";
        exit(0);
    }
    
    bool operator<(const ulozenie X){
        return (this->moc) > (X.moc);
    }
};

struct gracz{
    string nazwa;
    int wartosc_stacka;
    int id_gracza; // gdzie siedzi
    int liczba_pozostalych_graczy;
    int trudnosc_bota;
    bool uzyte_karty[52];
    double szansa; // [0, 1]
    string nazwa_bota;
    bool bot;
    bool *spasowali;
    bool aktualnie_w_grze;
    vector <vector <int> > szanse; /*/ dla kazdego gracza szanse jaka ma karte
                                       poczatkowo:
                                            {{0.19, 0.19, ... , 0.19},
                                             {0.19, 0.19, ... , 0.19}} /*/
                               
    vector <karta> karty;
    
    gracz(string x, int a, int y, int z, int T, bool B, vector <karta> K){
        nazwa = x;
        nazwa_bota = x;
        wartosc_stacka = a;
        id_gracza = y;
        liczba_pozostalych_graczy = z;
        trudnosc_bota = T;
        szansa = 1.0;
        bot = B;
        aktualnie_w_grze = true;
        spasowali = new bool[liczba_pozostalych_graczy];
        for (int i = 0; i < liczba_pozostalych_graczy; i ++)
            spasowali[i] = 0;
            
        vector <int> X = {};
        for (int i = 0; i < 52; i ++)
            X.push_back(1.0 / 52.0);
        
        for (int i = 0; i < liczba_pozostalych_graczy; i ++)
            szanse.push_back(X);        
        
        karty = K;
    }
    
    bool all_in(){
        return (wartosc_stacka == 0);
    }
    
};

struct Stol{
    vector <gracz> gracze;
    vector <karta> karty_na_stole;
    vector <karta> karty_wolne;
    vector <int> stacki; // ile kto dal
        // 1. lista osob ktore wchodza za dana pule
        // 2. jaka to pula
    
    int kto_rozdaje; // zeby wiedziec jak ida blindy
    int big_blind;   // ile za wejscie
    int ktora_tura;  // ktora aktualnie tura  
    int czyja_kolej; // żeby wiedzieć kogo displayowac
    
    bool deb  = debug;
    bool deb2 = debug;
    Stol(vector <pair <bool, pair <string, int> > > G, int ile_na_start, int startowy_blind){ 
        int cnt = 0;
        for (auto e : G){
            gracze.push_back(gracz(
                                e.second.first, 
                                ile_na_start, 
                                cnt, 
                                G.size() - 1, 
                                e.second.second,
                                e.first, 
                                {}
            ));
            cnt ++;
        }
        karty_na_stole = {};
        karty_wolne    = {};         
        kto_rozdaje    = 0;
        big_blind      = startowy_blind;
        ktora_tura     = 0;
        czyja_kolej    = 0;
        odswiez();
    }
    
    void legenda(){
        cout << "Jesli chcesz podbic o kwote x uzyj napisz: podbij x\n";
        cout << "Jesli chcesz sprawdzic lub czekac napisz:  sprawdz\n";
        cout << "Jesli chcesz spasowac napisz:              pas\n\n";
        cout << "Podaj swoj ruch: " << endl;
    }
    
    void interakcja(int x, bool spij = true){
        if(!spij){
            cout << " gracz " << gracze[x].nazwa << " spasowal.\n";
        //    usleep(B);
            return;
        }
        if (!debug) usleep(B);
        system("clear");
        cout << "ruch gracza " << gracze[x].nazwa << endl;
        if (!debug){
            cout << " napisz cokolwiek aby kontynuowac..";
            string xx; cin >> xx;
        }
        pokaz_stol();
    }
    
    int pula(){
        int res = 0;
        for (auto e : stacki)
            res += e;
        return res;
    }
    
    int max_stack(){
        int ans = 0;
        for (auto e : stacki)
            ans = max(ans, e);
        return ans;
    }
    
    int ile_gra(){
        int res = 0;
        for (auto e : gracze)
            res += (e.aktualnie_w_grze);
        return res;
    }
    
    int aktywni(){
        int res = 0;
        for (auto e : gracze)
            res += !e.all_in();
        return res;
    }
    
    string C(int c, char z){
        string res = "";
        for (int i = 0; i < c; i ++)
            res += z;
        return res;
    }
    
    void pokaz_stol(bool the_end = false){
        vector <string> T;
        for (int i = 0; i < board_size.first; i ++){
            string cand;
            for (int j = 0; j < board_size.second; j ++){
                if (i == 0 or i == board_size.first - 1)
                    cand += "_";
                else if (j == 0 or j == board_size.second - 1)
                    cand += "|";
                else
                    cand += " ";
            }
            T.push_back(cand);
        }
        
        vector <string> player_grid = {"            TOTAL BET",
                                       "                     ",
                                       "                     ",
                                       "   __   __           ",
                                       "  |  | |  |    $$$   ",
                                       "  |  | |  |          ",
                                       "  |__| |__|    $$$   ",};
      
        vector <string> Cards(4), Card  = {" __ ",
                                           "|  |",
                                           "|  |",
                                           "|__|"};
                                  
        for (int i = 0; i < karty_na_stole.size(); i ++){
            Card[2][1] = karty_na_stole[i].figura[0];
            if (karty_na_stole[i].figura.length() > 1)
                Card[2][2] = karty_na_stole[i].figura[1];
                
            Card[1][1] = (karty_na_stole[i].kolor == "pik")   * 1 +  
                         (karty_na_stole[i].kolor == "trefl") * 2 +
                         (karty_na_stole[i].kolor == "karo")  * 3 +
                         (karty_na_stole[i].kolor == "kier")  * 4 ;   
            
            for (int j = 0; j < Card.size(); j ++)
                for (int o = 0; o < Card[j].length(); o ++)
                    Cards[j] += Card[j][o];
                
            Card[2][1] = Card[2][2] = Card[1][1] = ' '; 
        } 
        
        pair <int, int> player_grid_size = {player_grid.size(), player_grid[0].length()};
        vector <pair <double, double> > places = {{1.0 / 4.0, 1.3 / 4.0}, 
                                                  {1.0 / 4.0, 2.7 / 4.0},
                                                  {1.0 / 2.0, 4.0 / 5.0}, 
                                                  {3.0 / 4.0, 2.7 / 4.0},
                                                  {3.0 / 4.0, 1.3 / 4.0}, 
                                                  {1.0 / 2.0, 1.0 / 5.0}}; // escape codes \ escape sequnces
        
        for (int i = 0; i < Cards.size(); i ++)
            for (int j = 0; j < Cards[i].length(); j ++)
                T[board_size.first / 2 - 2 + i][board_size.second / 2 - 13 + j] = Cards[i][j]; 
        
        for (int i = 0; i < places.size(); i ++)
            places[i] = {(int) (places[i].first  * board_size.first)  - player_grid_size.first  / 2,
                         (int) (places[i].second * board_size.second) - player_grid_size.second / 2}; 
        
        for (int i = 0; i < gracze.size(); i ++)
            for (int j = 0; j < player_grid.size(); j ++)
                for (int o = 0; o < player_grid[j].length(); o ++)
                    T[j + places[i].first][o + places[i].second] = player_grid[j][o];
        
        for (int o = 0; o < gracze.size(); o ++)
            for (int j = 0; j < gracze[o].karty.size(); j ++){
                for (int i = 0; i < gracze[o].karty[j].figura.length(); i ++)
                    if (deb or o == czyja_kolej or the_end) 
                        T[places[o].first + player_grid_size.first - 2][places[o].second + 3 + j * 5 + i] 
                        = gracze[o].karty[j].figura[i];  
                if (deb or o == czyja_kolej or the_end) 
                    T[places[o].first + player_grid_size.first - 3][places[o].second + 3 + j * 5] = 
                        (gracze[o].karty[j].kolor == "pik")   * 1 +  
                        (gracze[o].karty[j].kolor == "trefl") * 2 +
                        (gracze[o].karty[j].kolor == "karo")  * 3 +
                        (gracze[o].karty[j].kolor == "kier")  * 4 ;   
            }
                
            
        
        vector <pair <pair <int, int>, string> > slowa;
        slowa.push_back({{board_size.first / 2 - 4, board_size.second / 2 - 2}, "TOTAL"});
	    slowa.push_back({{board_size.first / 2 - 3, board_size.second / 2 - 2}, str(pula())});
	    
        for (int i = 0; i < gracze.size(); i ++){
            slowa.push_back({{places[i].first + 0, places[i].second + 5}, C(gracze[i].nazwa.length() + 2, '-')});
            slowa.push_back({{places[i].first + 1, places[i].second + 5}, "|" + gracze[i].nazwa + "|"});
            slowa.push_back({{places[i].first + 2, places[i].second + 5}, C(gracze[i].nazwa.length() + 2, '-')});
            slowa.push_back({{places[i].first + 1, places[i].second + 16}, str(stacki[i])});
            slowa.push_back({{places[i].first + 5, places[i].second + 16}, str(gracze[i].wartosc_stacka)});
        }
        
        for (auto e : slowa)
            for (int i = 0; i < e.second.length(); i ++)
                T[e.first.first][e.first.second + i - e.second.length() / 2] = e.second[i];
        
        
        const char *s1 = u8"\u2660";
        const char *s2 = u8"\u2663";
        const char *s3 = u8"\u2665";
        const char *s4 = u8"\u2666";
        
        for (auto e : T){
            for (auto f : e){
                if (f == 1) cout << s1;
                else if (f == 2) cout << s2;
                else if (f == 3) cout << s3;
                else if (f == 4) cout << s4;
                else cout << f;
            }
            cout << "\n";
        }
            
        cout << endl;                        
    }
    
    pair <string, vector <int> > kto_wygrywa(){ 
        vector <ulozenie> X;
        vector <karta> Y;
        for (auto e : karty_na_stole)
            Y.push_back(e);
            
        for (auto e : gracze)
            if (e.aktualnie_w_grze){
                for (auto f : e.karty)
                    Y.push_back(f);
                    
                X.push_back(ulozenie(Y, e.id_gracza));
                for (int i = 0; i < 2; i ++)
                    Y.pop_back();
            }
       sort(X.begin(), X.end());
       vector <int> res;
       for (auto e : X)     
            res.push_back(e.id_gracza);
            
       return {X[0].nazwa, res};
    }
    
    karta losuj_karte(){
        if (karty_wolne.empty()){
            cout << "\nBLAD PROGRAMU, BRAK WOLNYCH KART DO ROZDANIA!\n";
            exit(0);
        }
        
        swap(karty_wolne[rnd(0, karty_wolne.size() - 1)], karty_wolne[karty_wolne.size() - 1]);
        karta cand = karty_wolne.back();
        karty_wolne.pop_back();
        
        return cand;
    }
    
    void rozdaj_karty(){
        for(int i = 0; i < gracze.size(); ++i){
        	vector <karta> temp;
			temp.push_back(losuj_karte());
			temp.push_back(losuj_karte());
        	gracze[i].karty = temp;
        }
    }
    
    void rozdaj_kase(){
        vector <int> wygrani = kto_wygrywa().second;
        
        for (auto e : wygrani){
            int zabiera = stacki[e];
            for (int i = 0; i < stacki.size(); i ++){
                int ile = min(stacki[i], zabiera);
                gracze[e].wartosc_stacka += ile;
                stacki[i] -= ile;
            }
        }
    }
    
    void odswiez(){
        ktora_tura ++;
        if (ktora_tura % 5 == 0)
            big_blind *= 2;
        
        karty_wolne.clear();
        for(int i = 0; i < 52; ++ i)
            karty_wolne.push_back(karta(i));
        karty_na_stole.clear();
        stacki.resize(gracze.size());
        kto_rozdaje = (kto_rozdaje + 1) % gracze.size();
        for (int i = 0; i < gracze.size(); i ++)
            gracze[i].aktualnie_w_grze = true;
    }
  
    void pas(int id_gracza){
        gracze[id_gracza].aktualnie_w_grze = false;
        for (auto e : gracze)
            e.spasowali[id_gracza] = true;
    }
    
    void sprawdz(int id_gracza){
        int maks = 0;
        for (auto e : stacki)
            maks = max(maks, e);
        
        int ile_trzeba_podbic = min(maks - stacki[id_gracza], 
                                    gracze[id_gracza].wartosc_stacka);
        
        gracze[id_gracza].wartosc_stacka -= ile_trzeba_podbic;
        stacki[id_gracza] += ile_trzeba_podbic;
    }
    
    bool podbij(int id_gracza, int x, bool we_must = false){
        if (!we_must and x < big_blind / 2 + max_stack() - stacki[id_gracza])
            return false;
        
        
        x = min(x, gracze[id_gracza].wartosc_stacka);
        gracze[id_gracza].wartosc_stacka -= x;
        stacki[id_gracza] += x;
        return true;
    }
    
    int wykonaj_ruch_bota(int id_gracza, int ktora_runda){
        cout << "Bot " << gracze[id_gracza].nazwa << " wykonuje swoj ruch, prosze czekac." << endl;
      //  usleep(B);
        if (gracze[id_gracza].nazwa_bota == "staszek") return bot_michala(id_gracza, ktora_runda, *this);
        if (gracze[id_gracza].nazwa_bota == "blachu")   return bot_artura(id_gracza, ktora_runda, *this);
        if (gracze[id_gracza].nazwa_bota == "Franek")   return bot_artura(id_gracza, ktora_runda, *this);
        if (gracze[id_gracza].nazwa_bota == "Klementy")   return bot_artura(id_gracza, ktora_runda, *this);
        cout << "Nie ma takiego bota!\n"; exit(0);
    }
    
    int wykonaj_ruch_gracza(int id_gracza){
        legenda();
     
        string x;
        cin >> x;
        if (x == "podbij"){
            int y; 
            cin >> y;
         
            if (!podbij(id_gracza, y)){
                cout << "\nMinimalne podbicie to " << big_blind / 2 + max_stack() - stacki[id_gracza] << "!" << endl;
                usleep(B);
                return 0;
            }
            return 2;
        }
        else if (x == "sprawdz")
            sprawdz(id_gracza);
        else if (x == "pas")
            pas(id_gracza);
        else{
            cout << "Podano niepoprawny ruch, sprobuj jeszcze raz" << endl;
            
            return 0;
        }
        return 1;
    }
   
    void powiedz_kto_wygral(){
        pair <string, vector <int> > W = kto_wygrywa();
        cout << "Najlepsze ulozenie: " << W.first << "\n"; 
        cout << "Kolejnosc wygranych: ";
        for (auto e : W.second)
            cout << gracze[e].nazwa << ", "; 
        cout << "\n\nNapisz cos aby kontynuowac" << endl;   
        string x; cin >> x;
        
    }
    
    bool rozgrywaj(){
        rozdaj_karty();
        itt++;
        for (int i = 0; i <  5; i += (i == 0) + 1){
            int kon = gracze.size() - 1;
            for (int j = itt; j <= kon + itt and ile_gra() > 1; j ++){
                czyja_kolej = j % gracze.size();
                
                if (i == 0 and j == itt)
                    podbij(czyja_kolej, big_blind / 2, true);
                else if (i == 0 and j == itt + 1){
                	kon += 2;
                    podbij(czyja_kolej, big_blind, true);
                }
                else if (gracze[czyja_kolej].aktualnie_w_grze and !gracze[czyja_kolej].all_in()){
                    
                    
                    int W;
                    if (gracze[j % gracze.size()].bot){
                        W = wykonaj_ruch_bota(j % gracze.size(), i);
                    }
                    else {
                         interakcja(czyja_kolej);
                        W = wykonaj_ruch_gracza(j % gracze.size());
                    }
                    if (W == 0) j--;
                    if (W == 2) kon = j + gracze.size() - 2;
                }
                else interakcja(czyja_kolej, false), usleep(B);
            }
            
            for (int j = 0; j < (i < 4) + (i == 0) * 2; j ++)
                karty_na_stole.push_back(losuj_karte());           
        }
        usleep(B);
        system("clear");
        pokaz_stol(true);
        powiedz_kto_wygral();
        rozdaj_kase();
        odswiez();
        int wgrze = 0;
        for (auto e : gracze)
            if (e.wartosc_stacka > 0) ++wgrze;
        if (wgrze > 1) return 1;
        return 0;
    }
};

void solve(){
    vector <pair <bool, pair <string, int> > > X = {
    	{0, {"Michal",  -1}},
    	{0, {"John",    -1}},
    	{1, {"blachu",   -1}},
    	{1, {"Franek",   -1}},
    	
    };
    Stol Board(X, 10000, 25);

    
    while (Board.rozgrywaj());
    
    for (auto e : Board.gracze){
        if (e.wartosc_stacka > 0){
            cout << "Wygral " << e.nazwa << "!\n";
        }
    }
}   

int main(){
    srand(time(NULL));
    ios_base::sync_with_stdio(0);
    //cout.tie(0);
    //cin.tie(0);
    
    solve();
    
    return 0;
}

struct Akumulator_Michala{

} D;

int bot_michala(int id_gracza, int ktora_runda, Stol &S){
    return 100;
}

struct Akumulator_Artura{
    vector<vector<double> > szanse;
} C;

pair<karta, vector<bool> > losuj_karte(vector <double> prefsum, vector<bool> uzyte_karty){
    while (true){
        double X = (double) rnd(0, 1 << 15) / (double) (1 << 15);
        int a = 0, b = 52;//cerr<<X;system("pause");

        while(a + 1 < b){
            int s = (a + b) / 2;
            if (prefsum[s] > X) b = s;
            else a = s;
        }//cerr<<a<<endl;
        if (!uzyte_karty[a]){
            uzyte_karty[a] = true;
            return make_pair( karta(a), uzyte_karty );
        }
    }
}

double licz_szanse(vector<karta> karty_na_stole, vector< vector<double> > szanse, int liczba_graczy, vector<bool> uzyte_karty, vector<karta> karty, int id_gracza, vector<bool> spasowali){

    vector<double> prefsum_zb(52);
    double ile_wygralem = 0;
    prefsum_zb[0] = 0;

    for(int j = 1; j < 52; ++j)
        prefsum_zb[j] = prefsum_zb[j-1] + 1.0 / 52;

    vector <vector <double> > prefsum;
    prefsum.resize(szanse.size());
    for (int i = 0; i < szanse.size(); i ++)
        prefsum[i].resize(52);

    for (int i = 0; i < liczba_graczy; ++i){
        prefsum[i][0] = 0;
        for(int j = 1; j < szanse[i].size(); ++j)
            prefsum[i][j] = prefsum[i][j - 1] + szanse[i][j];
    }

    for (int i = 0; i < stala_symulacji; ++i){
        for(int j = 0; j < 52; ++j)
            uzyte_karty[j] = 0;
        uzyte_karty[karty[0].id_karty()] = uzyte_karty[karty[1].id_karty()] = 1;

        for(int j = 0; j < karty_na_stole.size(); ++j)
            uzyte_karty[karty_na_stole[j].id_karty()] = 1;

        vector <vector <karta> > karty_przeciwnikow(liczba_graczy);

        for (int j = 0; j < liczba_graczy; ++ j){
            pair<karta, vector<bool> > xxxx = losuj_karte(prefsum[j], uzyte_karty);
            karty_przeciwnikow[j].push_back(xxxx.first);
            xxxx = losuj_karte(prefsum[j], xxxx.second);
            karty_przeciwnikow[j].push_back(xxxx.first);
            uzyte_karty = xxxx.second;
            //cerr<<j<<" kart_przec_j"<<endl;
            //cerr<<id_gracza<<" id_gracza"<<endl;
            //cerr<<liczba_graczy<<" liczba_graczy"<<endl;
        }

        int dodatkowe_karty_na_stole = 0;
        while (karty_na_stole.size() < 5){
            ++ dodatkowe_karty_na_stole;
            pair<karta, vector<bool> > xxxx = losuj_karte(prefsum_zb, uzyte_karty);
            karty_na_stole.push_back(xxxx.first);
            uzyte_karty = xxxx.second;
        }

        vector <ulozenie> X;

        karty_przeciwnikow[id_gracza][0]=karty[0];
        karty_przeciwnikow[id_gracza][1]=karty[1];

        int cnt = 0;//cerr<<"!";
        for (auto e : karty_przeciwnikow){
            for (auto f : e)
                karty_na_stole.push_back(f);

            if(spasowali[cnt] == 0)
            X.push_back(ulozenie(karty_na_stole, cnt));

            for (auto f : e)
                karty_na_stole.pop_back();
            ++cnt;
        }//cerr<<"?";

        karty_przeciwnikow.pop_back();
        sort(X.begin(), X.end());

        int ile_gosci;
        bool odw = false;
        for (int j = 0; j < X.size() and X[j].moc == X[0].moc; j ++){
            ile_gosci = j + 1;
            if (X[j].id_gracza == id_gracza)
                odw = true;
        }

        if(odw == true) ile_wygralem += 1.0/ile_gosci;

        while(dodatkowe_karty_na_stole > 0){
            karty_na_stole.pop_back();
            -- dodatkowe_karty_na_stole;
        }
    }

    return ile_wygralem / (double) stala_symulacji;
}


int bot_artura(int id_gracza, int ktora_runda, Stol &S){
    double blef = 0.2;
    double odwaga = -0.3;
    double sila = 2.0;
    double atak = 0.1;

    int liczba_graczy = S.ile_gra();
    vector<vector<double> > szanse;
    vector<double> szansa_zb;
    for(int i=0;i<52;++i)szansa_zb.push_back(1.0/52);
    for(int i=0;i<liczba_graczy;++i)szanse.push_back(szansa_zb);

    vector<bool> spasowali;
    for(int i=0;i<liczba_graczy;++i){
        spasowali.push_back(!S.gracze[id_gracza].aktualnie_w_grze);
    }

    vector<bool> temp; temp.resize(52);
    double mojaszansa = licz_szanse(S.karty_na_stole, szanse, liczba_graczy, temp, S.gracze[id_gracza].karty, id_gracza, spasowali);
    if(debug)cerr<<"!!!"<<mojaszansa<<endl;

    struct X {
        static double los(){
            double x = 1.0*rnd(0, 1<<12) / (1<<12);
            return x;
        }
    };

    int corobic = -1;
    int maks = 0;
    int sum = 0;
    for (auto e : S.stacki){
        maks = max(maks, e);
        sum += e;
    }
    int ile_trzeba_podbic = min(maks - S.stacki[id_gracza],
                                S.gracze[id_gracza].wartosc_stacka);

    double woczekiwana = mojaszansa * sum  -  (1.0 - mojaszansa) * ile_trzeba_podbic;
    double woczproc = woczekiwana / ile_trzeba_podbic;

    if(woczproc + odwaga > 0)corobic = 0; // + (blef > X::los()) * 2 * X::los()
    if(ile_trzeba_podbic == 0)corobic = 0;


    if(corobic == 0 && ile_trzeba_podbic == 0){
        double oczatak = mojaszansa - 1.0 / liczba_graczy;
        if(oczatak + atak > 0)corobic = (blef < X::los()); // + (blef > X::los())
    }
    else if (corobic == 0){
        corobic = (blef > X::los());
    }
    else if( mojaszansa > 0.77 )corobic = 1;

    if(corobic == -1)S.pas(id_gracza);
    else if(corobic == 0)S.sprawdz(id_gracza);
    else if(corobic == 1)S.podbij(id_gracza, ile_trzeba_podbic + max((int)sila * maks,
                                                                     S.big_blind / 2 + S.max_stack() - S.stacki[id_gracza]));
    if(corobic == 1)return 2;
    return 1;
}



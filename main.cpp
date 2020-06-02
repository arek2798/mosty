#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

struct command { //struktura operacji
    int x, y;
    char line, way;
};

int charToNum(char m) { // Funkcja zamienia znak na liczbe
    if(m>48 && m<58) m-=49;
    else if(m>96 && m<123) m-=88;
    else if(m>64 && m<91) m-=56;
    return m;
}

void showHorizontalBorder(int width, bool isTop, char mark[35]) { //wyswietlanie poziomych lini obramowania
    if(isTop){
        cout << "   ";
        for(int i=0; i<width; i++) {
            cout << mark[i];
        }
        cout << endl << "  " << char(218);
    } else cout << "  " << char(192);
    for(int i=0; i<width; i++) {
        cout << char(196);
    }
    if(isTop) cout << char(191) << endl;
    else cout << char(217) << endl;
}

void showBoard(char gameBoard[20][20], int width, int height, char mark[35]) {  //wyswietlanie planszy
    showHorizontalBorder(width, true, mark);
    for(int i=0; i<height; i++) {
        cout << setw(2) << mark[i] << char(179);
        for(int j=0; j<width; j++) {
            cout << gameBoard[i][j];
        }
        cout << char(179);
        cout << endl;
    }
    showHorizontalBorder(width, false, mark);
}

bool isNumber(char gB[20][20], int x, int y) { //sprawdzanie czy znak na planszy jest liczba
    bool number = !(gB[y][x] == char(205) || gB[y][x] == char(186) || gB[y][x] == char(196) || gB[y][x] == char(179) || gB[y][x] == ' ');
    return number;
}

void commandExe(char gB[20][20], command cmd, int height, int width, char mark[35]) { //wykonywanie podanej operacji
    if(isNumber(gB, cmd.x, cmd.y)) {
        if(cmd.way == 's') { //dodawanie mostow w dol od wybranej literki
            bool goodWay = false;
            int n = 0;
            for(int i=cmd.y+1; i<height; i++) { //sprawdzanie czy w danym kierunku mozna poprowadzic most
                if(!isNumber(gB, cmd.x, i)) n++;
                else {
                    goodWay = true;
                    break;
                }
            }
            if(goodWay) { //tworzenie mostow w podanym kierunku
                for(int i=cmd.y+1; i<=cmd.y+n; i++) {
                    if(cmd.line == '-') gB[i][cmd.x] = char(179);
                    else if(cmd.line == '=') gB[i][cmd.x] = char(186);
                    else if(cmd.line == '!') gB[i][cmd.x] = ' ';
                }
            }
        } else if(cmd.way == 'w') { //dodawanie most�w w g�re od wybranej literki
            bool goodWay = false;
            int n = 0;
            for(int i=cmd.y-1; i>=0; i--) {
                if(!isNumber(gB, cmd.x, i)) n++;
                else {
                    goodWay = true;
                    break;
                }
            }
            if(goodWay) {
                for(int i=cmd.y-1; i>=cmd.y-n; i--) {
                    if(cmd.line == '-') gB[i][cmd.x] = char(179);
                    else if(cmd.line == '=') gB[i][cmd.x] = char(186);
                    else if(cmd.line == '!') gB[i][cmd.x] = ' ';
                }
            }
        } else if(cmd.way == 'd') { //dodawanie most�w w prawo od wybranej literki
            bool goodWay = false;
            int n = 0;
            for(int i=cmd.x+1; i<width; i++) {
                if(!isNumber(gB, i, cmd.y)) n++;
                else {
                    goodWay = true;
                    break;
                }
            }
            if(goodWay) {
                for(int i=cmd.x+1; i<=cmd.x+n; i++) {
                    if(cmd.line == '-') gB[cmd.y][i] = char(196);
                    else if(cmd.line == '=') gB[cmd.y][i] = char(205);
                    else if(cmd.line == '!') gB[cmd.y][i] = ' ';
                }
            }
        } else if(cmd.way == 'a') { //dodawanie most�w w lewo od wybranej literki
            bool goodWay = false;
            int n = 0;
            for(int i=cmd.x-1; i>=0; i--) {
                if(!isNumber(gB, i, cmd.y)) n++;
                else {
                    goodWay = true;
                    break;
                }
            }
            if(goodWay) {
                for(int i=cmd.x-1; i>=cmd.x-n; i--) {
                    if(cmd.line == '-') gB[cmd.y][i] = char(196);
                    else if(cmd.line == '=') gB[cmd.y][i] = char(205);
                    else if(cmd.line == '!') gB[cmd.y][i] = ' ';
                }
            }
        }
        system("cls");
        showBoard(gB, width, height, mark);
    } else {
        cout << "Wybrane pole nie jest liczba" << endl;
    }
}

bool checkLink(char gB[20][20], int width, int height) { // Funkcja sprawdza czy wszsytkie wyspy sa ze soba polaczone
    bool islandState[20][20]; //tablica odwzorowuje ulozenie wysp na planszy
    int numberOfLoop[20][20]; //tablica zawiera liczby okreslajace ilosc przebiegow petli na danej wyspie
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            numberOfLoop[i][j] = 0;
        }
    }
    bool lose = false;
    bool flag = false;
    int numberOfIsland = 0;
    int direction = 3; // zmienna okresla kierunki sprawdzania kolejnej wyspy 1-lewo 2-dol 3-prawo 4-gora
    int islandX = 0, islandY = 0;
    //przypisywanie wartosci false okreslajacej ze dana wyspa nie zostala jeszcze odwiedzona
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            if(isNumber(gB, j, i)) {
                islandState[i][j] = false;
                numberOfIsland++;
                if(!flag) {
                    islandState[i][j] = true;
                    numberOfIsland--;
                    flag = true;
                    islandX = j;
                    islandY = i;
                }
            }
        }
    }

    while(numberOfIsland>0 && !lose) {
        if(direction == 4) { //w gore
            if(gB[islandY-1][islandX] == char(179) || gB[islandY-1][islandX] == char(186)) {
                for(int i=islandY-1; i>=0; i--) {
                    if(isNumber(gB, islandX, i)) {
                        if(!islandState[i][islandX]) {
                            islandState[i][islandX] = true; //zapisanie ze wyspa jest juz odwiedzona
                            numberOfIsland--;               // i zmniejszenie liczby wysp do odwiedzenia o jeden
                        }
                        if((++numberOfLoop[i][islandX])>4) lose = true; //jesli na danej wyspie jestesmy wiecej niz cztery rzy oznacza to
                        islandY = i;                                    //ze wszystkie wyspy nie sa ze soba poloczone
                        direction = 1;
                        break;
                    }
                }
            } else direction = 3; //jesli w danym kierunku nie ma mostu kierunek zostaje zmieniony na nastepny w prawo

        } else if(direction == 3) { // w prawo
            if(gB[islandY][islandX+1] == char(196) || gB[islandY][islandX+1] == char(205)) {
                for(int i=islandX+1; i<width; i++) {
                    if(isNumber(gB, i, islandY)) {
                        if(!islandState[islandY][i]) {
                            islandState[islandY][i] = true;
                            numberOfIsland--;
                        }
                        if((++numberOfLoop[islandY][i])>4) lose = true;
                        islandX = i;
                        direction = 4;
                        break;
                    }
                }
            } else direction = 2;

        } else if(direction == 2) { //w dol
            if(gB[islandY+1][islandX] == char(179) || gB[islandY+1][islandX] == char(186)) {
                for(int i=islandY+1; i<height; i++) {
                    if(isNumber(gB, islandX, i)) {
                        if(!islandState[i][islandX]) {
                            islandState[i][islandX] = true;
                            numberOfIsland--;
                        }
                        if((++numberOfLoop[i][islandX])>4) lose = true;
                        islandY = i;
                        direction = 3;
                        break;
                    }
                }
            } else direction = 1;

        } else if(direction == 1) { //w lewo
            if(gB[islandY][islandX-1] == char(196) || gB[islandY][islandX-1] == char(205)) {
                for(int i=islandX-1; i>=0; i--) {
                    if(isNumber(gB, i, islandY)) {
                        if(!islandState[islandY][i]) {
                            islandState[islandY][i] = true;
                            numberOfIsland--;
                        }
                        if((++numberOfLoop[islandY][i])>4) lose = true;
                        islandX = i;
                        direction = 2;
                        break;
                    }
                }
            } else direction = 4;
        }
    }
    if(numberOfIsland==0 && !lose) return true; //wszystkie wyspy sa ze soba polaczone
    else if(numberOfIsland>0 && lose) return false;

}

bool checkWin(char gB[20][20], int width, int height) {
    bool win = true;
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            if(isNumber(gB, j, i)) {
                int n = gB[i][j] - 48; //zmienna n okresla liczbe mostow na wyspie, nastepnie od tej liczby odejmowane sa mosty w kazdym kierunku
                if(gB[i][j+1] == char(196)) n--; //most pojedynczy
                else if(gB[i][j+1] == char(205)) n-=2; //most podwojny
                if(gB[i][j-1] == char(196)) n--; //most pojedynczy
                else if(gB[i][j-1] == char(205)) n-=2; //most podwojny
                if(gB[i+1][j] == char(179)) n--; //most pojedynczy
                else if(gB[i+1][j] == char(186)) n-=2; //most podwojny
                if(gB[i-1][j] == char(179)) n--; //most pojedynczy
                else if(gB[i-1][j] == char(186)) n-=2; //most podwojny
                if (n!=0) { //jesli liczba mostow nie jest rowna z mozliwosciami wyspy to gramy dalej
                    win=false;
                    break;
                }
            }
        }
        if(!win) break;
    }
    if(win) {
        if(checkLink(gB, width, height)) win = true;
        else {
            win = false;
            cout << "Wszystkie wyspy musza byc polaczone" << endl;
        }
    }
    return win;
}

void help() {
    system("cls");
    cout << "POMOC" << endl << endl;
    cout << "1. Zasady gry" << endl << endl;
    cout << "W diagramie przedstawione sa wyspy z liczbami od 1 do 8, ktore okreslaja, ile dokladnie mostow ma byc przylaczonych do danej wyspy. Trzeba polaczyc je tak, aby z dowolnej mozna bylo przedostac sie przez te mosty na kazda inna. Obowiazuja przy nim nastepujace zasady: " << endl;
    cout << "- Mosty mozna przeprowadzic tylko w kierunkach poziomym i pionowym" << endl;
    cout << "- Kazdy most musi laczyc dwie wyspy" << endl;
    cout << "- Mosty nie moga sie przecinac, ani nie moga przechodzic przez wyspy" << endl;
    cout << "- Dwie wyspy moga byc polaczone miedzy soba najwyzej dwoma mostami" << endl << endl;
    cout << "2. Sterowanie" << endl << endl;
    cout << "Sterowanie w grze polega na wpisywaniu komend skladajacych sie ze czterech znakow." << endl;
    cout << "Pierwsze dwa znaki to wspolrzedne wyspy (1-Z)." << endl;
    cout << "Trzeci znak okresla rodzaj mostu jaki chcemy zbudowac, myslnik(-) oznacza most pojedynczy a znak rownosci(=) oznacza most podwojny. Gdy na trzeciej pozycji podamy wykrzyknik(!) zostanie usuniety dany most." << endl;
    cout << "Ostatnim znakiem jakim podajemy jest kierunek dzialania, okreslamy go nastepujaco: a -> lewo, d -> prawo, w -> gora, s -> dol." << endl << endl;
    cout << "Przyklady komend: 4C=d lub a3!w" << endl << endl;
    cout << "Gdy jako komende wpiszemy znak zapytania(?) wyswietlona zostanie pomoc do gry, natomiast litera q pozwala wrocic do menu gry." << endl << endl;
    system("pause");
    system("cls");
}

void game(char gameBoard[20][20], int gameBoardWidth, int gameBoardHeight, char mark[35], bool win) {
    showBoard(gameBoard, gameBoardWidth, gameBoardHeight, mark);
    while(!win) {
        cout << "Podaj operacje: ";
        char movement[7];
        cin >> movement;
        command cmd;
        cmd.x=charToNum(movement[1]);   //przypisywanie wprowadzonej operacji do struktury komendy
        cmd.y=charToNum(movement[0]);
        cmd.line = movement[2];
        cmd.way = movement[3];
        if(cmd.y == 25) break; // wpisano q
        else if(cmd.y == 63) { // wpisano ?
            help();
            showBoard(gameBoard, gameBoardWidth, gameBoardHeight, mark);
        }
        else if(cmd.x<0 || cmd.x>gameBoardWidth || cmd.y<0 || cmd.y>gameBoardHeight) cout << "Podany punkt nie znajduje sie na planszy (? - pomoc)" << endl;
        else if(cmd.line != '=' && cmd.line != '-' && cmd.line != '!') cout << "Podany parametr jest nieprawidlowy (? - pomoc)" << endl;
        else if(cmd.way != 'w' && cmd.way != 's' && cmd.way != 'a' && cmd.way != 'd') cout << "Podaj inny kierunek ruchu (w->gora, s->dol, a->lewo, d->prawo)" << endl;
        else {
            commandExe(gameBoard, cmd, gameBoardHeight, gameBoardWidth, mark);  //wykonywanie operacji
            win = checkWin(gameBoard, gameBoardWidth, gameBoardHeight); //sprawdzanie czy plansza jest rozwiazana poprawnie
        }
    }
    if(win) {
        cout << endl;
        cout << setw(10) << "WYGRALES!!!" << endl << endl;
        cout << "Sprawdz inne poziomy" << endl << endl;
        system("pause");
    }
    system("cls");
}

int main()
{
    char mark[35] = {'1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','R','S','T','U','W','X','Y','Z'};
    string errCont;
    bool err = false;

    while(1) {
        bool win = false;
        int gameBoardWidth;
        int gameBoardHeight;

        cout << setw(40) << " __  __  ___  ____ _______   __" << endl;
        cout << setw(40) << "|  \\/  |/ _ \\/ ___|_   _\\ \\ / /" << endl;
        cout << setw(40) << "| |\\/| | | | \\___ \\ | |  \\ V / " << endl;
        cout << setw(40) << "| |  | | |_| |___) || |   | |  " << endl;
        cout << setw(40) << "|_|  |_|\\___/|____/ |_|   |_|  " << endl << endl << endl;
        cout << "1 - Rozpocznij gre" << endl;
        cout << "2 - Pomoc" << endl;
        cout << "q - Wyjscie" << endl << endl;
        if(err) {
             cout << errCont << endl;
             err = false;
        }
        cout << "-> ";
        char option[1];
        cin >> option;
        cin.ignore(1);
        system("cls");
        switch(option[0]) {
        case '1':
            {
                cout << "Wybierz poziom trudnosci: " << endl << endl;
                cout << "1 - Poziom latwy (7x10)" << endl;
                cout << "2 - Poziom sredni (9x13)" << endl;
                cout << "3 - Poziom trudny (10x14)" << endl;
                cout << "->";
                char level[1];
                cin >> level;
                while(level[0] != '1' && level[0] != '2' && level[0] != '3') {
                    cout << "Podana opcja nie istnieje" << endl;
                    cout << "->";
                    cin >> level;
                }
                system("cls");
                if(level[0] == '1') {
                    gameBoardWidth = 10;
                    gameBoardHeight = 7;
                    char gameBoard[20][20] = {{' ','2',' ','4',' ','3',' ','6',' ','2'},
                                            {' ',' ','2',' ','3',' ','1',' ',' ',' '},
                                            {'2',' ',' ','2',' ','1',' ','5',' ','2'},
                                            {' ','1',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {'5',' ','5',' ','8',' ',' ','3',' ',' '},
                                            {' ',' ',' ',' ',' ','1',' ',' ',' ','2'},
                                            {'3',' ','2',' ','5',' ','3',' ','1',' '}};

                    game(gameBoard, gameBoardWidth, gameBoardHeight, mark, win);
                } else if(level[0] == '2') {
                    gameBoardWidth = 13;
                    gameBoardHeight = 9;
                    char gameBoard[20][20] = {{'3',' ','2',' ','3',' ','4',' ','3',' ','3',' ','2'},
                                            {' ',' ',' ',' ',' ',' ',' ','2',' ','5',' ','1',' '},
                                            {' ','1',' ','2',' ',' ','2',' ','3',' ',' ',' ',' '},
                                            {'3',' ',' ',' ','3',' ',' ','2',' ','5',' ',' ','3'},
                                            {' ','2',' ','4',' ',' ','5',' ','5',' ',' ','2',' '},
                                            {'3',' ',' ',' ',' ','1',' ','1',' ','3',' ',' ',' '},
                                            {' ',' ',' ','2',' ',' ','2',' ','1',' ',' ',' ','3'},
                                            {' ','1',' ',' ',' ','2',' ','3',' ','3',' ','3',' '},
                                            {'2',' ',' ','4',' ',' ','4',' ','3',' ','1',' ','2'}};

                    game(gameBoard, gameBoardWidth, gameBoardHeight, mark, win);
                } else if(level[0] == '3') {
                    gameBoardWidth = 14;
                    gameBoardHeight = 10;
                    char gameBoard[20][20] = {{'3',' ',' ','2',' ','3',' ',' ','4',' ',' ','3',' ','3'},
                                            {' ','3',' ',' ','3',' ','3',' ',' ','3',' ',' ','2',' '},
                                            {' ',' ','3',' ',' ','2',' ','2',' ',' ',' ',' ',' ','2'},
                                            {'3',' ',' ','2',' ',' ','3',' ',' ',' ','2',' ','3',' '},
                                            {' ',' ','2',' ','3',' ',' ','5',' ','5',' ','3',' ','3'},
                                            {' ','3',' ','2',' ',' ','2',' ','2',' ','1',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ','2',' ','2',' ','2',' ',' '},
                                            {'3',' ','3',' ','4',' ','4',' ','4',' ','2',' ',' ','3'},
                                            {' ',' ',' ','2',' ','3',' ',' ',' ',' ',' ','2',' ',' '},
                                            {'1',' ','3',' ','3',' ',' ','3',' ','3',' ',' ',' ','2'}};

                    game(gameBoard, gameBoardWidth, gameBoardHeight, mark, win);
                }
                break;
            }

        case '2':
            help();
            break;

        case 'q':
            return 0;
            break;

        case 'Q':
            return 0;
            break;

        default :
            errCont = "Podana opcja nie istnieje";
            err = true;
            break;
        }
    }
    return 0;
}

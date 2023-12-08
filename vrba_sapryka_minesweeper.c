#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int riadky;
    int stlpce;
    int pocetBomb;
    int wincon;
} nastaveniehp;

typedef struct {
    int jeBomba;
    int susediSBombou;
    int jeZakryte;
} policko;

nastaveniehp nastavithp() {
    nastaveniehp nast;
    int inputCheck1;
    printf("Zadajte pocet riadkov herneho pola (min. 4): ");
    inputCheck1 = scanf("%d", &nast.riadky);
    while (inputCheck1 != 1 || nast.riadky < 4) {
        while (getchar() != '\n');
        printf("Neplatny vstup. Zadajte kladne cele cislo (min. 4) pre pocet riadkov herneho pola: ");
        inputCheck1 = scanf("%d", &nast.riadky);
    }
    printf("Zadajte pocet stlpcov herneho pola (min. 4): ");
    inputCheck1 = scanf("%d", &nast.stlpce);
    while (inputCheck1 != 1 || nast.stlpce < 4) {
        while (getchar() != '\n');
        printf("Neplatny vstup. Zadajte kladne cele cislo (min. 4) pre pocet stlpcov herneho pola: ");
        inputCheck1 = scanf("%d", &nast.stlpce);
    }
    printf("Zadajte pocet bomb v hernom poli (max. %d): ", (nast.riadky * nast.stlpce) - 9);
    inputCheck1 = scanf("%d", &nast.pocetBomb);
    while (inputCheck1 != 1 || nast.pocetBomb < 0 || nast.pocetBomb > (nast.riadky * nast.stlpce) - 9) {
        while (getchar() != '\n');
        printf("Neplatny vstup. Zadajte cele cislo (max. %d) pre pocet bomb v hernom poli: ", (nast.riadky * nast.stlpce) - 9);
        inputCheck1 = scanf("%d", &nast.pocetBomb);
    }
    return nast;
}

void generaciaSuradnicBomb(int riadky, int stlpce, int pocetBomb, policko **hernepole,int in_x,int in_y) {
    srand(time(NULL));
    int pocetUmiestnenychBomb = 0;
    while (pocetUmiestnenychBomb < pocetBomb) {
        int x = rand() % riadky;
        int y = rand() % stlpce;
        if (hernepole[x][y].jeBomba == 0) {
            if (!(x >= in_x - 1 && x <= in_x + 1 && y >= in_y - 1 && y <= in_y + 1)) {
                hernepole[x][y].jeBomba = 1;
                pocetUmiestnenychBomb++;
            }
        }
    }
}

//debug funkcia
void vypisanieSuradnicBomb(policko **hernepole, int riadky, int stlpce) {
    printf("Suradnice bomb:\n");
    for (int i = 0; i < riadky; i++) {
        for (int j = 0; j < stlpce; j++) {
            if (hernepole[i][j].jeBomba) {
                printf("[%d,%d],\n", i+1, j+1);
            }
        }
    }
}

void vypocetSusediSBombou(int riadky, int stlpce, policko **hernepole) {
    for (int x = 0; x < riadky; x++) {
        for (int y = 0; y < stlpce; y++) {
            if (hernepole[x][y].jeBomba) {
                continue;
            }
            int pocetSusediSBombou = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < riadky && ny >= 0 && ny < stlpce && hernepole[nx][ny].jeBomba) {
                        pocetSusediSBombou++;
                    }
                }
            }
            hernepole[x][y].susediSBombou = pocetSusediSBombou;
        }
    }
}

void vypisanieHernehoPola(int riadky, int stlpce, policko **hernepole) {
    for (int i = 0; i < riadky; i++) {
        for (int j = 0; j < stlpce; j++) {
            if (hernepole[i][j].jeZakryte) {
                printf("? ");
            } else if (hernepole[i][j].jeBomba) {
                printf("* ");
            } else {
                printf("%d ", hernepole[i][j].susediSBombou);
            }
        }
        printf("\n");
    }
}

void odhaleniePolicok(policko **hernepole, int x, int y, int riadky, int stlpce, int *wincon) {
    if (x < 0 || x >= riadky || y < 0 || y >= stlpce) {
        return;
    }
    else if (!hernepole[x][y].jeZakryte) {
        return;
    }
    else if (hernepole[x][y].jeBomba) {
        *wincon=-1;
        for (int i = 0; i < riadky; i++) {
            for (int j = 0; j < stlpce; j++) {
                if (hernepole[i][j].jeBomba) {
                    hernepole[i][j].jeZakryte = 0;
                }
            }
        }
        return;
    }
    else {
        hernepole[x][y].jeZakryte = 0;
        (*wincon)--;
        if (hernepole[x][y].susediSBombou == 0) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int newX = x + dx;
                    int newY = y + dy;
                    odhaleniePolicok(hernepole, newX, newY, riadky, stlpce, wincon);
                }
            }
        }
    }
}

int uzivatelskeMenu() {
    int vyber;
    int inputCheck2;
    do {
        printf("Zadajte vas vyber z nasledujucich moznosti:\n");
        printf("1. Zahajit novu hru\n");
        printf("2. Nastavit herne pole\n");
        printf("3. Zobrazit najlepsie casy pre nastavene hracie pole\n");
        printf("4. Ukoncit program\n");
        inputCheck2 = scanf("%d", &vyber);
        while (getchar() != '\n');
        if (inputCheck2 != 1 || vyber < 1 || vyber > 4) {
            printf("Neplatny vstup.\n");
            inputCheck2 = 0;
        }
    } while (inputCheck2 == 0);
    return vyber;
}

double vyhra(double t, int riadky, int stlpce, policko **hernepole) {
    double dosiahnutyCas = ((double)t)/CLOCKS_PER_SEC;
    vypisanieHernehoPola(riadky, stlpce, hernepole);
    printf("Vyhrali ste s casom %f s. Gratulujem!\n",dosiahnutyCas);
    return dosiahnutyCas;
}

double prehra(double t, int riadky, int stlpce, policko **hernepole) {
    vypisanieHernehoPola(riadky, stlpce, hernepole);
    printf("Prehrali ste. To sa stava aj najlepsim!\n");
}

void vypisanierebricku(int riadky, int stlpce, int pocetBomb) {
    char filename[10];
    sprintf(filename,"%dx%dx%d.txt",riadky,stlpce,pocetBomb);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Na nastavenom hracom poli doposial nebola dosiahnuta ziadna vyhra.\n", riadky, stlpce, pocetBomb);
        return;
    }
    float times[100];
    int count = 0;
    while (fscanf(file, "%f", &times[count]) != EOF) {
        count++;
        if (count >= 100) {
            break;
        }
    }
    fclose(file);
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (times[j] > times[j + 1]) {
                float temp = times[j];
                times[j] = times[j + 1];
                times[j + 1] = temp;
            }
        }
    }
    printf("Najlepsie casy pre nastavene hracie pole:\n");
    for (int i = 0; i < count; i++) {
        printf("%f\n", times[i]);
    }
    return;
}

int main() {
    nastaveniehp nast = nastavithp();
    int vyber;
    do {
        vyber = uzivatelskeMenu();
        if (vyber==1){
            nast.wincon = nast.riadky * nast.stlpce - nast.pocetBomb;
            policko **hernepole = (policko **)malloc(nast.riadky * sizeof(policko *));
            for (int i = 0; i < nast.riadky; i++) {
                hernepole[i] = (policko *)malloc(nast.stlpce * sizeof(policko));
                for (int j = 0; j < nast.stlpce; j++) {
                    hernepole[i][j].jeBomba = 0;
                    hernepole[i][j].susediSBombou = 0;
                    hernepole[i][j].jeZakryte = 1;
                }
            }
            clock_t t = clock();
            do {
                vypisanieHernehoPola(nast.riadky, nast.stlpce, hernepole);
                int x, y, inputCheck3;
                printf("Zadajte suradnice policka, ktore si zelate odhalit (riadok stlpec):\n");
                inputCheck3 = scanf("%d %d", &x, &y);
                if (inputCheck3 != 2) {
                    printf("Neplatny vstup.\n");                    
                    while (getchar() != '\n');
                    continue;
                }
                x--;
                y--;
                if (x < 0 || x >= nast.riadky || y < 0 || y >= nast.stlpce) {
                    printf("Neplatne suradnice. Zadane suradnice muasia byt v rozsahu nastaveneho herneho pola.\n");
                    continue;
                }

                if (nast.wincon == nast.riadky * nast.stlpce - nast.pocetBomb) {
                    generaciaSuradnicBomb(nast.riadky, nast.stlpce, nast.pocetBomb, hernepole, x, y);
                    vypocetSusediSBombou(nast.riadky, nast.stlpce, hernepole);
                }
                odhaleniePolicok(hernepole, x, y, nast.riadky, nast.stlpce, &nast.wincon);
                if (nast.wincon == 0) {
                    double dosiahnutyCas=vyhra((clock()-t), nast.riadky, nast.stlpce, hernepole);               
                    char filename[10];
                    sprintf(filename,"%dx%dx%d.txt",nast.riadky,nast.stlpce,nast.pocetBomb);
                    FILE *file;
                    file=fopen(filename,"a");
                    fprintf(file, "%f\n",dosiahnutyCas);
                    fclose(file);
                    break;
                } else if (nast.wincon == -1) {
                    prehra((clock() - t), nast.riadky, nast.stlpce, hernepole);
                    break;
                }
            } while (1);
            for (int i = 0; i < nast.riadky; i++) {
                free(hernepole[i]);
            }
            free(hernepole);
        }
        else if (vyber == 2) {
            nast = nastavithp();
        }
        else if (vyber == 3) {
            vypisanierebricku(nast.riadky, nast.stlpce, nast.pocetBomb);
        }       
    } while (vyber != 4);
    printf("Ukoncovanie programu.\n");
    return 0;
}
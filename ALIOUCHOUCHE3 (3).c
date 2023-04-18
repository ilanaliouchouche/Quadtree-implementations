#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define formatBool(b) ((b) ? "oui" : "non")
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct bloc_image
{ bool toutnoir ;
    struct bloc_image * fils[4] ;
} bloc_image ;
typedef bloc_image *image ;


/* Cree une image blanche.
 */
image Construit_Blanc(){
    return NULL;
}

/* Cree une image noire.
 */
image Construit_Noir(){
    image img = malloc(sizeof(bloc_image));
    img->toutnoir = true;
    for (int i = 0; i < 4; i++)
        img->fils[i] = NULL;
    return img;
}

/* Cree une image dont les fils sont les images passees en parametre.
 */
image Construit_Composee(image i0, image i1, image i2, image i3)
{
    image img = malloc(sizeof(bloc_image));
    img->toutnoir = false;
    img->fils[0] = i0;
    img->fils[1] = i1;
    img->fils[2] = i2;
    img->fils[3] = i3;
    return img;
}

/* Affiche l'image passee en parametre.
 */
void Affiche_Normal(image arg)
{
    if (arg == NULL)
    {
        printf("B");
    }
    else if (arg->toutnoir)
    {
        printf("N");
    }
    else
    {
        printf("(");
        for (int i = 0; i < 4; ++i) {
            Affiche_Normal(arg -> fils[i]);
        }
        printf(")");
    }
}

/* Sous procedure d'Affiche_prof.
 */
void Affiche_Prof_Bis (image arg, int prof)
{
    if (arg == NULL)
        printf("B%d", prof);
    else if (arg->toutnoir)
        printf("N%d", prof);
    else
    {
        printf("(");
        for (int i = 0; i < 4; i++)
            Affiche_Prof_Bis(arg->fils[i], prof+1);
        printf(")");
    }
}

/* Affiche l'image passee en argument avec les profondeurs associees.
 */
void Affiche_Prof(image arg) {
    Affiche_Prof_Bis(arg, 0);
}


/* Retourne une sous-chaine de la chaine de caracteres passee en argument
 *allant de l'index m à n.
 */
char* substr(char *src, int m, int n) {
    int len = n - m;
    char *dest = malloc(len + 1);
    int i;
    for (i = m; i < n && (*(src + i) != '\0'); i++) {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

/* Lit l'image en argument sous forme de chaine de caracteres et la convertit
 * en une image. (La complexite est de O(n^2) on a tenté de faire mieux, en vain).
 */
image Lecture(char* arg) {
    int len = strlen(arg);
    char* tgt = malloc(len + 1);
    int tgtlen = 0;
    int i;
    for (i = 0; i < len; i ++) {
        if (arg[i] != '(' && arg[i] != ')' && arg[i] != 'N' && arg[i] != 'B' ) continue;
        tgt[tgtlen++] = arg[i];
    }
    tgt[tgtlen] = '\0';
    if (!strcmp(tgt, (char*)"N")) return Construit_Noir();
    if (!strcmp(tgt, (char*)"B")) return Construit_Blanc();
    image img = (image) malloc (sizeof(bloc_image));
    img -> toutnoir = 0;
    int l = 1, r = 0, cpt = 0;
    for (i = 0; i < 4; i ++) {
        while (1) {
            r ++;
            if (tgt[r] == '(') cpt ++;
            else if (tgt[r] == ')') cpt --;
            if (!cpt) {
                img -> fils[i] = Lecture(substr(tgt, l, r+1));
                l = r + 1;
                r = l - 1;
                break;
            }
        }
    }
    return img;
}

/* Rend vrai si l'image en parametre est noire.
 */
bool EstNoire(image arg) {
    if (arg == NULL) {
        return false;
    } else if (arg->toutnoir) {
        return true;
    } else {
        return EstNoire(arg->fils[0]) && EstNoire(arg->fils[1])
               && EstNoire(arg->fils[2]) && EstNoire(arg->fils[3]);
    }
}

/* Rend vrai si l'image en parametre est blanche.
 */
bool EstBlanche(image arg)
{
    if (arg == NULL)
        return true;
    else if (arg->toutnoir)
        return false;
    else
        return EstBlanche(arg->fils[0]) && EstBlanche(arg->fils[1]) &&
               EstBlanche(arg->fils[2]) && EstBlanche(arg->fils[3]);
}

/* Cree une image sous forme de diagonale de profondeur p.
 */
image Diagonale(int p)
{
    if (p == 0)
        return Construit_Noir();
    else
        return Construit_Composee(Diagonale(p - 1), Construit_Blanc(), Construit_Blanc(), Diagonale(p - 1));
}

/* Rend l'image passe en parametre avec un quart de tour dans le sens
 * des aiguilles d'une montre.
 */
image QuartDeTour(image arg){
    if (arg == NULL || arg->toutnoir)
        return arg;
    else
        return Construit_Composee(QuartDeTour(arg->fils[2]), QuartDeTour(arg->fils[0]), QuartDeTour(arg->fils[3]), QuartDeTour(arg->fils[1]));
}

/* En procedure.
 */
void QuartDeTour_Proc(image* arg)
{
    if (*arg == NULL || (*arg)->toutnoir)
        return;

    image i0 = (*arg)->fils[0];
    image i1 = (*arg)->fils[1];
    image i2 = (*arg)->fils[2];
    image i3 = (*arg)->fils[3];

    *arg = Construit_Composee(QuartDeTour(i2), QuartDeTour(i0), QuartDeTour(i3), QuartDeTour(i1));
}


/* Transforme l'image en parametre en son negatif.
 */
void Negatif(image* arg)
{
    if (*arg == NULL) {
        *arg = Construit_Noir();
    }
    else if ((*arg)->toutnoir) {
        *arg = Construit_Blanc();
    } else {
        Negatif(&(*arg)->fils[0]);
        Negatif(&(*arg)->fils[1]);
        Negatif(&(*arg)->fils[2]);
        Negatif(&(*arg)->fils[3]);
    }
}
/* Simplifie l'image passee en parametre en eliminant les arbres
 * monochrones par un pixel a la profondeur inidquee.
 */
void SimplifieProfP(image* arg, int prof) {
    if (*arg == NULL ||
        prof < 0 || (*arg) -> toutnoir ) {
        return;
    }
    bool n = true;
    bool b = true;
    for (int i = 0; i < 4;
         i++) { SimplifieProfP(&(*arg)->fils[i], prof - 1);
        n = n && EstNoire((*arg)->fils[i]);
        b = b && EstBlanche((*arg)->fils[i]);
    }
    if (prof == 0 &&
        n) {
        *arg = Construit_Noir();
    } else if (prof == 0 &&
               b) {
        *arg = Construit_Blanc();
    }
}

/* Verifie si la premiere image est incluse dans la deuxieme.
 */
bool Incluse(image arg1, image arg2){
    if (arg1 == NULL)
        return true;
    if (arg2 == NULL) //donc arg1 pas NULL mais peut etre blanche via ses fils.
        return EstBlanche(arg1); //On verifie que les fils sont blancs.
    if (arg2 -> toutnoir)
        return true;
    if(arg1 -> toutnoir)
        return EstNoire(arg2);   //la on verifie les fils sont noirs

    return Incluse(arg1->fils[0], arg2->fils[0]) &&
           Incluse(arg1->fils[1], arg2->fils[1]) &&
           Incluse(arg1->fils[2], arg2->fils[2]) &&
           Incluse(arg1->fils[3], arg2->fils[3]);
}




/*
int HautMaxBlanc(image arg) {
    if (EstNoire(arg)) return -1;
    if (arg == NULL) return 0;
    int cpt = -1;
    int i;
    for (i = 0; i < 4; i ++) cpt = max(cpt, HautMaxBlanc(arg->fils[i]));
    if (EstBlanche(arg)) cpt ++;
    return cpt;
}
 */

/* Retourne la hauteur de la plus haute image blanche.
 */
int HautMaxBlanc(image arg) {
    if (arg == NULL) return 0;
    if (EstNoire(arg)) return -1;

    int cpt = -1;
    int i = 0;
    while (i < 4) {
        int hauteur = HautMaxBlanc(arg->fils[i]);
        cpt = max(cpt, hauteur);
        i ++;
    }
    if (EstBlanche(arg)) cpt ++;
    return cpt;
}

/* Renvoie l'image passee en argument blanchit à profondeur p et
 * aux coordonnees (x,y).
 */
image BlanchitProfP(image arg, int p, int x, int y) {
    if (p==0) return Construit_Blanc();
    if (arg == NULL) return Construit_Blanc();
    if (arg -> toutnoir) arg = Construit_Composee(Construit_Noir(), Construit_Noir(), Construit_Noir(), Construit_Noir());
    image img = (image) malloc (sizeof(bloc_image));
    img -> toutnoir = arg -> toutnoir;
    int i;
    for (i = 0; i < 4; i ++) img -> fils[i] = arg -> fils[i];
    int d = 1 << p - 1;
    if (x < d && y >= d) img -> fils[0] = BlanchitProfP(arg->fils[0], p-1, x, y-d);
    else if (x >= d && y >= d) img -> fils[1] = BlanchitProfP(arg->fils[1], p-1, x-d, y-d);
    else if (x < d && y < d) img -> fils[2] = BlanchitProfP(arg->fils[2], p-1, x, y);
    else img -> fils[3] = BlanchitProfP(arg->fils[3], p-1, x-d, y);
    return img;
}

/* Modifie l'image en parametre en blanchissant à profondeur p et
 * aux coordonnes (x,y).
 */
void BlanchitProfPProc(image* arg, int p, int x, int y) {
    if (p==0) {
        *arg = Construit_Blanc();
        return;
    }
    if (*arg == NULL) {
        *arg = Construit_Blanc();
        return;
    }
    if ((*arg)->toutnoir)
        *arg = Construit_Composee(Construit_Noir(), Construit_Noir(), Construit_Noir(), Construit_Noir());
    int d = 1 << p - 1;
    if (x < d && y >= d) BlanchitProfPProc(&(*arg)->fils[0], p - 1, x, y - d);
    else if (x >= d && y >= d) BlanchitProfPProc(&(*arg)->fils[1], p - 1, x - d, y - d);
    else if (x < d && y < d) BlanchitProfPProc(&(*arg)->fils[2], p - 1, x, y);
    else BlanchitProfPProc(&(*arg)->fils[3], p - 1, x - d, y);
}

/* Calcule la pronfondeur max de l'image en parametre
 * (on pourrait dire la hauteur du Quadtree en parametre).
 */
int CalculProf(image arg) {
    if (arg == NULL) return 0;
    if (arg -> toutnoir) return 0;
    int prof = 0;
    for (int i = 0; i < 4; i ++) prof = max(prof, CalculProf(arg->fils[i]));
    return prof + 1;
}

/* Renvoie le nombre de pixels dans chaque colonne..
 */
int* HautPixProfP(image arg, int p) {
    int* res = malloc(sizeof(int) * ((1<<p)+1));
    int cpt = 0;
    if (arg == NULL) {
        for (int i = 0; i < 1 << p; i ++) res[cpt++] = 0;
        res[cpt] = -1;
        return res;
    }
    if (arg -> toutnoir) {
        for (int i = 0; i < 1 << p; i ++) res[cpt++] = 1 << p;
        res[cpt] = -1;
        return res;
    }
    int* x[4];
    for (int i = 0; i < 4; i ++) x[i] = HautPixProfP(arg->fils[i], p-1);
    for (int i = 0; i < 1<<p-1; i ++) res[cpt++] = x[0][i]+x[2][i];
    for (int i = 1<<(p-1); i < 1<<p; i ++) res[cpt++] = x[1][i-(1<<(p-1))]+x[3][i-(1<<p-1)];
    res[cpt] = -1;
    for (int i = 0; i < 4; i ++) free(x[i]);
    return res;
}

/* Cette fonction utilise le nombre de pixels dans chaque colonne passees en argument
 * pour construire une image.
 */
image ConstruitImageAvecHautPixAProfP(int* h, int p) {
    bool n = true, b = true;
    for (int i = 0; i < 1<<p; i ++) {
        if (h[i] != 1<<p) n = false;
        if (h[i]) b = false;
    }
    if (n) return Construit_Noir();
    if (b) return Construit_Blanc();
    int* x[4];
    int cpt[4];
    for (int i = 0; i < 4; i ++) {
        x[i] = malloc(sizeof(int) * ((1<<p)+1));
        cpt[i] = 0;
    }
    int d = 1 << p - 1;
    for (int i = 0; i < d; i ++) {
        x[0][cpt[0]++] = max(h[i]-d, 0);
        x[2][cpt[2]++] = min(h[i], d);
    }
    for (int i = d; i < d << 1; i ++) {
        x[1][cpt[1]++] = max(h[i]-d, 0);
        x[3][cpt[3]++] = min(h[i], d);
    }
    return Construit_Composee(ConstruitImageAvecHautPixAProfP(x[0], p-1), ConstruitImageAvecHautPixAProfP(x[1], p-1), ConstruitImageAvecHautPixAProfP(x[2], p-1), ConstruitImageAvecHautPixAProfP(x[3], p-1));
}
/* Rend l'image en parametre avec la force de gravite appliquee.
 * On calcule dans un premier temps la profondeur max de l'image.
 * Qu'on va utiliser pour ensuite obtenir un tableau avec le nombre de
 * pixel dans chaque colonne a cette profondeur. Puis on appelle CIAHPAP avec ce
 * tableau et la profondeur max calculee pour finalement retourner une image
 * avec cet effet de gravite simule.
 */
image Chute(image arg) {
    int prof = CalculProf(arg);
    return ConstruitImageAvecHautPixAProfP(HautPixProfP(arg, prof), prof);
}

/* On ne devine pas dans l'ennonce s'il faut faire une fonction ou une procedure
 * donc nous avons fait les 2 ( 3 ).
 * (cependant c'etait un peu trop facile de juste creer une image dans la procedure
 * puis faire *arg = *img donc nous avons fait une troisieme version dans le doute
 * ou il etait obligatoire de faire une procedure).
 */
void ChuteProc(image* arg){
    int prof = CalculProf(*arg);
    image img = ConstruitImageAvecHautPixAProfP(HautPixProfP(*arg, prof), prof);
    free(*arg);
    *arg = img;
}

//////Version de Chute en procedure qui modifie directement l'image.


/* Modifie l'image en argument au lieu d'en renvoyer une nouvelle comme sa soeur.
 */
void ConstruitImageAvecHautPixAProfPProc(image* arg, int* h, int p) {
    bool n = true, b = true;
    for (int i = 0; i < 1<<p; i ++) {
        if (h[i] != 1<<p) n = false;
        if (h[i]) b = false;
    }
    if (n) {
        *arg = Construit_Noir();
        return;
    }
    if (b) {
        *arg = Construit_Blanc();
        return;
    }
    int* x[4];
    int cpt[4];
    for (int i = 0; i < 4; i ++) {
        x[i] = malloc(sizeof(int) * ((1<<p)+1));
        cpt[i] = 0;
    }
    int d = 1 << p - 1;
    for (int i = 0; i < d; i ++) {
        x[0][cpt[0]++] = max(h[i]-d, 0);
        x[2][cpt[2]++] = min(h[i], d);
    }
    for (int i = d; i < d << 1; i ++) {
        x[1][cpt[1]++] = max(h[i]-d, 0);
        x[3][cpt[3]++] = min(h[i], d);
    }
    *arg = Construit_Composee(NULL, NULL, NULL, NULL);
    for (int i = 0; i < 4; i ++) {
        ConstruitImageAvecHautPixAProfPProc(&((*arg)->fils[i]), x[i], p-1);
    }
    for (int i = 0; i < 4; i ++) {
        free(x[i]);
    }
}

/* Ici on ne remplace pas l'image par une nouvelle image.
 */
void ChuteProc2(image* arg) {
    int prof = CalculProf(*arg);
    int* h = HautPixProfP(*arg, prof);
    ConstruitImageAvecHautPixAProfPProc(arg, h, prof);
    free(h);
}




int main() {
    printf("       VOICI LES TESTS SUR TOUS LES EXEMPLES DONNES DANS LE SUJET : \n\n        ");

    //////                   AFFICHAGES

    printf("Test d'affichage : \n");
    image imgEx1 = Construit_Composee(Construit_Noir(), Construit_Composee(Construit_Blanc(),Construit_Blanc(),Construit_Noir(),Construit_Blanc()), Construit_Blanc(),
                                      Construit_Composee(Construit_Noir(), Construit_Composee(Construit_Noir(),Construit_Noir(),Construit_Blanc(),
                                                                                              Construit_Composee(Construit_Noir(), Construit_Blanc(), Construit_Noir(), Construit_Noir())), Construit_Blanc(), Construit_Noir()));
    Affiche_Normal(imgEx1);
    printf("\n");
    Affiche_Prof(imgEx1);


    //////                  LECTURE

    printf("\n\n        Test sur lecture (on verifie qu'on retrouve bien l'image) : \n");
    image imgLec = Lecture("(N(B     4BNB)B(N(NNB    (NBNN7))BN))");
    Affiche_Normal(imgLec);
    printf("\n");
    Affiche_Prof(imgLec);
    printf("\nOn remarque qu'on reprend l'exemple de l'affichage et qu'on ajoute des caracteres "
           "obsoletes pour voir si on les ignore bien.");


    //////                  EstBlanche    EstNoire

    printf("\n\n        Test sur EstBlanche et EstNoire : \n");
    //Apres le test de Lecture on se permet de l'utilliser pour eviter
    //de perdre du temps de faire l'image à la main.
    image imgBoN = Lecture("(BBB(BB(BBBB)B))");
    Affiche_Normal(imgBoN);
    printf("\n");
    printf("L'image est blanche ? : %s", formatBool(EstBlanche(imgBoN)));
    printf("\nL'image est noire ? : %s", formatBool(EstNoire(imgBoN)));


    //////                  Diagonale

    printf("\n\n        Test de Diagonale : \n");
    printf("Diagonale(3) : ");
    Affiche_Normal(Diagonale(3));


    //////                  Quart de tour
    printf("\n\n        Test sur QuartDeTour : \n");
    image imgQDT = Lecture("((BNNN)B(NNBB)N)");
    Affiche_Normal(imgQDT);
    printf("\nQDT : ");
    Affiche_Normal(QuartDeTour(imgQDT));
    printf("\nAvec procedure : ");
    QuartDeTour_Proc(&imgQDT);
    Affiche_Normal(imgQDT);


    //////                  Negatif
    printf("\n\n        Test sur Negatif (pas d'exemple donc je reutilise les anciens) : \n");
    Affiche_Normal(imgEx1);
    printf("     --------->>>>>>>>>>>    NEG : ");
    Negatif(&imgEx1);
    Affiche_Normal(imgEx1);
    printf("\n");
    Affiche_Normal(imgBoN);
    Negatif(&imgBoN);
    printf("     --------->>>>>>>>>>>    NEG : ");
    Affiche_Normal(imgBoN);


    //////                  Simplifie a profondeur P
    printf("\n\n        Test sur SimplifieProfP : \n");
    image imgSPP = Lecture("( N (NB(NN(NNNN)N)B) (NBN(NBN(BBBB))) (BB(BBBB)B) )");
    Affiche_Normal(imgSPP);
    printf("\nEn simplifiant a profondeur 2 : ");
    SimplifieProfP(&imgSPP, 2);
    Affiche_Normal(imgSPP);


    //////                  Incluse
    printf("\n\n        Test sur Incluse : \n");
    image imgInc1 = Lecture("(((BBBB)NBN)BN((BBNN)BB(NBBN)))");
    image imgInc2 = Lecture("((BNNN)(BBNB)(NNNN)(NBN(NNNB)))");
    printf ("(((BBBB)NBN)BN((BBNN)BB(NBBN))) incluse dans ((BNNN)(BBNB)(NNNN)(NBN(NNNB))) ? : %s", formatBool(Incluse(imgInc1, imgInc2)));
    printf("\nEn modifiant le dernier pixel problematique : \n");
    image imgInc1Bis = Lecture("(((BBBB)NBN)BN((BBNN)BB(NBBN)))");
    image imgInc2Bis = Lecture("((BNNN)(BBNB)(NNNN)(NBN(NNNN)))");
    printf ("(((BBBB)NBN)BN((BBNN)BB(NBBN))) incluse dans ((BNNN)(BBNB)(NNNN)(NBN(NNNN))) ? : %s", formatBool(Incluse(imgInc1Bis, imgInc2Bis)));


    //////                  Hauteur Max Blanc
    printf("\n\n        Test sur HautMaxBlanc : \n");
    image imgHmb1 = Lecture("( (BBBB) N (BNBN) (NBN(NB(BB(BBBB)(BBB(BBBB)))N)))");
    printf("((BBBB)N(BNBN)(NBN(NB(BB(BBBB)(BBB(BBBB)))N))) : %d", HautMaxBlanc(imgHmb1));
    printf("\n(N(NNNN)N(NNNN)) : %d", HautMaxBlanc(Lecture("(N(NNNN)N(NNNN)")));


    //////                  Blanchit à profonfeur P
    printf("\n\n        Test sur BlanchitProfP : \n");
    image imgBP1 = Lecture("(NBNB)");
    image imgBP2 = Lecture("((N(NBNN)NN)BNB)");
    printf("(NBNB) : ");
    Affiche_Normal(BlanchitProfP(imgBP1, 2, 1, 3));
    printf("\n((N(NBNN)NN)BNB) : ");
    Affiche_Normal(BlanchitProfP(imgBP2, 2, 1, 3));

    printf("\nAvec procedure : \n");
    printf("(NBNB) : ");
    BlanchitProfPProc(&imgBP1, 2, 1, 3);
    Affiche_Normal(imgBP1);
    printf("\n((N(NBNN)NN)BNB) : ");
    BlanchitProfPProc(&imgBP2, 2, 1, 3);
    Affiche_Normal(imgBP2);


    //////                  Chute
    printf("\n\n        Test sur chute : \n");
    image imgChute = Lecture("( N ((NBNN)NB(BNBN)) (N(NBBN)(BNNN)(NBNB)) (NN(NBBB) (BNBN))) ");
    Affiche_Normal(imgChute);
    printf("\nEn appliquant la gravite : \n");
    Affiche_Normal(Chute(imgChute));

    printf("\nAvec procedure : \n");
    ChuteProc(&imgChute);
    Affiche_Normal(imgChute);
    imgChute = Lecture("( N ((NBNN)NB(BNBN)) (N(NBBN)(BNNN)(NBNB)) (NN(NBBB) (BNBN))) ");

    printf("\nAvec procedure V2 : \n");
    ChuteProc2(&imgChute);
    Affiche_Normal(imgChute);


    return 0;
}

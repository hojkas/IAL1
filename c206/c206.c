
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize  **Dark** Elf ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

  L->First = NULL;
  L->Act = NULL;
  L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/

  L->Act = L->First;
  while(L->Act != L->Last) { //dokud není dosaženo posledního prvku, ten sám vymazán nebude
    //posune se o prvek dál
    L->Act = L->Act->rptr;
    //zruší předchozí prvek
    free(L->Act->lptr);
  }
  free(L->Act); //zruší poslední == aktivní prvek
  //návrat do počátečního stavu:
  L->First = NULL;
  L->Act = NULL;
  L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  tDLElemPtr toAdd = malloc(sizeof(struct tDLElem));
  if(toAdd == NULL) DLError();
  else {
    toAdd->data = val;
    toAdd->lptr = NULL;

    if(L->First == NULL) {
      //je-li seznam úplně prázdný, naváže položku na začátek i konec
      L->First = toAdd;
      L->Last = toAdd;
      toAdd->rptr = NULL;
    }
    else {
      //obsahuje-li seznam již nějakou položku prováže ji s touto
      L->First->lptr = toAdd;
      toAdd->rptr = L->First;
      L->First = toAdd;
    }
  }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr toAdd = malloc(sizeof(struct tDLElem));
  if(toAdd == NULL) DLError();
  else {
    toAdd->data = val;
    toAdd->rptr = NULL;

    if(L->Last == NULL) {
      //je-li seznam úplně prázdný, naváže položku na začátek i konec
      L->First = toAdd;
      L->Last = toAdd;
      toAdd->lptr = NULL;
    }
    else {
      //obsahuje-li seznam již nějakou položku, prováže ji s touto
      L->Last->rptr = toAdd;
      toAdd->lptr = L->Last;
      L->Last = toAdd;
    }
  }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->First == NULL) DLError();
  else *val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->Last == NULL) DLError();
  else *val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  if(L->First == NULL) return; //seznam prázdný, nedělá nic

  if(L->First == L->Last) {
    //specialni chovani v pripade, ze je prvek jeidny v seznamu
    free(L->First);
    L->Last = NULL;
    L->First = NULL;
    L->Act = NULL;
  }
  else {
    //je-li prvni prvek aktivni, zrusi aktivitu, aby k neexistujici polozce
    //v budoucnu nebyl zadan pristup
    if(L->Act == L->First) L->Act = NULL;
    //nastaví prvek za tím co bude zrušený jako první
    L->First = L->First->rptr;
    //vymaže první prvek
    free(L->First->lptr);
    //vymaže spojení
    L->First->lptr = NULL;
  }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  if(L->First == NULL) return; //seznam prázdný, nedělá nic

  if(L->First == L->Last) {
    //specialni chovani v pripade, ze je prvek jeidny v seznamu
    free(L->Last);
    L->Last = NULL;
    L->First = NULL;
    L->Act = NULL;
  }
  else {
    //je-li prvni prvek aktivni, zrusi aktivitu, aby k neexistujici polozce
    //v budoucnu nebyl zadan pristup
    if(L->Act == L->Last) L->Act = NULL;
    //nastaví prvek za tím co bude zrušený jako první
    L->Last = L->Last->lptr;
    //vymaže první prvek
    free(L->Last->rptr);
    //vymaže spojení
    L->Last->rptr = NULL;
  }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

  //je-li seznam neaktivni nebo je-li aktivni prvek poslednim (druha podminka
  //zaroven vylouci take moznost prazdneho seznamu), funkce nic nevykona
  if(L->Act == NULL || L->Act == L->Last) return;

  if(L->Act->rptr == L->Last) {
    //ma-li byt odstraneny posledni prvek
    free(L->Last);
    L->Act->rptr = NULL;
    L->Last = L->Act;
  }
  else {
    //je-li odstranovany prvek uprostred seznamu
    //ulozim ukazatel na prvek, ktery potrebuji po odstraneni predchoziho napojit
    //na aktivni prvek
    tDLElemPtr nextinline = L->Act->rptr->rptr;
    free(L->Act->rptr);
    //navazani roztrhnuteho seznamu
    L->Act->rptr = nextinline;
    nextinline->lptr = L->Act;
  }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

  //neaktivni seznam, prazdny nebo s akt. na zacatku nic neprovede
  if(L->Act == NULL || L->Act == L->First) return;

  if(L->Act->lptr == L->First) {
    //ma-li byt odstraneny prvni prvek
    free(L->First);
    L->Act->lptr = NULL;
    L->First = L->Act;
  }
  else {
    //je-li odstranovany prvek uprostred seznamu
    //pomocny prvek je ten pred odstranovanym
    tDLElemPtr previnline = L->Act->lptr->lptr;
    free(L->Act->lptr);
    //navazani roztrhnuteho seznamu
    L->Act->lptr = previnline;
    previnline->rptr = L->Act;
  }

}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act == NULL) return;

  tDLElemPtr toAdd = malloc(sizeof(struct tDLElem));
  if(toAdd == NULL) { //kontrola prirazeni pameti
    DLError();
    return;
  }

  toAdd->data = val;

  if(L->Act == L->Last) {
    //je-li aktivni prvek poslednim
    L->Act->rptr = toAdd;
    toAdd->lptr = L->Act;
    toAdd->rptr = NULL;
    L->Last = toAdd;
  }
  else {
    //aktivni neni posledni
    //nastaveni ukazatelu toAdd prvku
    toAdd->rptr = L->Act->rptr;
    toAdd->lptr = L->Act;
    //nastaveni ukazatelu kolem prvku toAdd na něj
    toAdd->rptr->lptr = toAdd;
    L->Act->rptr = toAdd;
  }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act == NULL) return;

  tDLElemPtr toAdd = malloc(sizeof(struct tDLElem));
  if(toAdd == NULL) { //kontrola prirazeni pameti
    DLError();
    return;
  }

  toAdd->data = val;

  if(L->Act == L->First) {
    //je-li aktivni prvek prvnim
    L->Act->lptr = toAdd;
    toAdd->rptr = L->Act;
    toAdd->lptr = NULL;
    L->First = toAdd;
  }
  else {
    //aktivni neni prvni
    //nastaveni ukazatelu toAdd prvku
    toAdd->lptr = L->Act->lptr;
    toAdd->rptr = L->Act;
    //nastaveni ukazatelu kolem prvku toAdd na něj
    toAdd->lptr->rptr = toAdd;
    L->Act->lptr = toAdd;
  }

}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
  if(L->Act == NULL) DLError();
  else *val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
  if(L->Act != NULL) L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
  if(L->Act != NULL) L->Act = L->Act->rptr;
  //posledni prvek neni treba osetrovat, pri posledni polozce se do L->Act
  //prekopiruje hodnota rptr, coz je u posledniho prvku NULL
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
  if(L->Act != NULL) L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
  return (L->Act != NULL);
  //vraci hodnotu vyrazu, je-li aktivni, podminka plati, takze nenulovou (true)
  //je-li prazdny, vrati 0 (false)
}

/* Konec c206.c*/

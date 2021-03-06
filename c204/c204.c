
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
  char popped;
  //check jestli neni prazdny
  if(stackEmpty(s)) return;
  //ulozi vrchol zasobniku do popped a popne jednu polozku
  stackTop(s, &popped);
  stackPop(s);
  while(!stackEmpty(s) && popped != '(') {//pri nalezeni ( cyklus konci, netreba nic zapisovat do postExpr
    postExpr[*postLen] = popped;
    (*postLen)++;
    stackTop(s, &popped);
    stackPop(s);
  } //neresim pripad, kdy v popped neco zustane nevypsano - to by se mohlo stat
  //pouze v pripade spatneho vstupniho vyrazu, coz nemam resit
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat,
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {

  char atTop;
  while(1) {
    if(stackEmpty(s)) {
      stackPush(s, c);
      return;
    } //v pripade prazdneho zasobniku rovnou ulozi operator na vrchol

    stackTop(s, &atTop); //nacte do atTop vrchol zasobniku
    if(atTop == '(' || ( (c == '/' || c == '*') && (atTop == '+' || atTop == '-') )) {
      stackPush(s, c);
      return;
    } //najde-li ( nebo operator s nizsi prioritou, push c a konec funkce

    //sem cyklus dojde pouze pokud na vrcholu není ani ( ani operator s nizsi prioritou,
    //aka na vrcholu je operator se stejnou nebo vyssi prioritou

    stackPop(s); //odstrani predchozi operator ze stacku
    postExpr[*postLen] = atTop;
    (*postLen)++; //ulozi vybrany operator do postExpr
    //cyklus se opakuje, znova pokus o ulozeni operatoru na zasobnik
  }
}

/*
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202.
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
  tStack stack;
  stackInit(&stack);

  //indexy (pos = position) v infixové a postfixové notaci, postPos je zaroven postLen
  unsigned infPos = 0;
  unsigned postPos = 0;

  char* postExpr = malloc(sizeof(char) * MAX_LEN);

  if(infExpr == NULL || postExpr == NULL) return NULL; //chyba alokace nebo spatny
  //parametr, vraci NULL

  while(1) {
    if(infExpr[infPos] == '=') break;

    if(infExpr[infPos] == '(') stackPush(&stack, infExpr[infPos]);
    else if(infExpr[infPos] == ')') untilLeftPar(&stack, postExpr, &postPos);
    else if(infExpr[infPos] == '*' || infExpr[infPos] == '+' || infExpr[infPos] == '-' || infExpr[infPos] == '/')  doOperation(&stack, infExpr[infPos], postExpr, &postPos);
    else {
      postExpr[postPos] = infExpr[infPos];
      postPos++;
    }

    infPos++;
  }

  //sem skoci az v momente, kdy narazi na =
  while(!stackEmpty(&stack)) {
    stackTop(&stack, &postExpr[postPos]); //nahraje vrchol zasobniku na konec postExpr
    postPos++;
    stackPop(&stack);
  }
  postExpr[postPos] = '=';
  postPos++;
  postExpr[postPos] = '\0';
  postPos++;
  return postExpr;
}

/* Konec c204.c */

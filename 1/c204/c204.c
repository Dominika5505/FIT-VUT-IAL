
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

    // Pomocná premenná na uloženie vrcholu zásobníku
    char top;

    while (!Stack_IsEmpty(stack)) {
        // Ak zásobník nie je prázdny, uloží vrchol do premennej top
        Stack_Top(stack, &top);
        // Odstráni vrchol zásobníku
        Stack_Pop(stack);
        // Ak bol vrchol zás. znak '(', ukončím cyklus, pretože znak '(' sa nezapisuje do výsledného reťazca 
        if (top == '(') return;
        // Do výsledného reťazca zapíšem vrchol zás a inkrementujem dĺžku výsl. reťazca
        postfixExpression[(*postfixExpressionLength)++] = top;
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    // Pomocná premenná na uloženie vrcholu zásobníku 
    char top;

    while (TRUE) {

        // Ak zásobník nie je prázdny, uložím si vrchol zásobníku do premennej top
        if (!Stack_IsEmpty(stack)) {
            Stack_Top(stack, &top);
        } 

        // Ak je zásobník prázdny alebo na vrchole je '(', na vrchol vložím operátor v c a ukončím loop
        if (Stack_IsEmpty(stack) || top == '(') {
            Stack_Push(stack, c);
            break;
        }

        // Ak c je operátor + alebo -, na vrchole môže byť len operátor so shodnou alebo vyššiou prioritou
        // (prázdny zásobník a vrchol '(' sú ošetrené vyššie)
        // Vrchol zásobníka vložím na koniec reťazca, dĺžku reťazca zvýšim o 1, odstránim vrchol
        // Loop sa opakuje
        if (c == '+' || c == '-') {
            postfixExpression[(*postfixExpressionLength)++] = top;
            Stack_Pop(stack);
            continue;
        }

        // Ak c je op * alebo - môžu nastať len dve možnosti, buď bude na vrchole op s rovnakou/vuššiou prioritou alebo op s nižšiou prioritou
        if (c == '*' || c == '/') {
            // Ak je na vrchole op s rovnakou al. vyššiou prior
            if (top == '*' || top == '/') {
                postfixExpression[(*postfixExpressionLength)++] = top;
                Stack_Pop(stack);
                continue;
            // Nižšia priorita
            } else {
                Stack_Push(stack, c);
                break;
            }
        }

    }

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

    // Vytvorenie a alokácia výsledného reťazca
    char *postfixExpression = malloc(sizeof(char) * MAX_LEN);
    if (postfixExpression == NULL) return NULL;

    // Alokácia a inicializácia nového zásobníka
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    Stack_Init(stack);

    // Pomocná premenná na uloženie momentálneho znaku z reťazca infixExpression
    char currChar;
    // Pozícia/dĺžka výsledného (postfix) reťazca 
    unsigned pfExprPos = 0;

    // Cyklus prebieha, až kým nepríde na koniec pôvodného reťazca (infixExpression)
    for (unsigned infExprPos = 0; infixExpression[infExprPos] != '\0'; infExprPos++) {
        // Pri každom behu cyklu si uložím momentálny znak z pôvodného reťazca do pomocnej premennej
        currChar = infixExpression[infExprPos];

        // V prípade, že currChar je operátor, spustím funkciu doOperation, kde sa rozhodne priorita op, atď
        // Loop sa opakuje pre nasledujúci currChar
        if (currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/') {
            doOperation(stack, currChar, postfixExpression, &pfExprPos);
            continue;
        }

        // V prípade, že currChar je alfanumerický znak, zapíšem ho do výsledného reťazca a inkrementujem jeho dĺžku
        // Loop sa opakuje pre nasledujúci currChar
        if ((currChar >= '0' && currChar <= '9') || (currChar >= 'a' && currChar <= 'z') || (currChar >= 'A' && currChar <= 'Z')) {
            postfixExpression[pfExprPos++] = currChar;
            continue;
        }

        // V prípade, že curChar je znak '(', vložím ho na vrchol zásobníka
        // Loop sa opakuje pre nasledujúci currChar
        if (currChar == '(') {
            Stack_Push(stack, currChar);
            continue;
        }

        // V prípade, že curChar je znak ')', zavolám funkciu untilLeftPar, ktorá vyprázni znaky až po ľavú zátvorku
        // Loop sa opakuje pre nasledujúci currChar
        if (currChar == ')') {
            untilLeftPar(stack, postfixExpression, &pfExprPos);
            continue;
        }

        // Ak je currChar '=', zapíšem zvyšok zásobníka do výsledného reťazca a zásobník vyprázdnim
        if (currChar == '=') {
            while (!Stack_IsEmpty(stack)) {
                Stack_Top(stack, &postfixExpression[pfExprPos++]);
                Stack_Pop(stack);
            }
            // Na koniec reťazca pridám = a ukončím reťazec
            postfixExpression[pfExprPos++] = '=';
            postfixExpression[pfExprPos] = '\0';
            break;
        }
    }

    // Uvoľním zásobník
    free(stack);
    // Vrátim výsledný reťazec
    return postfixExpression;
}

/* Konec c204.c */

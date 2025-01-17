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

bool solved;

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
void untilLeftPar(Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength) {
    // check if stack is empty and if it is, return from function
    if (Stack_IsEmpty(stack)) {
        return;
    } else {
        char top;
        // while stack is not empty pop elements from stack and add them to postfixExpression
        while (Stack_IsEmpty(stack) == false) {
            Stack_Top(stack, &top);
            // if top of stack is left parenthesis, pop it and break from loop
            if (top == '(') {
                Stack_Pop(stack);
                break;
            }
            // pop top of stack and add it to postfixExpression
            Stack_Pop(stack);
            postfixExpression[(*postfixExpressionLength)++] = top;
        }
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
void doOperation(Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength) {
    // check if stack is empty and if it is, push c to stack and return from function
    if (Stack_IsEmpty(stack) == true) {
        Stack_Push(stack, c);
        return;
    } else {
        char top;
        while (Stack_IsEmpty(stack) == false) {
            Stack_Top(stack, &top);
            // if priority of c is lower than priority of top of stack or top of stack is left parenthesis, push c to stack and return from function
            if (((top == '+' || top == '-') && (c == '*' || c == '/')) ||
                top == '(') {
                Stack_Push(stack, c);
                return;
            }
            // pop top of stack and add it to postfixExpression
            postfixExpression[*postfixExpressionLength] = top;
            (*postfixExpressionLength)++;
            Stack_Pop(stack);
        }
        Stack_Push(stack, c);
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
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix(const char *infixExpression) {
    int pos = 0;
    unsigned length = 0;

    // allocate memory for stack
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) { // check if allocation was successful
        return NULL;
    }
    Stack_Init(stack); // initialize stack

    // allocate memory for postfixExpression
    char *postfixExpression = malloc(MAX_LEN * sizeof(char));
    if (postfixExpression == NULL) { // check if allocation was successful
        free(stack); // free memory for stack
        return NULL;
    }

    // while infixExpression is not empty
    while (infixExpression[pos] != '\0') {
        // read character from infixExpression
        char c = infixExpression[pos];

        // if character is left parenthesis, push it to stack
        if (c == '(') {
            Stack_Push(stack, c);
        } else if (c == ')') { // if character is right parenthesis, call untilLeftPar function
            untilLeftPar(stack, postfixExpression, &length);
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            // if character is operand, add it to postfixExpression
            postfixExpression[length++] = c;;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // if character is operator, call doOperation function
            doOperation(stack, c, postfixExpression, &length);
        } else if (c == '=') {
            while (Stack_IsEmpty(stack) == false) {
                Stack_Top(stack, &(postfixExpression[length++]));
                Stack_Pop(stack);
            }
            postfixExpression[length++] = '=';
            break;
        }
        pos++;
    }
    postfixExpression[length++] = '\0';

    free(stack);
    return postfixExpression;
}

/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push(Stack *stack, int value) {
    stack->topIndex++; // Increment the top index
    int *newPtr = (int *)&stack->array[stack->topIndex]; // Calculate the address for the new element
    *newPtr = value; // Assign the value to the new element
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop(Stack *stack, int *value) {
    if (stack->topIndex >= 0) {
        *value = stack->array[stack->topIndex]; // Get the value from the top of the stack
        stack->topIndex--; // Decrement the top index
    } else {
        *value = 0; // Set the value to 0 when the stack is empty
    }
}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval(const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *result) {
    char *postfixExpression = infix2postfix(infixExpression);
    if (postfixExpression == NULL) {
        return false;
    }

    Stack expressionStack;
    Stack_Init(&expressionStack);

    for (int i = 0; postfixExpression[i]; i++) {
        char currentToken = postfixExpression[i];

        if (currentToken == '=') {
            continue; // Skip the assignment operator
        } else if (currentToken >= '0' && currentToken <= '9') {
            // If it's a digit, push its integer value onto the stack
            expr_value_push(&expressionStack, currentToken - '0');
        } else if ((currentToken >= 'a' && currentToken <= 'z') || (currentToken >= 'A' && currentToken <= 'Z')) {
            // If it's an alphabetic character, find its corresponding value in variableValues
            char variableName = currentToken;
            int variableValue = 0;

            for (int j = 0; j < variableValueCount; j++) {
                if (variableValues[j].name == variableName) {
                    variableValue = variableValues[j].value;
                    break;
                }
            }

            expr_value_push(&expressionStack, variableValue);
        } else {
            // If it's an operator, perform the operation on the top two values in the stack
            int operator2;
            expr_value_pop(&expressionStack, &operator2);

            int operator1;
            expr_value_pop(&expressionStack, &operator1);

            int operationResult;

            if (currentToken == '+') {
                operationResult = operator1 + operator2;
            } else if (currentToken == '-') {
                operationResult = operator1 - operator2;
            } else if (currentToken == '*') {
                operationResult = operator1 * operator2;
            } else if (currentToken == '/') {
                // Handle division by zero
                if (operator2 == 0) {
                    Stack_Dispose(&expressionStack);
                    free(postfixExpression);
                    return false;
                }
                operationResult = operator1 / operator2;
            } else {
                // Handle unsupported operator
                Stack_Dispose(&expressionStack);
                free(postfixExpression);
                return false;
            }

            expr_value_push(&expressionStack, operationResult);
        }
    }

    // The result is the top value on the stack
    expr_value_pop(&expressionStack, result);
    Stack_Dispose(&expressionStack);
    free(postfixExpression);

    return true;
}

/* Konec c204.c */

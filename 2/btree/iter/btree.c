/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  bst_node_t *current = tree;

  while (current != NULL) {

    if (key == current->key) {

      *value = current->value;
      return true;

    } else if (key < current->key) {
      
      current = current->left;
      
    } else {
      
      current = current->right;

    }
  }
  
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  // Uloží momentálny uzol, cez ktorý sa bude iterovať
  bst_node_t *current = *tree;
  // Uloží otca iterovaného uzlu
  bst_node_t *parent = NULL;
  // Pravdivostná hodnota, ktorá značí, do ktorého podstromu sa bude hodnota ukladať
  bool right;

  while (current != NULL) {

    if (key == current->key) {

      current->value = value;
      return;
      
    } else if (key < current->key) {

      parent = current;
      current = current->left;
      right = false;

    } else {

      parent = current;
      current = current->right;
      right = true;

    }
    
  }

  // Vytvorenie nového prvku
  bst_node_t *new_node = malloc(sizeof(bst_node_t));
  if (new_node == NULL) return;

  new_node->key = key;
  new_node->value = value;
  new_node->left = NULL;
  new_node->right = NULL;

  // Ak je strom prázdny, nový prvok sa uloží ako root, inak sa uloží na niektorý podstrom
  if (parent == NULL) {
    *tree = new_node;  
  } else {
    right ? (parent->right = new_node) : (parent->left = new_node);
  }

}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;

  while (current != NULL) {

    if (current->right == NULL) {

      target->key = current->key;
      target->value = current->value;

      if (parent == NULL) {
        *tree = current->left;
      } else {
        parent->right = current->left;
      }

      free(current);
      return;
      
    }

    parent = current;
    current = current->right;
    
  }
  
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;
  int found = 0; // 0 == not found; -1 == left; 1 == right; 2 == root

  while (current != NULL) {

    if (key == current->key) {

      if (parent == NULL) 
        found = 2;

      if (current->left != NULL && current->right != NULL) {

        bst_replace_by_rightmost(current, tree);
        return;

      } else if (current->left != NULL && current->right == NULL) {

        if (found == 2) 
          *tree = current->left;
        else if (found == -1)
          parent->left = current->left;
        else        
          parent->right = current->left;


      } else if (current->left == NULL && current->right != NULL) {

        if (found == 2) 
          *tree = current->right;
        else if (found == -1)
          parent->left = current->right;
        else
          parent->right = current->right;

      } else {

        if (found == -1) 
          parent->left = NULL;
        else if (found == 1) 
          parent->right = NULL;

      }

      free(current);
      current = NULL;
      return;
        
    } else if (key < current->key) {

      parent = current;
      current = current->left;
      found = -1;
      
    } else {

      parent = current;
      current = current->right;
      found = 1;
      
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

  if (*tree == NULL) return;

  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);
  
  do {
    if (*tree == NULL) {
      if (!stack_bst_empty(stack)) {
        *tree = stack_bst_top(stack);
        stack_bst_pop(stack);
      }
    } else {
      if ((*tree)->right != NULL) {
        stack_bst_push(stack, (*tree)->right);
      }
      bst_node_t *temp = (*tree);

      *tree = (*tree)->left;

      free(temp);
      temp = NULL;
      
    }
  } while ((*tree != NULL) || (!stack_bst_empty(stack)));

  free(stack);

}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {

  bst_node_t *node = tree;

  while (node != NULL) {

    stack_bst_push(to_visit, node);
    bst_print_node(node);
    node = node->left;

  }

  
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {

  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);

  bst_node_t *node = tree;
  bst_leftmost_preorder(node, stack);

  while(!stack_bst_empty(stack)) {
    node = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(node->right, stack);
  }

  free(stack);

}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

  bst_node_t *node = tree;

  while (node != NULL) {
    stack_bst_push(to_visit, node);
    node = node->left;
  }
  
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {

  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);

  bst_node_t *node = tree;

  bst_leftmost_inorder(node, stack);

  while (!stack_bst_empty(stack)) {
    node  = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_print_node(node);
    bst_leftmost_inorder(node->right, stack);
  }

  free(stack);
  
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {

  bst_node_t *node = tree;

  while (node != NULL) {

    stack_bst_push(to_visit, node);
    stack_bool_push(first_visit, true);
    node = node->left;
    
  }
  
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

  stack_bst_t *stack_bst = malloc(sizeof(stack_bst_t));
  if (stack_bst == NULL) return;
  stack_bool_t *stack_bool = malloc(sizeof(stack_bool_t));
  if (stack_bool == NULL) return;

  stack_bst_init(stack_bst);
  stack_bool_init(stack_bool);

  bool left;

  bst_node_t *node = tree;

  bst_leftmost_postorder(node, stack_bst, stack_bool);

  while (!stack_bst_empty(stack_bst)) {

    node = stack_bst_top(stack_bst);
    left = stack_bool_top(stack_bool);
    stack_bool_pop(stack_bool);

    if (left) {
      stack_bool_push(stack_bool, false);
      bst_leftmost_postorder(node->right, stack_bst, stack_bool);
    } else {
      stack_bst_pop(stack_bst);
      bst_print_node(node);
    }
  }

  free(stack_bst);
  free(stack_bool);

  
}

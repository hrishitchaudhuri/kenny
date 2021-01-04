#include<stdio.h>
#include<stdlib.h>
#include"nfa.h"

State *state_list[100];
int state_count = 0;

int is_oper(char c) {
    return (c == '*' || c == '.' || c == '+') ? 1 : 0;
}

Nfa *nfa_pop(Nfa **n, int *i) {
    Nfa* x = n[*i];
    --*i;
    return x;
}

void nfa_push(Nfa *r, Nfa **s, int *i){
    ++*i; s[*i] = r;
}

void init_transitions(Transition **t) {
    t[0] = (Transition *) malloc(sizeof(Transition));
    t[1] = (Transition *) malloc(sizeof(Transition));

    t[0]->next = NULL; t[1]->next = NULL;
}

Nfa *char_automaton(char c) {
    Nfa* nf = (Nfa *) malloc(sizeof(Nfa));
    
    nf->start = (State *) malloc(sizeof(State));
    nf->final = (State *) malloc(sizeof(State));

    nf->start->final = 0;
    nf->final->final = 1;

    init_transitions(nf->start->transitions);
    init_transitions(nf->final->transitions);

    nf->start->transitions[0]->sig = c;
    nf->start->transitions[0]->next = nf->final;

    state_list[state_count] = nf->start;
    nf->start->id = state_count++;

    state_list[state_count] = nf->final;
    nf->final->id = state_count++;

    return nf;
}

Nfa *concat_automata(Nfa *n1, Nfa *n2) {
    Nfa *nf = (Nfa *) malloc(sizeof(Nfa));

    nf->start = (State *) malloc(sizeof(State));
    nf->final = (State *) malloc(sizeof(State));

    init_transitions(nf->start->transitions);
    init_transitions(nf->final->transitions);

    nf->start->final = 0;
    nf->final->final = 1;

    nf->start->transitions[0]->sig = 'e';
    nf->start->transitions[0]->next = n1->start;

    n1->final->final = 0;
    n2->final->final = 0;

    n1->final->transitions[0]->sig = 'e';
    n1->final->transitions[0]->next = n2->start;

    n2->final->transitions[0]->sig = 'e';
    n2->final->transitions[0]->next = nf->final;

    state_list[state_count] = nf->start;
    nf->start->id = state_count++;

    state_list[state_count] = nf->final;
    nf->final->id = state_count++;

    return nf;
}

Nfa *disjunct_automata(Nfa *n1, Nfa *n2) {
    Nfa *nf =(Nfa *) malloc(sizeof(Nfa));
    
    nf->start = (State *) malloc(sizeof(State));
    nf->final = (State *) malloc(sizeof(State));

    init_transitions(nf->start->transitions);
    init_transitions(nf->final->transitions);

    n1->final->final = 0;
    n2->final->final = 0;

    nf->start->final = 0;
    nf->final->final = 1;

    nf->start->transitions[0]->sig = 'e';
    nf->start->transitions[0]->next = n1->start;

    nf->start->transitions[1]->sig = 'e';
    nf->start->transitions[1]->next = n2->start;

    n1->final->transitions[0]->sig = 'e';
    n1->final->transitions[0]->next = nf->final;

    n2->final->transitions[0]->sig = 'e';
    n2->final->transitions[0]->next = nf->final;

    state_list[state_count] = nf->start;
    nf->start->id = state_count++;

    state_list[state_count] = nf->final;
    nf->final->id = state_count++;
    
    return nf;
}

Nfa *Kleene_star(Nfa *n) {
    Nfa *nf = (Nfa *) malloc(sizeof(Nfa));

    nf->start = (State *) malloc(sizeof(State));
    nf->final = (State *) malloc(sizeof(State));

    init_transitions(nf->start->transitions);
    init_transitions(nf->final->transitions);

    nf->start->final = 0;
    nf->final->final = 1;

    n->final->final = 0;

    nf->start->transitions[0]->sig = 'e';
    nf->start->transitions[0]->next = n->start;

    nf->start->transitions[1]->sig = 'e';
    nf->start->transitions[1]->next = nf->final;

    n->final->transitions[0]->sig = 'e';
    n->final->transitions[0]->next = nf->final;

    n->final->transitions[1]->sig = 'e';
    n->final->transitions[1]->next = n->start;

    state_list[state_count] = nf->start;
    nf->start->id = state_count++;

    state_list[state_count] = nf->final;
    nf->final->id = state_count++;

    return nf;
}

Nfa* create_nfa(char *postfix_regex) {
    int i = 0, top = -1;
    Nfa *res, *stack[10], *op1, *op2;

    while (postfix_regex[i] != '\0') {
        char ch = postfix_regex[i];

        if (is_oper(ch)) {
            switch(ch) {
                case '.' :
                    op1 = nfa_pop(stack, &top);
                    op2 = nfa_pop(stack, &top);
                    res = concat_automata(op2, op1);
                    nfa_push(res, stack, &top);
                    break;

                case '+' :
                    op1 = nfa_pop(stack, &top);
                    op2 = nfa_pop(stack, &top);
                    res = disjunct_automata(op2, op1);
                    nfa_push(res, stack, &top);
                    break;

                case '*' :
                    op1 = nfa_pop(stack, &top);
                    res = Kleene_star(op1);
                    nfa_push(res, stack, &top);
                    break;
            }
        }

        else {
            res = char_automaton(ch);
            nfa_push(res, stack, &top);
        }

        i++;
    }

    return nfa_pop(stack, &top);
}

void display_nfa(Nfa *nf) {
    printf("START STATE: %d\n", nf->start->id);
    for (register int i = 0; i < state_count; i++) {
        printf("\n %d", i); if (state_list[i]->final) printf("* -- "); else printf("  -- ");
        if (state_list[i]->transitions[0]->next) {
            printf(" %c <=> %d", state_list[i]->transitions[0]->sig, state_list[i]->transitions[0]->next->id);
            if (state_list[i]->transitions[0]->next->final) printf("*");
        }

        if (state_list[i]->transitions[1]->next) {
            printf(" %c <=> %d", state_list[i]->transitions[1]->sig, state_list[i]->transitions[1]->next->id);
            if (state_list[i]->transitions[1]->next->final) printf("*");
        }
    }
    printf("\n");
}

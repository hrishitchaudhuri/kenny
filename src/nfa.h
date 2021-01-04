typedef struct State State;
typedef struct Transition Transition;

typedef struct State {
    int final;
    int id;
    Transition *transitions[2];
} State;

typedef struct Transition {
    char sig;
    State *next;
} Transition;

typedef struct Automaton {
    State *start;
    State *final;
} Nfa;

int is_oper(char);
Nfa *nfa_pop(Nfa **, int *);
void nfa_push(Nfa *, Nfa **, int *);
void init_transitions(Transition **);
Nfa *char_automaton(char);
Nfa *concat_automata(Nfa *, Nfa *);
Nfa *disjunct_automata(Nfa *, Nfa *);
Nfa *Kleene_star(Nfa *);
Nfa* create_nfa(char *);
void display_nfa(Nfa *);

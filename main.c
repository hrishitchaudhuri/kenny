#include<stdio.h>
#include<stdlib.h>
#include"src/nfa.h"
#include"src/postfix.h"

int main() {
    char regex[20], post_regex[20];

    printf("[IN]: ");
    scanf("%s", regex);
    convert_postfix(regex, post_regex);

    printf("%s\n", post_regex);

    Nfa* nf = create_nfa(post_regex);
    display_nfa(nf);

    return 0;
}
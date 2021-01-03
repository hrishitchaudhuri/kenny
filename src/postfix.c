#include<stdio.h>

char peep(char *s, int t) {
    return s[t];
}

void push(char *s, int *t, char ch) {
    (*t)++;
    s[*t]=ch;
}

char pop(char *s, int *t) {
    return s[(*t)--];
}

int input_prec(char ch) {
    switch (ch) {
        case '+':
            return 1;

        case '.':
            return 3;
        
        case '*':
            return 5;

        case ')':
            return 0;

        case '(':
            return 9;
    
        default:
            return 7;
    }
}

int stack_prec(char ch) {
    switch (ch) {
        case '+':
            return 2;

        case '.':
            return 4;

        case '*':
            return 6;

        case '#':
            return -1;

        case '(':
            return 0;
    
        default:
            return 8;
    }
}

void convert_postfix(char *s, char *t) {
    char ch, stack[10]; 
    int head = -1, i = 0, j = 0;

    push(stack, &head, '#');

    while(s[i] != '\0') {
        ch = s[i];

        while (stack_prec(peep(stack, head)) > input_prec(ch))
            t[j++]=pop(stack, &head);

        if (input_prec(ch) > stack_prec(peep(stack, head)))
            push(stack, &head, ch);

        else pop(stack, &head);
        i++;
    }

    while (peep(stack, head) != '#')
        t[j++]=pop(stack, &head);

    t[j]='\0';
}
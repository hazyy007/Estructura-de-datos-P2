#include <stdio.h>
#include <stdlib.h>
#include "radio.h"
#include "stack.h"

Status mergeStack(Stack *s1, Stack *s2, Stack *sout)
{
    void *e1 = NULL, *e2 = NULL;
    Stack *ps = NULL;

    if (!s1 || !s2 || !sout)
    {
        return ERROR;
    }

    /*Mientras qeu ambas pilas tengan elementos*/
    while (!stack_isEmpty(s1) || !stack_isEmpty(s2))
    {
        e1 = stack_top(s1);
        e2 = stack_top(s2);

        if (music_getDuration((Music*)e1) > music_getDuration((Music*)e2))
        {
            stack_push(sout, stack_pop(s1));
        }
        else
        {
            stack_push(sout, stack_pop(s2));
        }
    }

    ps = stack_isEmpty(s1) ? s2 : s1;
    while (!stack_isEmpty(ps))
    {
        stack_push(sout, stack_pop(ps));
    }
    return OK;
}

int main()
{
    Radio *r1 = NULL, *r2 = NULL;
    Stack *s1 = NULL, *s2 = NULL, *sout = NULL;

    fopen("playlist1", "r");
    
}
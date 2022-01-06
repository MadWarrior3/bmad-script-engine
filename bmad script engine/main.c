// project headers
#include "common.h"
#include "variables.h"


// this is just a test program, don't look at this uncommented spaghetti too much
int main(int argc, char** argv)
{
    var_initHashTable(2);

    char sel;
    char sel1;
    char name[255] = "";
    VAR_TYPE type = t_empty;
    VAR_VALUE value = { .t_empty = NULL };

    VAR* tmpVar;

    do
    {
        printf("\n\n[a] Create variable\n"
               "[z] Read variable\n"
               "[e] Change variable value\n"
               "[q] Quit\n"
               ">");
        scanf(" %c", &sel);

        switch (sel)
        {
        case 'a':
            printf("\nname :\n");
            scanf("%s", name);

            printf("\ntype :\n"
                   "[s] String\n"
                   "[anything else] Int"
                   "\n>");
            scanf(" %c", &sel1);

            if (sel1 == 's')
            {
                type = t_str;

                printf("\nvalue : ");
                value.t_str.data = malloc(sizeof(char) * 1024);
                scanf(" %[^\t\n]", value.t_str.data);

                int i = 0;
                for (; value.t_str.data[i]; i++);
                value.t_str.length = i;
            }
            else
            {
                type = t_int;

                printf("\nvalue :\n");
                scanf("%lld", &value.t_int);
            }

            var_declareVariable(name, type, value, NULL);
            break;

        case 'z':
            printf("\nName : ");
            scanf("%s", name);

            tmpVar = var_getVariable(name);
            if (tmpVar == NULL)
            {
                printf("\n%s doesn't exist !", name);
                break;
            }
            
            if (tmpVar->type == t_str)
            {
                printf("type = string\nlength = %d\nvalue = %s\n", tmpVar->value.t_str.length, tmpVar->value.t_str.data);
            }
            else
            {
                printf("type = int\nvalue = %lld\n", tmpVar->value.t_int);
            }
            break;

        case 'e':
            printf("\nName : ");
            scanf("%s", name);

            tmpVar = var_getVariable(name);
            if (tmpVar == NULL)
            {
                printf("\n%s doesn't exist !", name);
                break;
            }
            else
            {
                if (tmpVar->type == t_str)
                {
                    printf("\ntype : string\nprevious value : %s\nnew value : ", tmpVar->value.t_str.data);
                    value.t_str.data = malloc(sizeof(char) * 1024);
                    scanf(" %[^\t\n]", value.t_str.data);

                    int i = 0;
                    for (; value.t_str.data[i]; i++);
                    value.t_str.length = i;
                }
                else
                {
                    printf("\ntype : int\nprevious value : %lld\nnew value : ", tmpVar->value.t_int);
                    scanf("%lld", &value.t_int);
                }

                VAR tmpVar2 = { .value = value, .alternateValue = NULL };
                tmpVar2.type = tmpVar->type;

                var_setVariable(tmpVar, &tmpVar2);
            }
            break;
        }

    } while (sel != 'q');

    return PROGRAM_RETURN_EVERYTHING_OK;
}
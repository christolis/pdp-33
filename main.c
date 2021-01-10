/* USER: christolis 
 * LANG: C
 * TASK: bankacc */

#include <stdio.h>
#include <stdlib.h>

#define MAX_CASH 1000000000
#define MAX_LINE_LENGTH 30

typedef struct account 
{
    struct account *left;
    struct account *right;
    struct account *parent;

    int id;
    int cash;
} account;

/*
 *  Initializes an account with the specified ID and an
 * initial amount of cash.
 *
 *  This function is used to initialize any type of node,
 * be it a root node or not.
 */
account *acc_init(int id, int cash, account *parent)
{
    account *acc = (account*) malloc(sizeof(account));

    acc->parent = parent;
    acc->left = NULL;
    acc->right = NULL;

    acc->id = id;
    acc->cash = cash;
    return acc;
}

/*
 *  Traverses the accounts binary tree to find any account
 * that matches the inputted ID.
 *
 *  If the function finds no such account, it creates one with
 * an initial cash amount of 0.
 */
account *acc_search(account *root_acc, int id)
{
    if (!root_acc)
    {
        root_acc = acc_init(id, 0, 0);
        return root_acc;
    }
    account *walker = root_acc;

    if (id == walker->id)
        return walker;

    if (id > walker->id)
    {
        if (!walker->right)
        {
            walker->right = acc_init(id, 0, walker);
            walker->right->parent = walker;
            return walker->right;
        }
        else
            return acc_search(walker->right, id);
    }
    else
    {
        if (!walker->left)
        {
            walker->left = acc_init(id, 0, walker);
            walker->left->parent = walker;
            return walker->left;
        }
        else
            return acc_search(walker->left, id);
    }
}

/*
 *  Searches for and adds (subtraction can work as well) cash to the
 * specificed account.
 *
 * NOTE:
 *  If the amount inputted is not valid (e.g not enough cash for
 * withdrawal), the function returns 0. If the account is found,
 * the amount is added to (or subtracted from) the account.
 */
int acc_add_cash(account *root_acc, int id, int amount)
{
    account *acc = acc_search(root_acc, id);
    int new_amount = acc->cash + amount;

    if (new_amount < 0 || new_amount > MAX_CASH)
        return 0;

    acc->cash += amount;
    return 1;
}

/*
 * Deletes an account and all of its descendants.
 *
 * This is used to free the memory when we are 
 * done with the data processing.
 */
void acc_delete(account *root_acc)
{
    if (root_acc)
    {
        acc_delete(root_acc->left);
        acc_delete(root_acc->right);
        free(root_acc);
    }
}

int main()
{
    /* root account has ID of -1 */
    account *root_acc = acc_init(-1, 0, 0);
    FILE *fpIn = fopen("bankacc.in", "r");
    FILE *fpOut = fopen("bankacc.out", "w");
    char line[MAX_LINE_LENGTH + 1];
    int n;

    /* Read the number of transactions (Number N) */
    fscanf(fpIn, "%d\n", &n); 

    if (n < 1000000)
    {
        for (int i = 0; i < n; i++)
        {
            fgets(line, MAX_LINE_LENGTH, fpIn);
            switch (*line)
            {
                /* In the case it asks for the account's balance */
                case 'q':
                {
                    unsigned int acc_id;

                    sscanf(line, "q %d", &acc_id);
                    account *acc = acc_search(root_acc, acc_id);

                    fprintf(fpOut, "%d\n", acc->cash);
                    break;
                }

                /* If it doesn't ask for the account's balance, then
                 * it should either ask for a deposit or withdrawal
                 * */
                default:
                {
                    char cmd;
                    unsigned int acc_id, amount;

                    sscanf(line, "%c %d %d", &cmd, &acc_id, &amount);
                    amount = (cmd == 'w') ? -amount : amount; /* If withdrawing cash, then we subtract*/

                    if (acc_add_cash(root_acc, acc_id, amount))
                        fputs("s\n", fpOut);
                    else
                        fputs("f\n", fpOut);

                    break;
                }
            }
        }
    }

    fclose(fpIn);
    fclose(fpOut);
    return 0;
}

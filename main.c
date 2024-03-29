/* USER: 33u79 
 * LANG: C
 * TASK: bankacc */

#include <stdio.h>
#include <stdlib.h>

#define MAX_CASH 1000000000
#define MAX_TRANSACTIONS 1000000
#define MAX_LINE_LENGTH 30

typedef struct account 
{
    struct account *left;
    struct account *right;

    int id;
    unsigned int cash;
} account;

/*
 *  Initializes an account with the specified ID and an
 * initial amount of cash.
 *
 *  This function is used to initialize any type of node,
 * be it a root node or not.
 */
account *acc_init(int id, int cash)
{
    account *acc = (account*) malloc(sizeof(account));

    acc->left = NULL;
    acc->right = NULL;

    acc->id = id;
    acc->cash = cash;
    return acc;
}

/*
 *  Iteratively searches through the accounts binary tree to
 * find any account that matches the inputted ID.
 *
 *  If the function finds no such account, it creates one with
 * an initial cash amount of 0.
 */
account *acc_search(account *root_acc, int id)
{
    if (!root_acc)
    {
        root_acc = acc_init(id, 0);
        return root_acc;
    }
    account *walker = root_acc;

    while (walker && id != walker->id)
    {
        if (id > walker->id)
        {
            if (!walker->right)
                walker->right = acc_init(id, 0);

            walker = walker->right;
        }
        else
        {
            if (!walker->left)
                walker->left = acc_init(id, 0);

            walker = walker->left;
        }
    }
    return walker;
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
    return new_amount;
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

int main(int argc, char *argv[])
{
    FILE *fpIn = fopen((argc > 1 ? argv[1] : "bankacc.in"), "r");
    FILE *fpOut = fopen("bankacc.out", "w");

    if (!fpIn)
    {
        printf("ΣΦΑΛΜΑ: Το αρχείο εισόδου \"%s\" δεν βρέθηκε.\n", argv[1]);
        exit(1);
    }

    if (!fpOut)
    {
        printf("ΣΦΑΛΜΑ: Το αρχείο εξόδου δεν μπόρεσε να δημιουργηθεί.\n");
        exit(1);
    }

    /* Root account has ID of -1 */
    account *root_acc = acc_init(-1, 0);
    char line[MAX_LINE_LENGTH + 1];
    int n;

    /* Read the number of transactions (Number N) */
    fscanf(fpIn, "%d\n", &n); 

    if (n <= MAX_TRANSACTIONS)
    {
        for (int i = 0; i < n; i++)
        {
            if (!(fgets(line, MAX_LINE_LENGTH, fpIn)))
                break;
            else
            {
                switch (*line)
                {
                    /* In the case it asks for the account's balance */
                    case 'q':
                    {
                        unsigned int acc_id;

                        sscanf(line, "q %u", &acc_id);
                        account *acc = acc_search(root_acc, acc_id);

                        fprintf(fpOut, "%u\n", acc->cash);
                        break;
                    }

                    /* If it doesn't ask for the account's balance, then
                     * it should either ask for a deposit or withdrawal
                     */
                    case 'd':
                    case 'w':
                    {
                        char cmd, res;
                        unsigned int acc_id, amount;

                        sscanf(line, "%c %u %u", &cmd, &acc_id, &amount);
                        amount = (cmd == 'w') ? -amount : amount; /* If withdrawing cash, then we subtract*/
                        res = acc_add_cash(root_acc, acc_id, amount) ? 's' : 'f';

                        fprintf(fpOut, "%c\n", res);
                        break;
                    }
                }
            }
        }
    }

    /* Delete all of the accounts. */
    acc_delete(root_acc);

    fclose(fpIn);
    fclose(fpOut);
    return 0;
}

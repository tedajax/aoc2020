#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef enum expr_op {
    expr_op_none,
    expr_op_value,
    expr_op_add,
    expr_op_mul,
} expr_op;

typedef struct expr_node {
    expr_op operation;
    long long value;
    struct expr_node* parent;
    struct expr_node* left;
    struct expr_node* right;
} expr_node;

typedef enum token_type {
    token_type_none,
    token_type_int,
    token_type_add,
    token_type_mul,
    token_type_open,
    token_type_close,
} token_type;

typedef struct token {
    token_type type;
    long long value;
} token;

expr_node node_pool[256];
int node_pool_id = 0;

void reset_pool()
{
    memset(node_pool, 0, sizeof(node_pool));
    node_pool_id = 0;
}

expr_node* alloc_node()
{
    return &node_pool[node_pool_id++];
}

expr_node* node_set_value(expr_node* node, long long value)
{
    if (!node) {
        node = alloc_node();
    }
    node->operation = expr_op_value;
    node->value = value;
    return node;
}

expr_node* node_set_left(expr_node* node, expr_node* left)
{
    if (!left) {
        left = alloc_node();
    }
    left->parent = node;
    node->left = left;
    return left;
}

expr_node* node_set_right(expr_node* node, expr_node* right)
{
    if (!right) {
        right = alloc_node();
    }
    right->parent = node;
    node->right = right;
    return right;
}

void node_set_add(expr_node* node)
{
    node->operation = expr_op_add;
}

void node_set_mul(expr_node* node)
{
    node->operation = expr_op_mul;
}

int isvalue(expr_op op)
{
    return op == expr_op_value;
}

int isop(expr_op op)
{
    return op >= expr_op_add && op <= expr_op_mul;
}

int is_node_empty(expr_node* node)
{
    return node == NULL || node->operation == expr_op_none;
}

long long eval_tree(expr_node* root)
{
    if (!root) {
        return 0;
    }

    switch (root->operation) {
    case expr_op_none:
        if (root->left) {
            return eval_tree(root->left);
        } else if (root->right) {
            return eval_tree(root->right);
        }
        return 0;
        break;
    case expr_op_value:
        return root->value;
    case expr_op_add:
        return eval_tree(root->left) + eval_tree(root->right);
    case expr_op_mul:
        return eval_tree(root->left) * eval_tree(root->right);
    default:
        return 0;
    }
}

void node_set_child_to(expr_node* node, expr_node* target, expr_node* new)
{
    if (node) {
        if (node->left == target) {
            node->left = new;
            new->parent = node;
        }
        if (node->right == target) {
            node->right = new;
            new->parent = node;
        }
    }
}

#define ERROR(expr)                                                                                \
    printf("ERROR %d!  %s\n", __LINE__, expr);                                                     \
    return 0;

token* get_tokens(const char* expr)
{
    token* tokens = NULL;
    int head = 0;
    int len = (int)strlen(expr);
    while (expr[head]) {
        int tail = head;
        while (tail < len && isdigit(expr[tail])) {
            ++tail;
        }
        if (tail > head) {
            long long val = strtoll(expr + head, NULL, 10);
            arrput(tokens, ((token){.type = token_type_int, .value = val}));
            head = tail;
            continue;
        } else if (expr[head] == '+') {
            arrput(tokens, ((token){.type = token_type_add}));
        } else if (expr[head] == '*') {
            arrput(tokens, ((token){.type = token_type_mul}));
        } else if (expr[head] == '(') {
            arrput(tokens, ((token){.type = token_type_open}));
        } else if (expr[head] == ')') {
            arrput(tokens, ((token){.type = token_type_close}));
        } else {
            // ignore
        }

        ++head;
    }

    return tokens;
}

int op_prec(token t)
{
    if (t.type == token_type_add) {
        return 2;
    } else if (t.type == token_type_mul) {
        return 1;
    } else {
        return 0;
    }
}

void print_tokens(token* tokens)
{
    for (int i = 0; i < arrlen(tokens); ++i) {
        token t = tokens[i];

        if (t.type == token_type_int) {
            printf("lld", t.value);
        } else if (t.type == token_type_mul) {
            printf("*");
        } else if (t.type == token_type_add) {
            printf("+");
        } else if (t.type == token_type_open) {
            printf("(");
        } else if (t.type == token_type_close) {
            printf(")");
        } else {
            printf("ERR");
        }
        printf(" ");
    }
    printf("\n");
}

token* convert_to_rpn(token* tokens)
{
    token* queue = NULL;
    token* stack = NULL;

    for (int i = 0; i < arrlen(tokens); ++i) {
        token t = tokens[i];

        switch (t.type) {
        case token_type_int:
            arrput(queue, t);
            break;
        case token_type_add:
        case token_type_mul:
            while (arrlen(stack) > 0) {
                token tt = arrpop(stack);
                int prec1 = op_prec(t);
                int prec2 = op_prec(tt);
                if (prec2 > 0 && prec1 <= prec2) {
                    arrput(queue, tt);
                } else {
                    arrput(stack, tt);
                    break;
                }
            }
            arrput(stack, t);
            break;
        case token_type_open:
            arrput(stack, t);
            break;
        case token_type_close:
            while (arrlen(stack) > 0) {
                token tt = arrpop(stack);
                if (tt.type != token_type_open) {
                    arrput(queue, tt);
                } else {
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    while (arrlen(stack) > 0) {
        arrput(queue, arrpop(stack));
    }

    arrfree(stack);

    return queue;
}

expr_node* build_tree(token* rpn_tokens)
{
    expr_node** stack = NULL;

    for (int i = 0; i < arrlen(rpn_tokens); ++i) {
        token t = rpn_tokens[i];
        switch (t.type) {
        case token_type_int: {
            expr_node* new = alloc_node();
            node_set_value(new, t.value);
            arrput(stack, new);
        } break;
        case token_type_add:
        case token_type_mul: {
            expr_node* right = (arrlen(stack) > 0) ? arrpop(stack) : NULL;
            expr_node* left = (arrlen(stack) > 0) ? arrpop(stack) : NULL;
            expr_node* new = alloc_node();
            new->operation = (t.type == token_type_add) ? expr_op_add : expr_op_mul;
            new->left = left;
            new->right = right;
            arrput(stack, new);
        } break;
        default:
            break;
        }
    }

    return stack[0];
}

long long eval_expr2(const char* expr)
{
    reset_pool();

    token* tokens = get_tokens(expr);
    token* rpn = convert_to_rpn(tokens);
    expr_node* node = build_tree(rpn);
    long long ret = eval_tree(node);
    arrfree(tokens);
    arrfree(rpn);
    return ret;
}

long long eval_expr(const char* expr)
{
    reset_pool();

    expr_node* curr = alloc_node();

    int head = 0;
    int len = (int)strlen(expr);
    int token = 0;
    while (expr[head]) {
        int tail = head;
        while (tail < len && isdigit(expr[tail])) {
            ++tail;
        }
        if (tail > head) {
            long long val = strtoll(expr + head, NULL, 10);

            if (curr->operation == expr_op_none) {
                node_set_value(curr, val);
            } else if (isop(curr->operation)) {
                if (is_node_empty(curr->left)) {
                    node_set_left(curr, node_set_value(NULL, val));
                } else if (is_node_empty(curr->right)) {
                    node_set_right(curr, node_set_value(NULL, val));
                } else {
                    ERROR(expr);
                }
            }

            head = tail;
            ++token;
            continue;
        } else if (expr[head] == '+') {
            if (isvalue(curr->operation) || isop(curr->operation)) {
                expr_node* new = alloc_node();
                node_set_add(new);
                node_set_child_to(curr->parent, curr, new);
                node_set_left(new, curr);
                curr = new;
            } else if (is_node_empty(curr)) {
                node_set_add(curr);
            } else {
                ERROR(expr);
            }
            ++token;
        } else if (expr[head] == '*') {
            if (isvalue(curr->operation) || isop(curr->operation)) {
                expr_node* new = alloc_node();
                node_set_mul(new);
                node_set_child_to(curr->parent, curr, new);
                node_set_left(new, curr);
                curr = new;
            } else if (is_node_empty(curr)) {
                node_set_mul(curr);
            } else {
                ERROR(expr);
            }
            ++token;
        } else if (expr[head] == '(') {
            if (isop(curr->operation)) {
                if (is_node_empty(curr->right)) {
                    curr = node_set_right(curr, NULL);
                } else {
                    ERROR(expr);
                }
            } else if (is_node_empty(curr)) {
                curr = node_set_left(curr, NULL);
            } else {
                ERROR(expr);
            }
            ++token;
        } else if (expr[head] == ')') {
            if (curr->parent) {
                curr = curr->parent;
            } else {
                ERROR(expr);
            }
            ++token;
        } else {
            // ignore
        }

        ++head;
    }

    while (curr->parent) {
        curr = curr->parent;
    }

    return eval_tree(curr);
}

int main(int argc, char* argv[])
{
    FILE* file = fopen("input_18.txt", "r");
    long long sum = 0;
    char exprbuf[1024];
    int index = 1;
    while (fgets(exprbuf, 1024, file)) {
        strtok(exprbuf, "\n");
        long long value = eval_expr2(exprbuf);
        sum += value;
        printf("%03d) %s = %lld : %lld\n", index++, exprbuf, value, sum);
    }

    fclose(file);

    const char* tests[] = {
        // "3*2+1", "3+(2*2)",
        // "1 + (2 * 3) + (4 * (5 + 6))",
        // "2 * 3 + (4 * 5)",
        // "5 + (8 * 3 + 9 + 3 * 4 * 3)",
        // "5*9*(7*3*3+9*3+(8+6*4))",
        // "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2",
        // "((2+4*9)*(6+9*8+6)+6)+2+4*2",
    };

    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        printf("%s = %lld\n", tests[i], eval_expr2(tests[i]));
    }

    printf("%lld\n", sum);
    getc(stdin);

    return 0;
}

// ((2+4*9)*(6+9*8+6)+6)+2+4*2
//    (54  *  210 + 6) + 2 + 4 * 2
//               216
//        11664 + 2 + 4 * 2

// 14357+264609*6*68796*2

// 14357+264609*6*68796*2
// 231475944024
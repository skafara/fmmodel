#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define STATES_CNT 8
#define INPUT_SYM_CNT 5
#define OUTPUT_SYM_CNT 6

#define INPUT_BUF_SIZE 64
#define INPUT_FORMAT "%s63s"

#define INPUT_SYM_END_FLAG -1
#define INPUT_SYM_NOT_EXISTS 0
#define INPUT_SYM_EXISTS 1

#define SYM_NAME_INDEX 0
#define SYM_DESC_INDEX 1

#define INPUT_SYM_PRINT_ARGS(SYM) "%-3s - %s\n",\
                                  input_sym_text[SYM][SYM_NAME_INDEX],\
                                  input_sym_text[SYM][SYM_DESC_INDEX]
#define OUTPUT_SYM_PRINT_ARGS(SYM) "%-3s - %s\n",\
                                   output_sym_text[SYM][SYM_NAME_INDEX],\
                                   output_sym_text[SYM][SYM_DESC_INDEX]

#define DIVIDER "----------------------------------------------------------------\n"


typedef enum state_t_ {
    D0, D1, D2, D3, D4, D5, D6, D7
} state_t;

typedef enum input_sym_t_ {
    I_BP, I_BS, I_TS, I_PC, I_TF5
} input_sym_t;

typedef enum output_sym_t_ {
    O_MO, O_MD, O_MU, O_L0, O_L1, O_TS5
} output_sym_t;

typedef enum output_action_t_ {
    A_MU, A_MD, A_L0, A_MUL1, A_MDL1, A_MOL0, A_MOTS5
} output_action_t;


const char *state_desc[STATES_CNT] = {
    "Vrata jsou zavrena, svetlo nesviti.",
    "Vrata se oteviraji, svetlo sviti.",
    "Vrata zastavena pri otevrani, svetlo nestviti.",
    "Vrata se zaviraji, svetlo sviti.",
    "Vrata zastavena pri zavirani, svetlo nesviti.",
    "Vrata jsou otevrena, svetlo nesviti.",
    "Vrata jsou zavrena, svetlo sviti.",
    "Vrata jsou otevrena, svetlo sviti."
};

const char input_sym_end[] = "KONEC";

const char *input_sym_text[INPUT_SYM_CNT][2] = {
    {"BP", "Tlacitko stisknuto / Ovladac stisknut."},
    {"BS", "Dolni spinac sepnut."},
    {"TS", "Horni spinac sepnut."},
    {"PC", "Paprsek fotobunky prerusen."},
    {"TF5", "Casovac peti minut dobehl."}
};

const char *output_sym_text[OUTPUT_SYM_CNT][2] = {
    {"MO", "Vypni motor ovladani vrat."},
    {"MD", "Pohybuj vraty dolu."},
    {"MU", "Pohybuj vraty nahoru."},
    {"L0", "Zhasni svetlo."},
    {"L1", "Rozsvit svetlo."},
    {"TS5", "Resetuj casovac a spust casovani peti minut."}
};


const state_t transition_tab[STATES_CNT][INPUT_SYM_CNT] = {
    {D1, -1, -1, -1, -1},
    {D2, -1, D7, -1, -1},
    {D3, -1, -1, -1, -1},
    {D4, D6, -1, D1, -1},
    {D1, -1, -1, -1, -1},
    {D3, -1, -1, -1, -1},
    {D1, -1, -1, -1, D0},
    {D3, -1, -1, -1, D5}
};

const output_action_t output_action_tab[STATES_CNT][INPUT_SYM_CNT] = {
    {A_MUL1,  -1,     -1,     -1,     -1  },
    {A_MOL0,  -1,     A_MOTS5,-1,     -1  },
    {A_MDL1,  -1,     -1,     -1,     -1  },
    {A_MOL0,  A_MOTS5,-1,     A_MU,   -1  },
    {A_MUL1,  -1,     -1,     -1,     -1  },
    {A_MDL1,  -1,     -1,     -1,     -1  },
    {A_MU,    -1,     -1,     -1,     A_L0},
    {A_MD,    -1,     -1,     -1,     A_L0}
};


state_t state = D0;

input_sym_t input_sym;


int input_symbol() {
    char input_buf[INPUT_BUF_SIZE];
    int sym_exists;
    size_t s;

    scanf(INPUT_FORMAT, input_buf);
    if (strcmp(input_buf, input_sym_end) == 0) {
        return INPUT_SYM_END_FLAG;
    }
    sym_exists = INPUT_SYM_NOT_EXISTS;
    for (s = 0; s < INPUT_SYM_CNT; s++) {
        if (strcmp(input_buf, input_sym_text[s][SYM_NAME_INDEX]) == 0) {
            input_sym = (input_sym_t) s;
            sym_exists = INPUT_SYM_EXISTS;
            break;
        }
    }

    return sym_exists;
}


void output_action(output_action_t action) {
    switch (action) {
        case A_MU:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MU));
            break;
        case A_MD:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MD));
            break;
        case A_L0:
            printf(OUTPUT_SYM_PRINT_ARGS(O_L0));
            break;
        case A_MUL1:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MU));
            printf(OUTPUT_SYM_PRINT_ARGS(O_L1));
            break;
        case A_MDL1:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MD));
            printf(OUTPUT_SYM_PRINT_ARGS(O_L1));
            break;
        case A_MOL0:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MO));
            printf(OUTPUT_SYM_PRINT_ARGS(O_L0));
            break;
        case A_MOTS5:
            printf(OUTPUT_SYM_PRINT_ARGS(O_MO));
            printf(OUTPUT_SYM_PRINT_ARGS(O_TS5));
            break;
    }
}


void print_intro() {
    size_t s;

    printf(DIVIDER);
    printf("Konecne-automatovy model: Ovladani garazovych vrat\n");
    printf(DIVIDER);
    printf("Pro ukonceni programu zadejte vstupni symbol: %s.\n", input_sym_end);
    printf("Vstupni symboly automatu:\n");
    printf(DIVIDER);
    for (s = 0; s < INPUT_SYM_CNT; s++) {
        printf(INPUT_SYM_PRINT_ARGS(s));
    }
    printf(DIVIDER);
}


void print_state_ask_sym() {
    printf(DIVIDER);
    printf("Stav: D%d - %s\n", state, state_desc[state]);
    printf("Zadejte vstupni symbol: ");
}


void print_end_program() {
    printf(DIVIDER);
    printf("Konec programu.\n");
    printf(DIVIDER);
}


void loop() {
    int input_sym_flag;

    while (1) {
        print_state_ask_sym();

        input_sym_flag = input_symbol();
        if (input_sym_flag == INPUT_SYM_END_FLAG) {
            print_end_program();
            break;
        }
        else if (input_sym_flag == INPUT_SYM_NOT_EXISTS) {
            printf("Zadali jste neexistujici vstupni symbol.\n");
            continue;
        }
        else if (transition_tab[state][input_sym] == (state_t) -1) {
            printf("Vstupni symbol nema pro aktualni stav vyznam.\n");
            continue;
        }
        else {
            printf("Byly vyslany nasledujici ridici signaly:\n");
            printf(DIVIDER);

            output_action(output_action_tab[state][input_sym]);

            printf(DIVIDER);

            state = transition_tab[state][input_sym];
        }
    }
}


int main() {
    print_intro();
    loop();
    return EXIT_SUCCESS;
}

/***************************************************************************************[minisat.h]
Copyright (c) 2008-2011, Niklas Sorensson
              2008, Koen Claessen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Minisat_C_Bindings_h
#define Minisat_C_Bindings_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct CMiniSat CMiniSat;
typedef int minisat_Var;
typedef int minisat_Lit;
typedef int minisat_lbool;

extern const minisat_lbool minisat_l_True;
extern const minisat_lbool minisat_l_False;
extern const minisat_lbool minisat_l_Undef;

minisat_lbool minisat_get_l_True(void);
minisat_lbool minisat_get_l_False(void);
minisat_lbool minisat_get_l_Undef(void);

const char *minisat_signature(void);
CMiniSat *minisat_init(void);
void minisat_release(CMiniSat *s);

minisat_Var minisat_newVar(CMiniSat *s);
minisat_Lit minisat_newLit(CMiniSat *s);

minisat_Lit minisat_mkLit(minisat_Var x);
minisat_Lit minisat_mkLit_args(minisat_Var x, int sign);
minisat_Lit minisat_negate(minisat_Lit p);
minisat_Var minisat_var(minisat_Lit p);
bool minisat_sign(minisat_Lit p);

void minisat_setPolarity(CMiniSat *s, minisat_Var v, minisat_lbool lb);
void minisat_setDecisionVar(CMiniSat *s, minisat_Var v, bool b);

void minisat_setFrozen(CMiniSat *s, minisat_Var v, bool b);
bool minisat_isEliminated(CMiniSat *s, minisat_Var v);
bool minisat_eliminate(CMiniSat *s, bool turn_off_elim);

bool minisat_addClause(CMiniSat *s, int len, minisat_Lit *ps);
void minisat_addClause_begin(CMiniSat *s);
void minisat_addClause_addLit(CMiniSat *s, minisat_Lit p);
bool minisat_addClause_commit(CMiniSat *s);

bool minisat_simplify(CMiniSat *s);

bool minisat_solve(CMiniSat *s, int len, minisat_Lit *ps);
minisat_lbool minisat_limited_solve(CMiniSat *s, int len, minisat_Lit *ps);
void minisat_solve_begin(CMiniSat *s);
void minisat_solve_addLit(CMiniSat *s, minisat_Lit p);
bool minisat_solve_commit(CMiniSat *s);
minisat_lbool minisat_limited_solve_commit(CMiniSat *s);

bool minisat_okay(CMiniSat *s);

minisat_lbool minisat_value_Var(CMiniSat *s, minisat_Var x);
minisat_lbool minisat_value_Lit(CMiniSat *s, minisat_Lit p);
minisat_lbool minisat_modelValue_Var(CMiniSat *s, minisat_Var x);
minisat_lbool minisat_modelValue_Lit(CMiniSat *s, minisat_Lit p);

int minisat_conflict_len(CMiniSat *s);
minisat_Lit minisat_conflict_nthLit(CMiniSat *s, int i);

void minisat_set_conf_budget(CMiniSat *s, int x);
void minisat_set_prop_budget(CMiniSat *s, int x);
void minisat_no_budget(CMiniSat *s);

void minisat_interrupt(CMiniSat *s);
void minisat_clearInterrupt(CMiniSat *s);

void minisat_set_verbosity(CMiniSat *s, int v);
int minisat_get_verbosity(CMiniSat *s);
void minisat_set_random_var_freq(CMiniSat *s, double random_var_freq);
void minisat_set_random_seed(CMiniSat *s, double random_seed);

int minisat_num_assigns(CMiniSat *s);
int minisat_num_clauses(CMiniSat *s);
int minisat_num_learnts(CMiniSat *s);
int minisat_num_vars(CMiniSat *s);
int minisat_num_freeVars(CMiniSat *s);
int64_t minisat_num_conflicts(CMiniSat *s);
int64_t minisat_num_decisions(CMiniSat *s);
int64_t minisat_num_restarts(CMiniSat *s);
int64_t minisat_num_propagations(CMiniSat *s);

#ifdef __cplusplus
}
#endif

#endif

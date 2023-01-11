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

typedef struct CMinisat CMinisat;
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
CMinisat *minisat_init(void);
void minisat_release(CMinisat *s);

minisat_Var minisat_newVar(CMinisat *s);
minisat_Lit minisat_newLit(CMinisat *s);

minisat_Lit minisat_mkLit(minisat_Var x);
minisat_Lit minisat_mkLit_args(minisat_Var x, int sign);
minisat_Lit minisat_negate(minisat_Lit p);
minisat_Var minisat_var(minisat_Lit p);
bool minisat_sign(minisat_Lit p);

void minisat_setPolarity(CMinisat *s, minisat_Var v, minisat_lbool lb);
void minisat_setDecisionVar(CMinisat *s, minisat_Var v, bool b);

void minisat_setFrozen(CMinisat *s, minisat_Var v, bool b);
bool minisat_isEliminated(CMinisat *s, minisat_Var v);
bool minisat_eliminate(CMinisat *s, bool turn_off_elim);

bool minisat_addClause(CMinisat *s, int len, minisat_Lit *ps);
void minisat_addClause_begin(CMinisat *s);
void minisat_addClause_addLit(CMinisat *s, minisat_Lit p);
bool minisat_addClause_commit(CMinisat *s);

bool minisat_simplify(CMinisat *s);

bool minisat_solve(CMinisat *s, int len, minisat_Lit *ps);
minisat_lbool minisat_limited_solve(CMinisat *s, int len, minisat_Lit *ps);
void minisat_solve_begin(CMinisat *s);
void minisat_solve_addLit(CMinisat *s, minisat_Lit p);
bool minisat_solve_commit(CMinisat *s);
minisat_lbool minisat_limited_solve_commit(CMinisat *s);

bool minisat_okay(CMinisat *s);

minisat_lbool minisat_value_Var(CMinisat *s, minisat_Var x);
minisat_lbool minisat_value_Lit(CMinisat *s, minisat_Lit p);
minisat_lbool minisat_modelValue_Var(CMinisat *s, minisat_Var x);
minisat_lbool minisat_modelValue_Lit(CMinisat *s, minisat_Lit p);

int minisat_conflict_len(CMinisat *s);
minisat_Lit minisat_conflict_nthLit(CMinisat *s, int i);

void minisat_set_conf_budget(CMinisat *s, int x);
void minisat_set_prop_budget(CMinisat *s, int x);
void minisat_no_budget(CMinisat *s);

void minisat_interrupt(CMinisat *s);
void minisat_clearInterrupt(CMinisat *s);

void minisat_set_verbosity(CMinisat *s, int v);
int minisat_get_verbosity(CMinisat *s);
void minisat_set_random_var_freq(CMinisat *s, double random_var_freq);
void minisat_set_random_seed(CMinisat *s, double random_seed);

int minisat_num_assigns(CMinisat *s);
int minisat_num_clauses(CMinisat *s);
int minisat_num_learnts(CMinisat *s);
int minisat_num_vars(CMinisat *s);
int minisat_num_freeVars(CMinisat *s);
int64_t minisat_num_conflicts(CMinisat *s);
int64_t minisat_num_decisions(CMinisat *s);
int64_t minisat_num_restarts(CMinisat *s);
int64_t minisat_num_propagations(CMinisat *s);

#ifdef __cplusplus
}
#endif

#endif

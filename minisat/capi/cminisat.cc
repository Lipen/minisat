/**************************************************************************************[minisat.cc]
Copyright (c) 2008-2010, Niklas Sorensson
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

#include <stdlib.h>

#include "minisat/simp/SimpSolver.h"

using namespace Minisat;

struct CMiniSat : public SimpSolver {
    vec<Lit> clause;
    vec<Lit> assumps;
};

extern "C" {

#include "cminiSat.h"

// Note:
//  `minisat_Var` (=`int`) can be converted to `Minisat::Var` (=`int`) via `mkLit`
//  `minisat_Lit` (=`int`) can be converted to `Minisat::Lit` (`struct`) via `toLit`

// This implementation of lbool may or not may be an exact mirror of the C++ implementation:
//
extern const minisat_lbool minisat_l_True = 1;
extern const minisat_lbool minisat_l_False = 0;
extern const minisat_lbool minisat_l_Undef = -1;

static inline minisat_lbool toC(lbool a) {
    return a == l_True    ? minisat_l_True
           : a == l_False ? minisat_l_False
                          : minisat_l_Undef;
}

static inline lbool fromC(minisat_lbool a) {
    return a == minisat_l_True    ? l_True
           : a == minisat_l_False ? l_False
                                  : l_Undef;
}

minisat_lbool minisat_get_l_True(void) { return minisat_l_True; }
minisat_lbool minisat_get_l_False(void) { return minisat_l_False; }
minisat_lbool minisat_get_l_Undef(void) { return minisat_l_Undef; }

const char *minisat_signature(void) { return "minisat"; }
CMiniSat *minisat_init(void) { return new CMiniSat(); }
void minisat_release(CMiniSat *s) { delete s; }

minisat_Var minisat_newVar(CMiniSat *s) { return s->newVar(); }
minisat_Lit minisat_newLit(CMiniSat *s) { return toInt(mkLit(s->newVar())); }

minisat_Lit minisat_mkLit(minisat_Var x) { return toInt(mkLit(x)); }
minisat_Lit minisat_mkLit_args(minisat_Var x, int sign) { return toInt(mkLit(x, sign)); }
minisat_Lit minisat_negate(minisat_Lit p) { return toInt(~toLit(p)); }
minisat_Var minisat_var(minisat_Lit p) { return var(toLit(p)); }
bool minisat_sign(minisat_Lit p) { return sign(toLit(p)); }

void minisat_setPolarity(CMiniSat *s, minisat_Var v, minisat_lbool lb) { s->setPolarity(v, fromC(lb)); }
void minisat_setDecisionVar(CMiniSat *s, minisat_Var v, bool b) { s->setDecisionVar(v, b); }

void minisat_setFrozen(CMiniSat *s, minisat_Var v, bool b) { s->setFrozen(v, b); }
bool minisat_isEliminated(CMiniSat *s, minisat_Var v) { return s->isEliminated(v); }
bool minisat_eliminate(CMiniSat *s, bool turn_off_elim) { return s->eliminate(turn_off_elim); }

void minisat_addClause_begin(CMiniSat *s) { s->clause.clear(); }
void minisat_addClause_addLit(CMiniSat *s, minisat_Lit p) { s->clause.push(toLit(p)); }
bool minisat_addClause_commit(CMiniSat *s) { return s->addClause_(s->clause); }

bool minisat_simplify(CMiniSat *s) { return s->simplify(); }

// NOTE: Currently these run with default settings for implicitly calling preprocessing. Turn off
// before if you don't need it. This may change in the future.
void minisat_solve_begin(CMiniSat *s) { s->assumps.clear(); }
void minisat_solve_addLit(CMiniSat *s, minisat_Lit p) { s->assumps.push(toLit(p)); }
bool minisat_solve_commit(CMiniSat *s) { return s->solve(s->assumps); }
minisat_lbool minisat_limited_solve_commit(CMiniSat *s) { return toC(s->solveLimited(s->assumps)); }

bool minisat_okay(CMiniSat *s) { return s->okay(); }

minisat_lbool minisat_value_Var(CMiniSat *s, minisat_Var x) { return toC(s->value(x)); }
minisat_lbool minisat_value_Lit(CMiniSat *s, minisat_Lit p) { return toC(s->value(toLit(p))); }
minisat_lbool minisat_modelValue_Var(CMiniSat *s, minisat_Var x) { return toC(s->modelValue(x)); }
minisat_lbool minisat_modelValue_Lit(CMiniSat *s, minisat_Lit p) { return toC(s->modelValue(toLit(p))); }

int minisat_conflict_len(CMiniSat *s) { return s->conflict.size(); }
minisat_Lit minisat_conflict_nthLit(CMiniSat *s, int i) { return toInt(s->conflict[i]); }

void minisat_set_conf_budget(CMiniSat *s, int x) { s->setConfBudget(x); }
void minisat_set_prop_budget(CMiniSat *s, int x) { s->setPropBudget(x); }
void minisat_no_budget(CMiniSat *s) { s->budgetOff(); }

void minisat_interrupt(CMiniSat *s) { s->interrupt(); }
void minisat_clearInterrupt(CMiniSat *s) { s->clearInterrupt(); }

void minisat_set_verbosity(CMiniSat *s, int v) { s->verbosity = v; }
int minisat_get_verbosity(CMiniSat *s) { return s->verbosity; }
void minisat_set_random_var_freq(CMiniSat *s, double random_var_freq) { s->random_var_freq = random_var_freq; }
void minisat_set_random_seed(CMiniSat *s, double random_seed) { s->random_seed = random_seed; }

int minisat_num_assigns(CMiniSat *s) { return s->nAssigns(); }
int minisat_num_clauses(CMiniSat *s) { return s->nClauses(); }
int minisat_num_learnts(CMiniSat *s) { return s->nLearnts(); }
int minisat_num_vars(CMiniSat *s) { return s->nVars(); }
int minisat_num_freeVars(CMiniSat *s) { return s->nFreeVars(); }
int64_t minisat_num_conflicts(CMiniSat *s) { return (int64_t)s->conflicts; }
int64_t minisat_num_decisions(CMiniSat *s) { return (int64_t)s->decisions; }
int64_t minisat_num_restarts(CMiniSat *s) { return (int64_t)s->starts; }
int64_t minisat_num_propagations(CMiniSat *s) { return (int64_t)s->propagations; }

// Convenience functions for actual c-programmers (not language interfacing people):
//
bool minisat_solve(CMiniSat *s, int len, minisat_Lit *ps) {
    minisat_solve_begin(s);
    for (int i = 0; i < len; i++)
        minisat_solve_addLit(s, ps[i]);
    return minisat_solve_commit(s);
}

minisat_lbool minisat_limited_solve(CMiniSat *s, int len, minisat_Lit *ps) {
    minisat_solve_begin(s);
    for (int i = 0; i < len; i++)
        minisat_solve_addLit(s, ps[i]);
    return minisat_limited_solve_commit(s);
}

bool minisat_addClause(CMiniSat *s, int len, minisat_Lit *ps) {
    minisat_addClause_begin(s);
    for (int i = 0; i < len; i++)
        minisat_addClause_addLit(s, ps[i]);
    return minisat_addClause_commit(s);
}
}

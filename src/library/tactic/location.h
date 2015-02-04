/*
Copyright (c) 2015 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "library/kernel_serializer.h"

namespace lean {
/*
   We can specify the scope of application of some clauses using
   "locations". Locations specify goal and/or hypotheses and
   "occurrences" inside a given location. The occurrences are
   just numeric values and the specify which occurrences of a
   term instance are considered by the tactic.

   Examples:

   - rewrite H1 in H at (1, 3)

   says the scope of (rewrite H1) is the hypothesis H, and
   only the first and third occurrences of the H1 left-hand-side
   will be considered.

   - rewrite H1 in H at -1

   says the scope of (rewrite H1) is the hypothesis H and all
   but the first occurrence of the H1 left-hand-side
   will be considered.
*/

class occurrence {
    enum kind { All, Pos, Neg };
    kind           m_kind;
    list<unsigned> m_occs;
    occurrence(kind k, list<unsigned> const & occs):m_kind(k), m_occs(occs) {}
public:
    occurrence():m_kind(All) {}
    static occurrence mk_occurrences(buffer<unsigned> const & occs) { return occurrence(Pos, to_list(occs)); }
    static occurrence mk_occurrences(list<unsigned> const & occs) { return occurrence(Pos, occs); }
    static occurrence mk_except_occurrences(buffer<unsigned> const & occs) { return occurrence(Neg, to_list(occs)); }
    static occurrence mk_except_occurrences(list<unsigned> const & occs) { return occurrence(Neg, occs); }
    bool is_all() const { return m_kind == All; }
    bool is_except() const { return m_kind == Neg; }
    /** \brief Return true iff this occurrence object contains the given occurrence index. */
    unsigned contains(unsigned occ_idx) const;
    friend serializer & operator<<(serializer & s, occurrence const & o);
    friend deserializer & operator>>(deserializer & d, occurrence & o);
};

class location {
public:
    enum kind { Everywhere, GoalOnly, AllHypotheses, Hypotheses, GoalHypotheses };
private:
    kind                         m_kind;
    occurrence                   m_goal;
    list<pair<name, occurrence>> m_hyps;
    location(kind k, occurrence const & g = occurrence(),
             list<pair<name, occurrence>> const & hs = list<pair<name, occurrence>>()):
        m_kind(k), m_goal(g), m_hyps(hs) {}
public:
    location():m_kind(GoalOnly) {}

    static location mk_goal_only() { return location(); }
    static location mk_everywhere() { return location(Everywhere); }
    static location mk_all_hypotheses() { return location(AllHypotheses); }
    static location mk_hypotheses(buffer<name> const & hs);
    static location mk_goal_hypotheses(buffer<name> const & hs);
    static location mk_goal_at(occurrence const & o) { return location(GoalOnly, o); }
    static location mk_hypotheses_at(buffer<name> const & hs, buffer<occurrence> const & occs);
    static location mk_at(occurrence const & g_occs, buffer<name> const & hs, buffer<occurrence> const & hs_occs);

    bool is_goal_only() const { return m_kind == GoalOnly; }

    optional<occurrence> includes_goal() const;
    optional<occurrence> includes_hypothesis(name const & h) const;
    void get_explicit_hypotheses_names(buffer<name> & r) const;

    friend serializer & operator<<(serializer & s, location const & loc);
    friend deserializer & operator>>(deserializer & d, location & loc);
};
}

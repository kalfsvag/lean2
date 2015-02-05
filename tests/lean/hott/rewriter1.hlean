import algebra.group
open path_algebra

constant f {A : Type} : A → A → A

theorem test1 {A : Type} [s : add_comm_group A] (a b c : A) : f (a + 0) (f (a + 0) (a + 0)) = f a (f (0 + a) a) :=
begin
  rewrite [
    add_right_id at {1 3}, -- rewrite 1st and 3rd occurrences
    {0 + _}add_comm]   -- apply commutativity to (0 + _)
end

axiom Ax {A : Type} [s₁ : has_mul A] [s₂ : has_one A] (a : A) : f (a * 1) (a * 1) = 1

theorem test2 {A : Type} [s : comm_group A] (a b c : A) : f a a = 1 :=
begin
  rewrite [-(mul_right_id a),  -- - means apply symmetry, rewrite 0 ==> a * 0  at 1st and 2nd occurrences
           Ax]                 -- use Ax as rewrite rule
end
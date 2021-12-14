(define (domain blocksworld)
(:requirements :typing)
(:types block - object)
(:predicates (clear ?x)
             (on-table ?x)
             (on ?x ?y))

(:action move-b-to-b
  :parameters (?bm ?bf ?bt - block)
  :precondition (and (clear ?bm) (clear ?bt) (on ?bm ?bf))
  :effect (and (not (clear ?bt)) (not (on ?bm ?bf))
               (on ?bm ?bt) (clear ?bf)))

(:action move-b-to-t
  :parameters (?bm ?bf - block)
  :precondition (and (clear ?bm) (on ?bm ?bf))
  :effect (and (not (on ?bm ?bf))
               (on-table ?bm) (clear ?bf)))

(:action move-t-to-b
  :parameters (?bm ?bt - block)
  :precondition (and (clear ?bm) (clear ?bt) (on-table ?bm))
  :effect (and (not (clear ?bt)) (not (on-table ?bm))
               (on ?bm ?bt))))


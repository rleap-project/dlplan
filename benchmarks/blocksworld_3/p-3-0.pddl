

(define (problem BW-rand-3)
(:domain blocksworld)
(:objects b1 b2 b3  - block)
(:init
(on b1 b3)
(on b2 b1)
(on-table b3)
(clear b2)
)
(:goal
(and
(on b1 b2)
(on b3 b1))
)
)



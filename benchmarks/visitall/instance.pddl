(define (problem grid-2)
	(:domain grid-visit-all)
	(:objects x0-y0	x0-y1 x1-y0	x1-y1 - place 
			
	)
	(:init
		(at-robot x1-y1)
		(visited x1-y1)
		(connected x0-y0 x1-y0)
		(connected x0-y0 x0-y1)
		(connected x0-y1 x1-y1)
		(connected x0-y1 x0-y0)
		(connected x1-y0 x0-y0)
		(connected x1-y0 x1-y1)
		(connected x1-y1 x0-y1)
		(connected x1-y1 x1-y0)
	
	)
	(:goal (and (visited x1-y0)
		(visited x1-y1))
	)
)
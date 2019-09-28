-coordinate-
*npoin 2
*loop nodes
*format "%12.12e %12.12e"
*NodesCoord(1) *NodesCoord(2)
*end
*set var numfixnode=0
*Set Cond A-line-Constraints *nodes
*Add Cond A-point-Constraints *nodes
*loop nodes *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end

-mesh-
*Set var numfixnode=0
*set cond A-Surface-MaterialSN-q4g2 *elems
*loop elems *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end
q4
*numfixnode 4
*set cond A-Surface-MaterialSN-q4g2 *elems
*loop elems *OnlyIncond
*elemsConec
*end

-field-

-ID-
*set var numfixnode=0
*Set Cond A-line-Constraints *nodes
*Add Cond A-point-Constraints *nodes
*loop nodes *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end
*numfixnode 1
*Set Cond A-line-Constraints *nodes
*Add Cond A-point-Constraints *nodes
*loop nodes *OnlyInCond
*NodesNum *cond(1)
*end

-value-
*set var numfixnode=0
*Set Cond A-line-Constraints *nodes
*Add Cond A-point-Constraints *nodes
*loop nodes *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end
*numfixnode 1
*Set Cond A-line-Constraints *nodes
*Add Cond A-point-Constraints *nodes
*loop nodes *OnlyInCond
*NodesNum *cond(2)
*end

-init-
*set var numfixnode=0
*Set Cond A-surface-initial-data1 *nodes
*Add Cond A-line-initial-data1 *nodes
*Add Cond A-point-initial-data1 *nodes
*loop nodes *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end
*numfixnode 1
*Set Cond A-surface-initial-data1 *nodes
*Add Cond A-line-initial-data1 *nodes
*Add Cond A-point-initial-data1 *nodes
*loop nodes *OnlyInCond
*NodesNum *cond(1)
*end

-mate-
*Set var numfixnode=0
*set cond A-Surface-MaterialSN-q4g2 *elems
*loop elems *OnlyInCond
*set var numfixnode=operation(numfixnode+1)
*end
q4
*numfixnode
*set cond A-Surface-MaterialSN-q4g2 *elems
*loop elems *OnlyIncond
*ElemsNum *cond(1)
*end
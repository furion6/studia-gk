.global _start
_start:

!wzor = (n+1)*(n/2)


 mov 8, %l1 		! l1 = n
 mov 0, %l2 		! w l2 bedziemy miec wynik wzoru
 add %l1, 1, %l3	! l3 = n+1 uzysukejmy pierwsza czesc wzoru
 srl %l1, 1, %l4	! l4 = n/2 mamy druga czesc wzoru teraz musimy pomnozyc l3*l4
 mov %l3,%o0 		! oO = l3
 mov %l4,%o1 		! o1 = l4
 call mnozenie		! funkcja ktora mnozy o0 i o1
 nop
 mov %o0,%l2		! wynik z o0 do l2
 ret




mnozenie:
 mov 0,%l0 		! w l0 bedzie wynik
 mov %o1, %l1		! l1 = o1
petla:
 add %l0,%o0,%l0 	! mnozymy poprzez dodawanie
 subcc %o1,1,%o1 	! dekrementujemy licznik mnozen 
 be koniec        	! jesli licznik  < 0 to koniec funkcji
 nop
 ba petla       	! wroc na poczatek petli
 nop
koniec:
 mov %l0,%o0		! wynik do o0
 retl			! return
 nop 

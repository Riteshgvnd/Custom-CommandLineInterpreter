all : prog1 prog2
	
prog1:
		gcc getcmd.c -o MyShell  

prog2:		
		gcc cut2.c -o cut2 

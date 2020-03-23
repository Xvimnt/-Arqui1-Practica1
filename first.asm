.model small
.stack 100h

printStr macro aString
mov ah,9h
lea dx,aString
int 21h
endm

printChar macro var
mov ah,2h
mov dl, var
int 21h
mov ah,0
endm

readInt macro var
mov ah,1h
int 21h
mov var,al
endm

begin macro
mov ax, @data
mov ds,ax
endm

exit macro
mov   ax,4c00h; Function (Quit with exit code (EXIT))
int   21h; Interruption DOS Functions
endm

.data
   x db ?
   entring db 10,13, 'entrando a play proc'
   loading db 10,13, 'Cargando juego'
   playing db 10,13, 'Jugando juego$'
   invalidChoice db 10,13, 'Eleccion invalida: Numero del 1 al 3:', 13,10,'$'
   header db 10, 13, 'UNIVERSIDAD DE SAN CARLOS DE GUATEMALA', 13, 10 , 'FACULTAD DE INGENIERIA', 13, 10, "$"
   header2 db  'CIENCIAS Y SISTEMAS', 13, 10 , 'ARQUITECTURA DE COMPUTADORAS Y ENSAMBLADORES 1', 13, 10, "$"
   header3 db  'NOMBRE: JAVIER ALEJANDRO MONTERROSO', 13, 10 , 'CARNET: 201700831', 13, 10, 'SECCION: A', 13,10, "$"
   menu db 10,13, '1) Iniciar Juego', 10, 13, '2) Cargar juego', 10,13, '3) Salir', 10,13, '$'
.code
 
DISPLAY_MENU  proc
initialMenu:
   begin
   printStr header
   printStr header2
   printStr header3
   printStr menu

switchUserInput:
   readInt x; Lee un digito ingresado y lo carga a AL
   cmp x, '1'
   jl tryAgain
   cmp x, '3'
   jg tryAgain
   cmp x,'1'
   je PLAY
   cmp x,'2'
   je LOAD
   exit

tryAgain:
   printStr invalidChoice
   jmp switchUserInput

DISPLAY_MENU  endp;


LOAD proc
   printStr loading
   exit
LOAD endp


PLAY proc
   printStr entring
   printStr playing
   exit
PLAY endp

end DISPLAY_MENU
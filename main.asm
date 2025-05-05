section .text
global moveCharacter

moveCharacter:
    ; Guardar registros
    push rbp
    mov rbp, rsp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15
    
    ; Parametros de la funcion
    mov r12, rcx        ; r12 = puntero a x
    mov r13, rdx        ; r13 = puntero a y
    mov r14, r8         ; r14 = gameArea
    movzx esi, r9b      ; esi = input 
    mov r15, [rbp + 48] ; r15 = puntero a remainingX
    
    ; Cargar coordenadas actuales
    mov eax, [r12]      ; eax = x
    mov ebx, [r13]      ; ebx = y
    
    ; Tecla presionada
    cmp esi, 'w'
    je move_up
    cmp esi, 's'
    je move_down
    cmp esi, 'a'
    je move_left
    cmp esi, 'd'
    je move_right

    jmp end_function    ; tecla invalida (No wasd)
    
move_up:
    dec ebx             ; y = y - 1
    jmp check_movement
    
move_down:
    inc ebx             ; y = y + 1
    jmp check_movement
    
move_left:
    dec eax             ; x = x - 1
    jmp check_movement
    
move_right:
    inc eax             ; x = x + 1
    
check_movement:
    ; Calculo del nuevo indice en gameArea
    mov ecx, ebx        ; ecx = y
    imul ecx, 35        ; ecx = y * WIDTH (35)
    add ecx, eax        ; ecx = y*WIDTH + x
    
    ; Revisar el contenido de la nueva posicion
    mov dl, [r14 + rcx] ; Obtener el caracter de la nueva posicion
    
    cmp dl, ' '         ; Espacio vacio (podemos movernos)
    je update_position
    
    cmp dl, 'X'         ; X (necesitamos empujar)
    je check_push
    
    ; Si no es espacio vacio ni X, no nos podemos mover, es un limite (#)
    mov eax, [r12]
    mov ebx, [r13]
    jmp end_function
    
check_push:
    ; Direccion a empujar
    mov edx, ebx        ; edx = new y
    mov edi, eax        ; edi = new x
    
    cmp esi, 'w'
    je push_up
    cmp esi, 's'
    je push_down
    cmp esi, 'a'
    je push_left
    cmp esi, 'd'
    je push_right

    jmp end_function
    
push_up:
    dec edx             ; Revisar posicion arriba
    jmp check_push_space
    
push_down:
    inc edx             ; Revisar posicion abajo
    jmp check_push_space
    
push_left:
    dec edi             ; Revisar posicion izquierda
    jmp check_push_space
    
push_right:
    inc edi             ; Revisar posicion derecha
    
check_push_space:
    ; Indice para la posicion desplazada
    mov ecx, edx
    imul ecx, 35
    add ecx, edi
    
    ; Revisar si hay una 'P' en la nueva posicion
    cmp byte [r14 + rcx], 'P'
    je remove_x        ; Si es una 'P', eliminar la 'X'
    
    ; Hay espacio para empujar
    cmp byte [r14 + rcx], ' '
    jne cant_push
    
    ; Actualizar posicion de la X
    mov byte [r14 + rcx], 'X'
    jmp update_position
    
remove_x:
    ; Eliminar la 'X' de la posicion actual
    mov ecx, ebx        ; y
    imul ecx, 35        ; y * WIDTH
    add ecx, eax        ; y*WIDTH + x
    mov byte [r14 + rcx], ' '  ; Quitar la 'X'

    ; Decrementar remainingX
    mov ecx, [r15]      ; Cargar valor actual
    dec ecx             ; Decrementar
    mov [r15], ecx      ; Guardar valor actualizado
    
    jmp update_position
    
cant_push:
    ; No se puede empujar
    mov eax, [r12]
    mov ebx, [r13]
    jmp end_function
    
update_position:
    ; Limpiar antigua posicion del jugador
    mov ecx, [r13]      ; cargar y original
    imul ecx, 35        ; y * WIDTH
    add ecx, [r12]      ; y*WIDTH + x
    mov byte [r14 + rcx], ' '
    
    ; Actualizar gameArea 
    mov ecx, ebx        ; New y
    imul ecx, 35        ; y * WIDTH
    add ecx, eax        ; y*WIDTH + x
    mov byte [r14 + rcx], 'C'
    
    ; Actualizar las variables x, y
    mov [r12], eax
    mov [r13], ebx
    
end_function:
    ; Restaurar registros
    pop r15
    pop r14
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    pop rbp
    ret

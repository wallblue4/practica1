#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#define SHM_KEY 12345
#define SHM_SIZE 1024


int main() {

    //creacion del segmento de memoria compartida
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Conectarse a la memoria compartida
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    int opcion;
    
    do {
        printf("==== Bienvenido ====\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
        
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            int IdStart,IdDestiny,hod;
            case 1:
                printf("Ingrese la direccion de origen\n");
                scanf("%d",&IdStart);

                if(IdStart >= 0 && IdStart <= 1160) {
                printf("El id ingresado ha sido validado........ pulse para volver al menu\n");
                 } else {
                    printf("El id ingresado no es valido.\n");
                }
                //system("pause");
                break;
            case 2:
                printf("Ingrese la direccion del destino\n");
                scanf("%d",&IdDestiny);
                 if(IdDestiny >= 0 && IdDestiny <= 1160) {
                printf("El id ingresado ha sido validado.\n");
                 } else {
                    printf("El id ingresado no es valido.\n");
                }
                break;
            case 3:
                printf("Ingrese la hora\n");
                scanf("%d",&hod);
                if(hod >= 0 && hod <= 23) {
                printf("la hora se ha registrado exitosamente.\n");
                 } else {
                    printf("la hora ingresada no es valida.\n");
                }
                
                break;
            case 4: printf("su solicitud esta en progreso \n");
                char* key = malloc(strlen(IdStart) + strlen(IdDestiny) + strlen(hod) + 1);
                strcpy(shm_ptr, key); // Escribir en la memoria compartida
                sleep(2); //espera a que la busqueda en el archvo Hash termine
                printf("tiempo de viaje medio: %s",shm_ptr);
                shmdt(shm_ptr); //desconectarse del segmento de memoria compartida
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida. Por favor selecciona una opcion valida.\n");
                break;
        }
        
        printf("\n");
    } while (opcion != 0 && opcion !=5);
    
    return 0;
}